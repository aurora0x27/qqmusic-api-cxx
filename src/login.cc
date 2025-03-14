#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <botan/auto_rng.h>
#include <chrono>
#include <cmath>
#include <ctime>
#include <qqmusic/details/api.h>
#include <qqmusic/login.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/session.h>
#include <ratio>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>

static qqmusic::Task<qqmusic::Result<qqmusic::utils::Credential>> auth_qq_qr(std::string_view sigx,
                                                                             std::string_view uin);

static qqmusic::Task<qqmusic::Result<qqmusic::utils::Credential>> auth_wx_qr(std::string_view code);

namespace qqmusic {

qqmusic::Task<qqmusic::Result<QRCode>> get_qrcode(QRLoginType login_type) {
    namespace http = boost::beast::http;
    namespace asio = boost::asio;
    auto session = utils::SessionManager::get_instance().get_session();
    auto& context = session.get_context_ref();
    if (login_type == QRLoginType::QQ) {
        /*qq login*/
        boost::url url{"https://ssl.ptlogin2.qq.com/ptqrshow"};

        Botan::AutoSeeded_RNG rng;
        /*random unsigned long long generator*/
        auto randull = [&rng]() -> uint64_t {
            uint64_t res = 0;
            std::vector<uint8_t> buffer(sizeof(res));
            rng.randomize(buffer);
            memcpy(&res, buffer.data(), sizeof(res));
            return res;
        };
        url.set_params({{"appid", "716027609"},
                        {"e", "2"},
                        {"l", "M"},
                        {"s", "3"},
                        {"d", "72"},
                        {"v", "4"},
                        /* origin python code: `"t": str(random.random()),` */
                        {"t", "0." + std::to_string(randull() % 10000000000000000)},
                        {"daid", "383"},
                        {"pt_3rd_aid", "100497308"}});

        http::request<http::string_body> req{http::verb::get, url, 11};
        req.set(http::field::host, url.host());
        req.set(http::field::accept, "*/*");
        /*use raw buffer instead of compressed buffer when debuging*/
        req.set(http::field::accept_encoding, "gzip, deflate");
        req.set(http::field::connection, "keep-alive");
        req.set(http::field::user_agent,
                "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");
        req.set(http::field::referer, "https://xui.ptlogin2.qq.com/");

        /*send request*/
        auto res = co_await session.perform_request(url, req);
        if (res.isErr()) {
            co_return Err(utils::Exception(
                utils::Exception::NetworkError,
                std::format("[get_qrcode] -- Error occurred when performing https request: `{}`",
                            res.unwrapErr().what())));
        }
        auto data = utils::resp2buf(res.unwrap());
        auto qrsig_res = context.cookies.get("qrsig");
        auto qrsig = qrsig_res.unwrap();
        if (qrsig_res.isErr()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           "[get_qrcode] -- Cannot get qrsig from cookie"));
        }
        co_return Ok(QRCode{.qr_type = QRLoginType::QQ,
                            .identifier = qrsig,
                            .mimie_type = "image/png",
                            .data = data});
    } else {
        /*wx login*/
        boost::url uuid_url{"https://open.weixin.qq.com/connect/qrconnect"};
        uuid_url.set_params({
            {"appid", "wx48db31d50e334801"},
            {"redirect_uri",
             "https://y.qq.com/portal/wx_redirect.html?login_type=2&surl=https://y.qq.com/"},
            {"response_type", "code"},
            {"scope", "snsapi_login"},
            {"state", "STATE"},
            {"href",
             "https://y.qq.com/mediastyle/music_v17/src/css/popup_wechat.css#wechat_redirect"},
        });

        http::request<http::string_body> req{http::verb::get, uuid_url, 11};
        req.set(http::field::host, uuid_url.host());
        req.set(http::field::accept, "*/*");
        /*use raw buffer instead of compressed buffer when debuging*/
        req.set(http::field::accept_encoding, "identity");
        req.set(http::field::connection, "keep-alive");
        req.set(http::field::referer, "y.qq.com");
        req.set(http::field::user_agent,
                "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");

        /*send request*/
        auto res = co_await session.perform_request(uuid_url, req);
        if (res.isErr()) {
            co_return Err(utils::Exception(
                utils::Exception::NetworkError,
                std::format("[get_qrcode] -- Error occurred when performing https request: `{}`",
                            res.unwrapErr().what())));
        }

        auto resp = res.unwrap();
        std::string raw = boost::beast::buffers_to_string(resp.body().data());
        std::regex uuid_pattern{R"REGEX(uuid=(.+?)\")REGEX"};
        std::smatch matches;
        if (!std::regex_search(raw, matches, uuid_pattern)) {
            /*Not found*/
            co_return Err(utils::Exception(
                utils::Exception::DataDestroy,
                "[get_qrcode] -- No uuid found in wechat api response, data might be ruined"));
        }

        std::string&& uuid = matches[1].str();
        boost::url qr_url{std::format("https://open.weixin.qq.com/connect/qrcode/{}", uuid)};
        http::request<http::string_body> qr_req{http::verb::get, qr_url, 11};
        qr_req.set(http::field::host, qr_url.host());
        qr_req.set(http::field::accept, "*/*");
        /*use raw buffer instead of compressed buffer when debuging*/
        qr_req.set(http::field::accept_encoding, "identity");
        qr_req.set(http::field::connection, "keep-alive");
        qr_req.set(http::field::referer, "https://open.weixin.qq.com/connect/qrconnect");
        qr_req
            .set(http::field::user_agent,
                 "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                 "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");
        auto qr_result = co_await session.perform_request(qr_url, qr_req);
        if (qr_result.isErr()) {
            co_return Err(utils::Exception(
                utils::Exception::NetworkError,
                std::format("[get_qrcode] -- Error occurred when performing https request: `{}`",
                            qr_result.unwrapErr().what())));
        }
        auto image = utils::resp2buf(qr_result.unwrap());
        co_return Ok(QRCode{.qr_type = QRLoginType::WX,
                            .identifier = uuid,
                            .mimie_type = "image/jpeg",
                            .data = image});
    }
}

