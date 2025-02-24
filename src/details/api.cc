#include <boost/beast/http/message.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/common.h>
#include <string>

namespace qqmusic::details {

qqmusic::Result<nlohmann::json> Api::parse_response(utils::buffer& response) {
    try {
        nlohmann::json resp_json = nlohmann::json::parse(response);
        nlohmann::json req_data;
        uint64_t rc = resp_json["code"].get<uint64_t>();
        switch (rc) {
        case 0:
            /* FIXME: discarded req_data["code"] value*/
            req_data = resp_json[module + "." + method];
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

qqmusic::Task<qqmusic::Result<RequestParam>> Api::prepare_request(const nlohmann::json& params) {
    try {
        auto& context = session.get_context_ref();
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

            /*set credential relative fields*/
            common["qq"] = std::to_string(credential.musicid);
            common["authst"] = credential.musickey;
            common["tmeLoginType"] = std::to_string(credential.loginType);
        }

        /*Build request data*/
        nlohmann::json request_data = {
            {"comm", common},
            {module + '.' + method, {{"module", module}, {"method", method}, {"param", params}}},
        };

        boost::url url{context.api_config.enable_sign ? context.api_config.enc_endpoint
                                                      : context.api_config.endpoint};

        if (context.api_config.enable_sign) {
            /*set request param sign*/
            url.set_params({{"sign", utils::sign(request_data)}});
        }

        /*set cookie*/
        context.cookies.set({.domain = "qq.com",
                             .path = "/",
                             .key = "uin",
                             .value = std::to_string(credential.musicid)});
        context.cookies.set(
            {.domain = "qq.com", .path = "/", .key = "qqmusic_key", .value = credential.musickey});
        context.cookies.set(
            {.domain = "qq.com", .path = "/", .key = "qm_keyst", .value = credential.musickey});
        context.cookies.set({.domain = "qq.com",
                             .path = "/",
                             .key = "tmeLoginType",
                             .value = std::to_string(credential.loginType)});

        session.sync_global();

        http::request<http::string_body> req{http::verb::post, url, 11};
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
        req.body() = nlohmann::to_string(request_data);

        auto cookie_res = context.cookies.serialize("qq.com");
        if (cookie_res.isErr()) {
            co_return Err(utils::Exception(
                utils::Exception::JsonError,
                std::format("[Api::prepare_request] -- Cannot serialize cookie: `{}`",
                            cookie_res.unwrapErr().what())));
        }
        req.set(http::field::cookie, cookie_res.unwrap());
        co_return Ok(RequestParam{url, req});

    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::UnknownError,
                             std::format("[Api::prepare_request] -- UnknownError occurred: `{}`",
                                         e.what())));
    }
}

} // namespace qqmusic::details
