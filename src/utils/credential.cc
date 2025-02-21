#include "qqmusic/result.h"
#include <format>
#include <nlohmann/detail/conversions/from_json.hpp>
#include <nlohmann/detail/conversions/to_json.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/utils/credential.h>
#include <string>
#include <string_view>

namespace qqmusic::utils {

/* NOTE: Assume that only loginType and musickey field can be defaulted*/
Credential::Credential(const nlohmann::json& cookie)
    : Credential() {
    nlohmann::from_json(cookie, *this);
    if (cookie.contains("musickey")) {
        musickey = cookie["musickey"].get<std::string>();

        if (!cookie.contains("loginType") && musickey.substr(0, 3) == "W_X") {
            loginType = 1;
        } else {
            loginType = 2;
        }
    }
}

Credential::Credential(std::string_view cookie)
    : Credential(nlohmann::json::parse(cookie)) {}

bool Credential::is_valid() const {
    if (musickey.size() == 0 || musicid == 0) {
        /*credential have no musickey or musicid is not valid*/
        return false;
    }
    return true;
}

qqmusic::Result<nlohmann::json> Credential::to_json() {
    try {
        nlohmann::json res;
        nlohmann::to_json(res, *this);
        return Ok(res);
    } catch (const std::exception& e) {
        return Err(Exception(Exception::DataDestroy,
                             std::format("[Credential::as_json] -- failed to generate json: {}",
                                         e.what())));
    }
}

qqmusic::Result<std::string> Credential::to_string() {
    try {
        nlohmann::json res;
        nlohmann::to_json(res, *this);
        return Ok(res.dump());
    } catch (const std::exception& e) {
        return Err(
            Exception(Exception::JsonError,
                      std::format("[Credential::as_string] -- failed to generate json string: {}",
                                  e.what())));
    }
}

qqmusic::Task<qqmusic::Result<bool>> Credential::is_expired() {
    // auto api = details::Api("music.UserInfo.userInfoServer", "GetLoginUserInfo", *this);
    // auto resp_res = co_await api.do_request(
    //     nlohmann::json::object()); /*use nlohmann::json::object as `{}`*/
    // if (resp_res.isErr()) {
    //     co_return Err(resp_res.unwrapErr());
    // }
    // auto response = resp_res.unwrap();
    // auto json_res = api.parse_response(response);
    // if (json_res.isErr()) {
    //     switch (json_res.unwrapErr().get_error_enum()) {
    //         using namespace qqmusic::utils;
    //     case Exception::CredendialExpiredError:
    //         co_return Ok(true);
    //     case Exception::SignInvalidError:
    //         co_return Err(std::move(json_res.unwrapErr()));
    //     default:
    //         co_return Ok(false);
    //     }
    // }
    // co_return Ok(false);
    co_return Err(Exception(Exception::UnknownError, "Not implemented yet"));
}

qqmusic::Task<qqmusic::Result<void>> Credential::refresh() {
    // auto api = qqmusic::details::Api("music.login.LoginServer",
    //                                  "Login",
    //                                  *this,
    //                                  {"tmeLoginType", std::to_string(loginType)});
    // nlohmann::json params = {
    //     {"refresh_key", refresh_key},
    //     {"refresh_token", refresh_token},
    //     {"musickey", musickey},
    //     {"musicid", musicid},
    // };

    // auto response_res = co_await api.do_request(params);
    // if (response_res.isErr()) {
    //     co_return Err(Exception(response_res.unwrapErr()));
    // }

    // auto response = response_res.unwrap();
    // auto json_res = api.parse_response(response);
    // if (json_res.isErr()) {
    //     co_return Err(std::move(json_res.unwrapErr()));
    // }

    // auto json = json_res.unwrap();
    // try {
    //     /*Write back the update result*/
    //     json.at("openid").get_to(openid);
    //     json.erase("openid");
    //     json.at("refresh_token").get_to(refresh_token);
    //     json.erase("refresh_token");
    //     json.at("access_token").get_to(access_token);
    //     json.erase("access_token");
    //     json.at("expired_at").get_to(expired_at);
    //     json.erase("expired_at");
    //     json.at("musicid").get_to(musicid);
    //     json.erase("musicid");
    //     json.at("unionid").get_to(unionid);
    //     json.erase("unionid");
    //     json.at("str_musicid").get_to(str_musicid);
    //     json.erase("str_musicid");
    //     json.at("musickey").get_to(musickey);
    //     json.erase("musickey");
    //     json.at("refresh_key").get_to(refresh_key);
    //     json.erase("refresh_key");
    //     json.at("encryptUin").get_to(encryptUin);
    //     json.erase("encryptUin");
    //     json.at("loginType").get_to(loginType);
    //     json.erase("loginType");
    //     extra_fields = json;
    // } catch (const std::exception& e) {
    //     co_return Err(Exception(
    //         Exception::JsonError,
    //         std::format("[Credential::refresh] -- Cannot write back from json: {}", e.what())));
    // }

    // co_return Ok();
    co_return Err(Exception(Exception::UnknownError, "Not implemented yet"));
}

} // namespace qqmusic::utils