qqmusic::Task<qqmusic::Result<QRCodeLoginResult>> check_qq_qr(QRCode& qrc) {
    namespace http = boost::beast::http;
    if (qrc.qr_type != QRLoginType::QQ) {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       "[check_qq_qr] -- Not a QQ login QRCode"));
    }
    auto session = utils::SessionManager::get_instance().get_session();
    auto qrsig = qrc.identifier;

    /*zoned time cannot be used on MacOS platform yet*/
#ifdef PLATFORM_APPLE
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto duration = now.time_since_epoch() % 1000;
    auto millis = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration)
                      .count();
    std::tm local_tm{};
    localtime_r(&now_time_t, &local_tm);
    auto seconds = mktime(&local_tm);
    double ts = static_cast<double>(seconds) * 1000.0 + millis;
#else
    std::chrono::time_point<std::chrono::system_clock> tp{std::chrono::system_clock::now()};
    auto zoned_time = std::chrono::zoned_time{std::chrono::current_zone(), tp};
    double ts = std::chrono::duration<double, std::milli>(
                    zoned_time.get_local_time().time_since_epoch())
                    .count();
#endif

    boost::url url{"https://ssl.ptlogin2.qq.com/ptqrlogin"};
    url.set_params({
        {"u1", "https://graph.qq.com/oauth2.0/login_jump"},
        {"ptqrtoken", std::to_string(utils::hash33(qrsig))},
        {"ptredirect", "0"},
        {"h", "1"},
        {"t", "1"},
        {"g", "1"},
        {"from_ui", "1"},
        {"ptlang", "2052"},
        {"action", std::format("0-0-{}", ts)},
        {"js_ver", "20102616"},
        {"js_type", "1"},
        {"pt_uistyle", "40"},
        {"aid", "716027609"},
        {"daid", "383"},
        {"pt_3rd_aid", "100497308"},
        {"has_onekey", "1"},
    });
    http::request<http::string_body> qr_req{http::verb::get, url, 11};
    qr_req.set(http::field::host, url.host());
    qr_req.set(http::field::accept, "*/*");
    /*use raw buffer instead of compressed buffer when debuging*/
    qr_req.set(http::field::accept_encoding, "identity");
    qr_req.set(http::field::connection, "keep-alive");
    qr_req.set(http::field::referer, "https://xui.ptlogin2.qq.com/");
    qr_req.set(http::field::cookie, std::format("qrsig={}", qrsig));
    qr_req.set(http::field::user_agent,
               "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
               "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");

    auto qr_res = co_await session.perform_request(url, qr_req);
    if (qr_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[check_qq_qr] -- Error occurred when performing https request: `{}`",
                        qr_res.unwrapErr().what())));
    }

    auto resp = qr_res.unwrap();
    std::string raw = boost::beast::buffers_to_string(resp.body().data());
    /*match status*/
    std::regex list_pat{R"REGEX(ptuiCB\((.*)\))REGEX"};
    std::vector<std::string> items;
    std::smatch list_match;
    if (std::regex_match(raw, list_match, list_pat)) {
        auto list = list_match[1].str();
        auto list1 = list;
        std::regex item_pat{R"REGEX(\'([^,']*)\')REGEX"};
        std::smatch item_match;
        while (std::regex_search(list1, item_match, item_pat)) {
            items.push_back(item_match[1].str());
            list1 = item_match.suffix().str();
        }
    } else {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       "[check_qq_qr] -- Failed to get QR Code status"));
    }

    int code;
    try {
        code = std::stoi(items[0]);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       "[check_qq_qr] -- Failed parsing status code"));
    }

    auto event = QRCodeLoginEvent(code);
    if (event == QRCodeLoginEvent::Status::DONE) {
        /*Authorize*/
        std::regex sigx_pat{R"REGEX(&ptsigx=(.+?)&s_url)REGEX"};
        std::regex uin_pat{R"REGEX(&uin=(.+?)&service)REGEX"};
        std::smatch sigx_match, uin_match;
        if (!std::regex_search(items[2], sigx_match, sigx_pat)) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           "[check_qq_qr] -- Cannot find sigx"));
        }
        if (!std::regex_search(items[2], uin_match, uin_pat)) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           "[check_qq_qr] -- Cannot find uin"));
        }
        std::string sigx = sigx_match[1].str();
        std::string uin = uin_match[1].str();
        auto auth_res = co_await auth_qq_qr(sigx, uin);
        if (auth_res.isErr()) {
            co_return Err(auth_res.unwrapErr());
        }
        co_return Ok(QRCodeLoginResult{event, auth_res.unwrap()});
    }

    co_return Ok(QRCodeLoginResult{event, std::nullopt});
}

