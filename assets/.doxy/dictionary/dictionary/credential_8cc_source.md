

# File credential.cc

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**utils**](dir_313caf1132e152dd9b58bea13a4052ca.md) **>** [**credential.cc**](credential_8cc.md)

[Go to the documentation of this file](credential_8cc.md)


```C++
#include <format>
#include <nlohmann/detail/conversions/from_json.hpp>
#include <nlohmann/detail/conversions/to_json.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/credential.h>
#include <qqmusic/utils/session.h>
#include <string>
#include <string_view>

namespace qqmusic::utils {

/* NOTE: Assume that only loginType and musickey field can be defaulted*/
Credential::Credential(nlohmann::json& cookie) {
    /*Write back the update result*/
    cookie.at("openid").get_to(openid);
    cookie.erase("openid");
    cookie.at("refresh_token").get_to(refresh_token);
    cookie.erase("refresh_token");
    cookie.at("access_token").get_to(access_token);
    cookie.erase("access_token");
    cookie.at("expired_at").get_to(expired_at);
    cookie.erase("expired_at");
    cookie.at("musicid").get_to(musicid);
    cookie.erase("musicid");
    cookie.at("unionid").get_to(unionid);
    cookie.erase("unionid");
    cookie.at("str_musicid").get_to(str_musicid);
    cookie.erase("str_musicid");
    cookie.at("musickey").get_to(musickey);
    cookie.erase("musickey");
    cookie.at("refresh_key").get_to(refresh_key);
    cookie.erase("refresh_key");
    cookie.at("encryptUin").get_to(encryptUin);
    cookie.erase("encryptUin");
    cookie.at("loginType").get_to(loginType);
    cookie.erase("loginType");
    extra_fields = cookie;
    if (cookie.contains("musickey")) {
        musickey = cookie["musickey"].get<std::string>();

        if (!cookie.contains("loginType") && musickey.substr(0, 3) == "W_X") {
            loginType = 1;
        } else {
            loginType = 2;
        }
    }
}

// TODO: is there a way to delete the repeat code here ?
Credential::Credential(std::string_view cookie) {
    auto data = nlohmann::json::parse(cookie);
    /*Write back the update result*/
    data.at("openid").get_to(openid);
    data.erase("openid");
    data.at("refresh_token").get_to(refresh_token);
    data.erase("refresh_token");
    data.at("access_token").get_to(access_token);
    data.erase("access_token");
    data.at("expired_at").get_to(expired_at);
    data.erase("expired_at");
    data.at("musicid").get_to(musicid);
    data.erase("musicid");
    data.at("unionid").get_to(unionid);
    data.erase("unionid");
    data.at("str_musicid").get_to(str_musicid);
    data.erase("str_musicid");
    data.at("musickey").get_to(musickey);
    data.erase("musickey");
    data.at("refresh_key").get_to(refresh_key);
    data.erase("refresh_key");
    data.at("encryptUin").get_to(encryptUin);
    data.erase("encryptUin");
    data.at("loginType").get_to(loginType);
    data.erase("loginType");
    extra_fields = data;
    if (data.contains("musickey")) {
        musickey = data["musickey"].get<std::string>();

        if (!data.contains("loginType") && musickey.substr(0, 3) == "W_X") {
            loginType = 1;
        } else {
            loginType = 2;
        }
    }
}

bool Credential::from_cache(std::string_view cache) {
    /*Write back the update result*/
    try {
        auto data = nlohmann::json::parse(cache);
        data.at("openid").get_to(openid);
        data.at("refresh_token").get_to(refresh_token);
        data.at("access_token").get_to(access_token);
        data.at("expired_at").get_to(expired_at);
        data.at("musicid").get_to(musicid);
        data.at("unionid").get_to(unionid);
        data.at("str_musicid").get_to(str_musicid);
        data.at("musickey").get_to(musickey);
        data.at("refresh_key").get_to(refresh_key);
        data.at("encryptUin").get_to(encryptUin);
        data.at("loginType").get_to(loginType);
        data.at("extra_fields").get_to(extra_fields);
    } catch (const std::exception& e) {
        return true;
    }
    return false;
}

bool Credential::from_cache(nlohmann::json& cache) {
    /*Write back the update result*/
    try {
        cache.at("openid").get_to(openid);
        cache.at("refresh_token").get_to(refresh_token);
        cache.at("access_token").get_to(access_token);
        cache.at("expired_at").get_to(expired_at);
        cache.at("musicid").get_to(musicid);
        cache.at("unionid").get_to(unionid);
        cache.at("str_musicid").get_to(str_musicid);
        cache.at("musickey").get_to(musickey);
        cache.at("refresh_key").get_to(refresh_key);
        cache.at("encryptUin").get_to(encryptUin);
        cache.at("loginType").get_to(loginType);
        cache.at("extra_fields").get_to(extra_fields);
    } catch (const std::exception& e) {
        return true;
    }
    return false;
}

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
        res["extra_fields"] = extra_fields;
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
        res["extra_fields"] = extra_fields;
        return Ok(res.dump());
    } catch (const std::exception& e) {
        return Err(
            Exception(Exception::JsonError,
                      std::format("[Credential::as_string] -- failed to generate json string: {}",
                                  e.what())));
    }
}

qqmusic::Task<qqmusic::Result<bool>> Credential::is_expired() {
    auto session = SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.UserInfo.userInfoServer", "GetLoginUserInfo", *this);
    auto req_param_res = co_await api.prepare_request(nlohmann::json::object());
    if (req_param_res.isErr()) {
        co_return Err(Exception(
            Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[Credential::is_expired] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }
    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(Exception(
            Exception::Kind(resp_res.unwrapErr().get_error_enum()),
            std::format("[Credential::is_expired] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }
    auto json_res = api.parse_response(resp2buf(resp_res.unwrap()));
    if (json_res.isErr()) {
        switch (json_res.unwrapErr().get_error_enum()) {
            using namespace qqmusic::utils;
        case Exception::CredendialExpiredError:
            co_return Ok(true);
        case Exception::SignInvalidError:
            co_return Err(json_res.unwrapErr());
        default:
            co_return Ok(false);
        }
    }
    co_return Ok(false);
}

qqmusic::Task<qqmusic::Result<void>> Credential::refresh() {
    auto session = SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.login.LoginServer",
                            "Login",
                            *this,
                            {"tmeLoginType", std::to_string(loginType)});

    nlohmann::json params = {
        {"refresh_key", refresh_key},
        {"refresh_token", refresh_token},
        {"musickey", musickey},
        {"musicid", musicid},
    };

    auto request_res = co_await api.prepare_request(params);
    if (request_res.isErr()) {
        co_return Err(Exception(
            Exception::Kind(request_res.unwrapErr().get_error_enum()),
            std::format("[Credential::refresh] -- Error occurred when preparing request: `{}`",
                        request_res.unwrapErr().what())));
    }

    auto url = request_res.unwrap().url;
    auto req = request_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(Exception(response_res.unwrapErr()));
    }

    auto json_res = api.parse_response(resp2buf(response_res.unwrap()));
    if (json_res.isErr()) {
        co_return Err(json_res.unwrapErr());
    }

    auto json = json_res.unwrap();
    try {
        /*Write back the update result*/
        json.at("openid").get_to(openid);
        json.erase("openid");
        json.at("refresh_token").get_to(refresh_token);
        json.erase("refresh_token");
        json.at("access_token").get_to(access_token);
        json.erase("access_token");
        json.at("expired_at").get_to(expired_at);
        json.erase("expired_at");
        json.at("musicid").get_to(musicid);
        json.erase("musicid");
        json.at("unionid").get_to(unionid);
        json.erase("unionid");
        json.at("str_musicid").get_to(str_musicid);
        json.erase("str_musicid");
        json.at("musickey").get_to(musickey);
        json.erase("musickey");
        json.at("refresh_key").get_to(refresh_key);
        json.erase("refresh_key");
        json.at("encryptUin").get_to(encryptUin);
        json.erase("encryptUin");
        json.at("loginType").get_to(loginType);
        json.erase("loginType");
        extra_fields = json;
    } catch (const std::exception& e) {
        co_return Err(
            Exception(Exception::JsonError,
                      std::format("[Credential::refresh] -- Cannot write back from json: {}",
                                  e.what())));
    }
    co_return Ok();
}

} // namespace qqmusic::utils
```


