/*-----------------------------utils/credential.h-------------------------------
 * Credential used for identification
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_CREDENTIAL_H
#define QQMUSIC_UTILS_CREDENTIAL_H

#include <cstdint>
#include <nlohmann/detail/macro_scope.hpp>
#include <qqmusic/result.h>
#include <string>

namespace qqmusic::utils {

class Credential {
public:
    Credential() = default;
    Credential(std::string_view cookie);
    Credential(const Json& cookie);
    bool is_valid() const;
    qqmusic::Task<qqmusic::Result<bool>> is_refreshable();
    qqmusic::Task<qqmusic::Result<bool>> refresh();
    qqmusic::Result<std::string> as_string();
    qqmusic::Result<Json> as_json();

    struct {
        std::string openid;
        std::string refresh_token;
        std::string access_token;
        uint64_t expired_at;
        uint64_t musicid;
        std::string musickey;
        std::string unionid;
        std::string musicid_s;
        std::string refresh_key;
        std::string encrypt_uin;
        int loginType;
        Json extra_fields;
    } inner;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Credential,
                                   inner.openid,
                                   inner.refresh_token,
                                   inner.access_token,
                                   inner.expired_at,
                                   inner.musicid,
                                   inner.unionid,
                                   inner.musicid_s,
                                   inner.refresh_key,
                                   inner.encrypt_uin,
                                   inner.extra_fields);
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_CREDENTIAL_H