qqmusic::Task<qqmusic::Result<QRCodeLoginResult>> check_wx_qr(QRCode& qrc) {
    namespace http = boost::beast::http;
    if (qrc.qr_type != QRLoginType::WX) {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       "[check_qq_qr] -- Not a WX login QRCode"));
    }
    auto session = utils::SessionManager::get_instance().get_session();
    auto uuid = qrc.identifier;

#ifdef PLATFORM_APPLE
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto duration = now.time_since_epoch() % 1000;
    auto millis = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration)
                      .count();
    std::tm local_tm{};
    localtime_r(&now_time_t, &local_tm);
    auto seconds = mktime(&local_tm);
    double ts = static_cast<double>(seconds) * 1000.0 + millis;
#else
    std::chrono::time_point<std::chrono::system_clock> tp{std::chrono::system_clock::now()};
    auto zoned_time = std::chrono::zoned_time{std::chrono::current_zone(), tp};
    double ts = std::chrono::duration<double, std::milli>(
                    zoned_time.get_local_time().time_since_epoch())
                    .count()
                / 1000;
#endif

    boost::url url{"https://lp.open.weixin.qq.com/connect/l/qrconnect"};
    url.set_params({{"uuid", uuid}, {"_", std::to_string(lround(ts) * 1000)}});

    http::request<http::string_body> req{http::verb::get, url, 11};
    req.set(http::field::host, url.host());
    req.set(http::field::accept, "*/*");
    /*use raw buffer instead of compressed buffer when debuging*/
    req.set(http::field::accept_encoding, "identity");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::referer, "https://open.weixin.qq.com/");
    req.set(http::field::user_agent,
            "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");

    auto res = co_await session.perform_request(url, req);
    if (res.isErr()) {
        if (res.unwrapErr().get_error_enum() == utils::Exception::OperationOutOfTime) {
            co_return Ok(QRCodeLoginResult({QRCodeLoginEvent::Status::SCAN, std::nullopt}));
        }

        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[check_wx_qr] -- Error occurred when performing https request: `{}`",
                        res.unwrapErr().what())));
    }

    auto resp = res.unwrap();
    auto text = boost::beast::buffers_to_string(resp.body().data());
    std::regex pat{R"REGEX(window\.wx_errcode=(\d+);window\.wx_code=\'([^\']*)\')REGEX"};
    std::smatch match;
    if (!std::regex_search(text, match, pat)) {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       "[check_wx_qr] -- Cannot get QR Code status"));
    }

    int wx_errcode = 0;
    try {
        wx_errcode = std::stoi(match[1].str());
    } catch (const std::exception&) {
        /*Not a number*/
        co_return Ok(QRCodeLoginResult{QRCodeLoginEvent::Status::OTHER, std::nullopt});
    }

    auto event = QRCodeLoginEvent(wx_errcode);
    if (event == QRCodeLoginEvent::Status::DONE) {
        auto wx_code = match[2].str();
        if (!wx_code.size()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           "[check_wx_qr] -- Cannot get wx code"));
        }

        auto credential_res = co_await auth_wx_qr(wx_code);
        if (credential_res.isErr()) {
            co_return Err(
                utils::Exception(utils::Exception::CredentialInvalidError,
                                 std::format("[check_wx_qr] -- Cannot get credential: `{}`",
                                             credential_res.unwrapErr().what())));
        }
        co_return Ok(QRCodeLoginResult{event, credential_res.unwrap()});
    }

    co_return Ok(QRCodeLoginResult({event, std::nullopt}));
}

