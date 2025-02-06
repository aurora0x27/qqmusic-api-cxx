#include <format>
#include <nlohmann/detail/conversions/from_json.hpp>
#include <nlohmann/detail/conversions/to_json.hpp>
#include <qqmusic/utils/credential.h>
#include <string>
#include <string_view>

/* NOTE: Assume that only loginType and musickey field can be defaulted*/
qqmusic::utils::Credential::Credential(const Json& cookie)
    : qqmusic::utils::Credential() {
    nlohmann::from_json(cookie, *this);
    if (cookie.contains("musickey")) {
        inner.musickey = cookie["musickey"].get<std::string>();

        if (!cookie.contains("loginType") && inner.musickey.substr(0, 3) == "W_X") {
            inner.loginType = 1;
        } else {
            inner.loginType = 2;
        }
    }
}

qqmusic::utils::Credential::Credential(std::string_view cookie)
    : qqmusic::utils::Credential(nlohmann::json::parse(cookie)) {}

bool qqmusic::utils::Credential::is_valid() const {
    if (inner.musickey.size() == 0 || inner.musicid == 0) {
        return true;
    }
    return false;
}

qqmusic::Result<nlohmann::json> qqmusic::utils::Credential::as_json() {
    try {
        Json res;
        nlohmann::to_json(res, *this);
        return Ok(res);
    } catch (const std::exception& e) {
        return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::DataDestroy,
            std::format("[Credential::as_json] -- failed to generate json: {}", e.what())));
    }
}

qqmusic::Result<std::string> qqmusic::utils::Credential::as_string() {
    try {
        Json res;
        nlohmann::to_json(res, *this);
        return Ok(res.dump());
    } catch (const std::exception& e) {
        return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::DataDestroy,
            std::format("[Credential::as_string] -- failed to generate json string: {}", e.what())));
    }
}

qqmusic::Task<qqmusic::Result<bool>> qqmusic::utils::Credential::is_refreshable() {
    if (inner.musicid == 0 || inner.musickey == "") {
        co_return Ok(false);
    }
    /* TODO: check is expired by login api*/
    co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::UnknownError,
                                            "[is_refreshable] -- login api is not implemeted..."));
    co_return Ok(true);
}
