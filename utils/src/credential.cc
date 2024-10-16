#include <cstdint>
#include <utils/credential.h>
#include <json/json.h>
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <map>
#include <string>

// constructor
qqmusic::utils::Credential::Credential()
    : openid(""), refresh_token(""), access_token(""), expired_at(0), musicid(0),
      musickey(""), unionid(""), str_musicid(""), refresh_key(""), encrypt_uin(""), login_type(0) {}

// check if musickey starts with the given prefix
bool qqmusic::utils::Credential::starts_with(const std::string& str, const std::string& prefix) const {
    return str.rfind(prefix, 0) == 0;
}

// initialize and set login type
void qqmusic::utils::Credential::post_init() {
    if (login_type == 0) {
        if (starts_with(musickey, "W_X")) {
            login_type = 1;
        } else {
            login_type = 2;
        }
    }
}

// check if musicid is provided
bool qqmusic::utils::Credential::has_musicid() const {
    return musicid != 0;
}

// check if musickey is provided
bool qqmusic::utils::Credential::has_musickey() const {
    return !musickey.empty();
}

// raise an error if the credential is invalid
void qqmusic::utils::Credential::raise_for_invalid() const {
    if (!has_musicid()) {
        throw CredentialInvalidError("没有提供 musicid");
    }
    if (!has_musickey()) {
        throw CredentialInvalidError("没有提供 musickey");
    }
}

// refresh function (assumed implementation)
bool qqmusic::utils::Credential::refresh() {
    // simulate refresh logic
    return true;  // return whether it was successful
}

// check if the credential can be refreshed (assumed implementation)
bool qqmusic::utils::Credential::can_refresh() {
    return has_musicid() && has_musickey() && !refresh_key.empty() && !refresh_token.empty();
}

// check if the credential is expired (assumed implementation)
bool qqmusic::utils::Credential::is_expired() {
    return expired_at < time(nullptr);
}

// get the credential as a dictionary
Json::Value qqmusic::utils::Credential::as_dict() const {
    Json::Value root;
    root["openid"] = openid;
    root["refresh_token"] = refresh_token;
    root["access_token"] = access_token;
    root["expired_at"] = expired_at;
    root["musicid"] = musicid;
    root["musickey"] = musickey;
    root["unionid"] = unionid;
    root["str_musicid"] = str_musicid;
    root["refresh_key"] = refresh_key;
    root["encrypt_uin"] = encrypt_uin;
    root["login_type"] = login_type;

    for (const auto& [key, value] : extra_fields) {
        root[key] = value;
    }

    return root;
}

// get the credential as a JSON string
std::string qqmusic::utils::Credential::as_json() const {
    Json::StreamWriterBuilder writer;
    Json::Value data = as_dict();
    return Json::writeString(writer, data);
}

// create credential from cookies
qqmusic::utils::Credential qqmusic::utils::Credential::from_cookies_dict(const std::map<std::string, std::string>& cookies) {
    Credential cred;
    cred.openid = cookies.at("openid");
    cred.refresh_token = cookies.at("refresh_token");
    cred.access_token = cookies.at("access_token");
    cred.expired_at = std::stoll(cookies.at("expired_at"));
    cred.musicid = std::stoi(cookies.at("musicid"));
    cred.musickey = cookies.at("musickey");
    cred.unionid = cookies.at("unionid");
    cred.str_musicid = cookies.at("str_musicid");
    cred.refresh_key = cookies.at("refresh_key");
    cred.encrypt_uin = cookies.at("encrypt_uin");
    cred.login_type = std::stoi(cookies.at("loginType"));
    cred.extra_fields = cookies;
    return cred;
}