qqmusic::Task<qqmusic::Result<PhoneLoginResult>> send_authcode(std::string_view phone,
                                                               std::string_view country_code) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.login.LoginServer",
                            "SendPhoneAuthCode",
                            {},
                            {{"tmeLoginMethod", "3"}});
    auto req_res = co_await api.prepare_request({
        {"tmeAppid", "qqmusic"},
        {"phoneNo", phone},
        {"areaCode", country_code},
    });

    if (req_res.isErr()) {
        co_return Err(utils::Exception(
            qqmusic::utils::Exception::DataDestroy,
            std::format("[send_authcode] -- Error occurred when preparing request: `{}`",
                        req_res.unwrapErr().what())));
    }

    auto url = req_res.unwrap().url;
    auto req = req_res.unwrap().req;
    auto res = co_await session.perform_request(url, req);
    if (res.isErr()) {
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[send_authcode] -- Network Error when request for security url: `{}`",
                        res.unwrapErr().what())));
    }

    auto resp_parse_res = api.parse_response(utils::resp2buf(res.unwrap()));
    if (resp_parse_res.isErr()) {
        co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                                "[send_authcode] -- Cannot parse response"));
    }

    auto resp = resp_parse_res.unwrap();
    try {
        int64_t code = resp["code"].get<int64_t>();
        switch (code) {
        case 0:
            co_return Ok(PhoneLoginResult{PhoneLoginEvent::SEND, ""});
        case 20276:
            co_return Ok(PhoneLoginResult{PhoneLoginEvent::CAPTCHA, resp["data"]["securityURL"]});
        case 100001:
            co_return Ok(PhoneLoginResult{PhoneLoginEvent::FREQUENCY, ""});
        default:
            /*Error*/
            co_return Ok(PhoneLoginResult{PhoneLoginEvent::OTHER, resp["data"]["errMsg"]});
        }
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            "[send_authcode] -- Cannot parse security url or error message from json"));
    }
}

