#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/url/urls.hpp>
#include <netinet/tcp.h>
#include <nlohmann/json.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/details/common.h>
#include <qqmusic/details/network.h>
#include <qqmusic/result.h>
#include <string>

qqmusic::Result<nlohmann::json> qqmusic::details::Api::parse_response(
    qqmusic::utils::buffer& response) {
    try {
        nlohmann::json resp_json = nlohmann::json::parse(response);
        nlohmann::json req_data = resp_json[module + "." + method];
        int rc = req_data["code"].get<int>();
        switch (rc) {
        case 0:
            return Ok(req_data["data"]);
        case 2000:
            return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::SignInvalidError,
                                                 "[parse_response] -- Sign is invalid"));
        case 1000:
            return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::CredendialExpiredError,
                                                 "[parse_response] -- Credential is expired"));
        default:
            return Err(qqmusic::utils::Exception(
                qqmusic::utils::Exception::ResponseCodeError,
                std::format("[parse_response] -- Unexpected response code: {}", rc)));
        }
    } catch (const std::exception& e) {
        return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::JsonError,
            std::format("[parse_response] -- Error occurred when parsing json: {}", e.what())));
    }
}

qqmusic::Task<qqmusic::Result<qqmusic::utils::buffer>> qqmusic::details::Api::do_request(
    const nlohmann::json& params) {
    if (need_verify && !credential.is_valid()) {
        co_return Err(
            qqmusic::utils::Exception(qqmusic::utils::Exception::CredentialInvalidError,
                                      "[Api::do_request] -- Provided with invalid credential"));
    }

    /*get api context*/
    auto& ncm = qqmusic::details::NetworkContextManager::get_instance();
    auto context = ncm.get_global_context();

    common = {
        {"cv", context.api_config.version_code},
        {"v", context.api_config.version_code},
        {"QIMEI36", context.qimei.q36},
        {"ct", "11"},
        {"tmeAppID", "qqmusic"},
        {"format", "json"},
        {"inCharset", "utf-8"},
        {"outCharset", "utf-8"},
        {"uid", "3931641530"},
    };

    /*load this->credential first, if invalid, choose the one in context*/
    if (!credential.is_valid()) {
        credential = context.credential;
    }

    try {
        if (credential.is_valid()) {
            common["qq"] = std::to_string(credential.musicid);
            common["authst"] = credential.musickey;
            common["tmeLoginType"] = std::to_string(credential.loginType);
        }
    } catch (const std::exception& e) {
        std::cout << "here: " << e.what() << std::endl;
        exit(1);
    }

    /*Build request data*/
    nlohmann::json data = {
        {"comm", common},
        {module + '.' + method, {{"module", module}, {"method", method}, {"param", params}}},
    };

    nlohmann::json request_params = {{"url",
                                      /*use enc_endpoint if enable sign*/
                                      context.api_config.enable_sign
                                          ? context.api_config.enc_endpoint
                                          : context.api_config.endpoint},
                                     {"json", data}};

    if (context.api_config.enable_sign) {
        request_params["sign"] = qqmusic::details::sign(data);
    }

    /* FIXME:
     * Well, the problem is I haven't find a way to deal with cookie *qwq*
     * Since it is `automaticly` handled in httpx in origin python code
     * ```
     * def _set_cookies(credential: Credential | None, session: Session):
     *     credential = credential or session.credential or Credential()
     *     if credential.has_musicid() and credential.has_musickey():
     *         cookies = httpx.Cookies()
     *         cookies.set("uin", str(credential.musicid), domain="qq.com")
     *         cookies.set("qqmusic_key", credential.musickey, domain="qq.com")
     *         cookies.set("qm_keyst", credential.musickey, domain="qq.com")
     *         cookies.set("tmeLoginType", str(credential.login_type), domain="qq.com")
     *         session.cookies = cookies
     * ```
     * */

    /*set cookies*/
    if (credential.is_valid()) {
        auto ctx = ncm.get_global_context();
        ctx.cookie = {{
            "qq.com",
            {{"uin", std::to_string(credential.musicid)},
             {"qqmusic_key", credential.musickey},
             {"qm_keyst", credential.musickey},
             {"tmeLoginType", std::to_string(credential.loginType)}},
        }};
        ncm.set_global_context(ctx);
    }

    auto gen_cookie_str = [](const nlohmann::json& cookie_content) {
        /*generate cookie string from key-value pairs*/
        std::string res;
        try {
            for (auto& i : cookie_content.items()) {
                res += i.key();
                res += '=';
                res += i.value();
                res += "; ";
            }
            /*delete `; ` at the end of cookie string*/
            res.erase(res.size() - 2);
            return res;
        } catch (const std::exception& e) {
            return res;
        }
    };

    /*prepare payload*/
    boost::urls::url url(request_params["url"].get<std::string>());
    boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::post,
                                                                     url,
                                                                     11};

    req.set(http::field::host, url.host());
    req.set(http::field::accept, "*/*");
    /*use raw buffer instead of compressed buffer when debuging*/
    req.set(http::field::accept_encoding, "ideflate");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::referer, "y.qq.com");
    req.set(http::field::user_agent,
            "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");
    req.set(http::field::content_type, "application/json");
    req.set(http::field::cookie, gen_cookie_str(ncm.get_global_context().cookie["qq.com"]));

    req.body() = request_params["json"].dump();

    auto resp_res = co_await ncm.perform_request(url.host(), req);

    if (resp_res.isErr()) {
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::UnknownError,
            std::format("[Api::do_request] -- failed to perform request: {}",
                        resp_res.unwrapErr().what())));
    }

    auto resp = resp_res.unwrap();
    if (resp.result() != http::status::ok) {
        /*Response Code Error*/
        co_return Err(qqmusic::utils::Exception(resp.result_int()));
    }

    /* TODO: I haven't found a better way to convert http::request to normal buffer*/
    qqmusic::utils::buffer res(resp.body().data().buffer_bytes());
    std::size_t offset = 0;
    for (auto const& buffer_part : resp.body().data()) {
        std::memcpy(res.data() + offset, buffer_part.data(), buffer_part.size());
        offset += buffer_part.size();
    }

    co_return Ok(res);
}