qqmusic::Task<qqmusic::Result<utils::Credential>> phone_authorize(std::string_view phone,
                                                                  std::string_view auth_code,
                                                                  std::string_view country_code) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.login.LoginServer",
                            "Login",
                            {},
                            {{"tmeLoginMethod", "3"}, {"tmeLoginType", "0"}});
    auto req_res = co_await api.prepare_request(
        {{"code", auth_code}, {"phoneNo", phone}, {"areaCode", country_code}, {"loginMode", 1}});
    if (req_res.isErr()) {
        co_return Err(utils::Exception(
            qqmusic::utils::Exception::DataDestroy,
            std::format("[phone_authorize] -- Error occurred when preparing request: `{}`",
                        req_res.unwrapErr().what())));
    }
    auto url = req_res.unwrap().url;
    auto req = req_res.unwrap().req;
    auto res = co_await session.perform_request(url, req);
    if (res.isErr()) {
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[phone_authorize] -- Network Error when request for credential: `{}`",
                        res.unwrapErr().what())));
    }
    auto resp_parse_res = api.parse_response(utils::resp2buf(res.unwrap()));
    if (resp_parse_res.isErr()) {
        co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                                "[phone_authorize] -- Cannot parse response"));
    }
    auto resp = resp_parse_res.unwrap();
    try {
        int64_t code = resp["code"].get<int64_t>();
        switch (code) {
        case 0:
            co_return Ok(utils::Credential(resp["data"]));
        case 20271:
            co_return Err(
                utils::Exception(utils::Exception::LoginError,
                                 "[phone_authorize] -- Auth_code error or already authorized"));
        default:
            /*Error*/
            co_return Err(
                utils::Exception(utils::Exception::UnknownError,
                                 "[phone_authorize] -- Authorize failed for unknown error"));
        }
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            "[send_authcode] -- Cannot parse security url or error message from json"));
    }
    co_return Err(utils::Exception(utils::Exception::UnknownError, "Not implemented yet"));
}

} // namespace qqmusic

static qqmusic::Task<qqmusic::Result<qqmusic::utils::Credential>> auth_qq_qr(std::string_view sigx,
                                                                             std::string_view uin) {
    namespace http = boost::beast::http;
    auto session = qqmusic::utils::SessionManager::get_instance().get_session();
    auto& context = session.get_context_ref();
    boost::url p_skey_url{"https://ssl.ptlogin2.graph.qq.com/check_sig"};
    p_skey_url.set_params({
        {"uin", uin},
        {"pttype", "1"},
        {"service", "ptqrlogin"},
        {"nodirect", "0"},
        {"ptsigx", sigx},
        {"s_url", "https://graph.qq.com/oauth2.0/login_jump"},
        {"ptlang", "2052"},
        {"ptredirect", "100"},
        {"aid", "716027609"},
        {"daid", "383"},
        {"j_later", "0"},
        {"low_login_hour", "0"},
        {"regmaster", "0"},
        {"pt_login_type", "3"},
        {"pt_aid", "0"},
        {"pt_aaid", "16"},
        {"pt_light", "0"},
        {"pt_3rd_aid", "100497308"},
    });

    http::request<http::string_body> p_skey_req{http::verb::get, p_skey_url, 11};
    p_skey_req.set(http::field::host, p_skey_url.host());
    p_skey_req.set(http::field::accept, "*/*");
    /*use raw buffer instead of compressed buffer when debuging*/
    p_skey_req.set(http::field::accept_encoding, "identity");
    p_skey_req.set(http::field::connection, "keep-alive");
    p_skey_req.set(http::field::referer, "https://xui.ptlogin2.qq.com/");
    p_skey_req
        .set(http::field::user_agent,
             "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
             "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");

    auto pskey_resp_res = co_await session.perform_request(p_skey_url, p_skey_req);
    if (pskey_resp_res.isErr()) {
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[auth_qq_qr] -- Network Error when request for p_skey: `{}`",
                        pskey_resp_res.unwrapErr().what())));
    }

    auto getkey_res = context.cookies.get("p_skey");
    if (getkey_res.isErr()) {
        co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::NetworkError,
                                                "[auth_qq_qr] -- Cannot get p_skey"));
    }
    auto p_skey = getkey_res.unwrap();

    boost::url location_url{"https://graph.qq.com/oauth2.0/authorize"};

    /* regex R"REGEX(\&*(?:([^&=]+)\=([^&=]+))\&*)REGEX" 
     * can be used to parse application/x-www-form-urlencoded format data
     * */

    boost::uuids::uuid u = boost::uuids::random_generator()();
    auto timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();
    std::string timestamp_str = std::to_string(timestamp_ms - timestamp_ms % 1000);

    std::vector<std::pair<std::string, std::string>>
        params{{"response_type", "code"},
               {"client_id", "100497308"},
               {"redirect_uri",
                "https%3A%2F%2Fy.qq.com%2Fportal%2Fwx_redirect.html%3Flogin_type%3D1%26surl%"
                "3Dhttps%253A%25252F%25252Fy.qq.com%25252F"},
               {"scope", "get_user_info%2Cget_app_friends"},
               {"state", "state"},
               {"switch", ""},
               {"from_ptlogin", "1"},
               {"src", "1"},
               {"update_auth", "1"},
               {"openapi", "1010_1030"},
               {"g_tk", std::to_string(qqmusic::utils::hash33(p_skey, 5381))},
               {"auth_time", timestamp_str},
               {"ui", boost::uuids::to_string(u)}};

    std::ostringstream oss;
    for (auto& i : params) {
        oss << i.first << "=" << i.second << "&";
    }
    auto data = oss.str();
    data.erase(data.end() - 1);
    http::request<http::string_body> location_req{http::verb::post, location_url, 11};
    location_req.set(http::field::host, location_url.host());
    location_req.set(http::field::accept, "*/*");
    /*use raw buffer instead of compressed buffer when debuging*/
    location_req.set(http::field::accept_encoding, "identity");
    location_req.set(http::field::connection, "keep-alive");
    location_req
        .set(http::field::user_agent,
             "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
             "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");
    location_req.set(http::field::content_type, "application/x-www-form-urlencoded");
    auto cookie1_dump = context.cookies.serialize("graph.qq.com");
    auto cookie2_dump = context.cookies.serialize("qq.com");
    if (cookie1_dump.isErr() || cookie2_dump.isErr()) {
        co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::JsonError,
                                                "[auth_qq_qr] -- Cannot set cookie"));
    }
    auto cookie_str = cookie1_dump.unwrap() + "; " + cookie2_dump.unwrap();
    location_req.set(http::field::cookie, cookie_str);
    location_req.body() = data;

    location_req.prepare_payload();

    auto location_res = co_await session.perform_request(location_url, location_req, false);
    if (location_res.isErr()) {
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[auth_qq_qr] -- Error occurred when getting location: `{}`",
                        location_res.unwrapErr().what())));
    }

    auto headers = location_res.unwrap().base();
    std::string location;
    if (auto pos = headers.find(http::field::location); pos == headers.end()) {
        /*Not find*/
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::DataDestroy,
            "[auth_qq_qr] -- Cannot find location, data might be ruined"));
    } else {
        location = pos->value();
    }

    std::regex code_pat{R"REGEX(code=([^&]+))REGEX"};
    std::smatch code_match;
    if (!std::regex_search(location, code_match, code_pat)) {
        co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                                "[auth_qq_qr] -- Cannot get code from location"));
    }

    auto code = code_match[1].str();
    auto api = qqmusic::details::Api(session,
                                     "music.login.LoginServer",
                                     "Login",
                                     {},
                                     nlohmann::json{{"tmeLoginType", "2"}});
    auto credential_req = co_await api.prepare_request({{"code", code}});
    if (credential_req.isErr()) {
        if (credential_req.unwrapErr().get_error_enum()
            == qqmusic::utils::Exception::CredendialExpiredError) {
            co_return Err(
                qqmusic::utils::Exception(qqmusic::utils::Exception::LoginError,
                                          "[auth_wx_qr] -- Cannot authorize for multiple times"));
        }
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::DataDestroy,
            std::format("[auth_qq_qr] -- Error occurred when preparing request: `{}`",
                        credential_req.unwrapErr().what())));
    }

    auto cred_url = credential_req.unwrap().url;
    auto cred_req = credential_req.unwrap().req;
    auto credential_res = co_await session.perform_request(cred_url, cred_req);
    if (credential_res.isErr()) {
        if (credential_res.unwrapErr().get_error_enum()
            == qqmusic::utils::Exception::CredendialExpiredError) {
            co_return Err(
                qqmusic::utils::Exception(qqmusic::utils::Exception::LoginError,
                                          "[auth_qq_qr] -- Cannot authorize for multiple times"));
        }
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[auth_qq_qr] -- Error occurred when getting credential: `{}`",
                        credential_res.unwrapErr().what())));
    }

    auto credential_raw_res = api.parse_response(qqmusic::utils::resp2buf(credential_res.unwrap()));
    if (credential_raw_res.isErr()) {
        co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                                "[auth_qq_qr] -- Cannot parse response"));
    }
    try {
        co_return Ok(qqmusic::utils::Credential(credential_raw_res.unwrap()));
    } catch (const std::exception& e) {
        co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::JsonError,
                                                "[auth_qq_qr] -- Cannot parse credential"));
    }
}

static qqmusic::Task<qqmusic::Result<qqmusic::utils::Credential>> auth_wx_qr(std::string_view code) {
    auto session = qqmusic::utils::SessionManager::get_instance().get_session();
    auto api = qqmusic::details::Api(session,
                                     "music.login.LoginServer",
                                     "Login",
                                     {},
                                     {{"tmeLoginType", "1"}});
    auto req_res = co_await api.prepare_request(
        {{"code", code}, {"strAppid", "wx48db31d50e334801"}});
    if (req_res.isErr()) {
        if (req_res.unwrapErr().get_error_enum()
            == qqmusic::utils::Exception::CredendialExpiredError) {
            co_return Err(
                qqmusic::utils::Exception(qqmusic::utils::Exception::LoginError,
                                          "[auth_wx_qr] -- Cannot authorize for multiple times"));
        }
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::DataDestroy,
            std::format("[auth_wx_qr] -- Error occurred when preparing request: `{}`",
                        req_res.unwrapErr().what())));
    }

    auto cred_url = req_res.unwrap().url;
    auto cred_req = req_res.unwrap().req;
    auto credential_res = co_await session.perform_request(cred_url, cred_req);
    if (credential_res.isErr()) {
        if (credential_res.unwrapErr().get_error_enum()
            == qqmusic::utils::Exception::CredendialExpiredError) {
            co_return Err(
                qqmusic::utils::Exception(qqmusic::utils::Exception::LoginError,
                                          "[auth_wx_qr] -- Cannot authorize for multiple times"));
        }
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[auth_qq_qr] -- Error occurred when getting credential: `{}`",
                        credential_res.unwrapErr().what())));
    }

    auto credential_raw_res = api.parse_response(qqmusic::utils::resp2buf(credential_res.unwrap()));
    if (credential_raw_res.isErr()) {
        co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                                "[auth_wx_qr] -- Cannot parse response"));
    }
    try {
        co_return Ok(qqmusic::utils::Credential(credential_raw_res.unwrap()));
    } catch (const std::exception& e) {
        co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::JsonError,
                                                "[auth_wx_qr] -- Cannot parse credential"));
    }
}
