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

    /* construct from cookie string */
    explicit Credential(std::string_view cookie);

    /* construct from cookie string */
    explicit Credential(nlohmann::json& cookie);

    /* construct from cached string */
    bool from_cache(std::string_view cache);

    /* construct from cached json */
    bool from_cache(nlohmann::json& cache);

    bool is_valid() const;
    qqmusic::Task<qqmusic::Result<bool>> is_expired();
    qqmusic::Task<qqmusic::Result<void>> refresh();
    qqmusic::Result<std::string> to_string();
    qqmusic::Result<nlohmann::json> to_json();

    std::string openid;
    std::string refresh_token;
    std::string access_token;
    uint64_t expired_at = 0;
    uint64_t musicid = 0;
    std::string musickey;
    std::string unionid;
    std::string str_musicid;
    std::string refresh_key;
    std::string encryptUin;
    int loginType = 2;
    nlohmann::json extra_fields;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Credential,
                                   openid,
                                   refresh_token,
                                   access_token,
                                   expired_at,
                                   musicid,
                                   unionid,
                                   str_musicid,
                                   musickey,
                                   refresh_key,
                                   encryptUin,
                                   loginType);
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_CREDENTIAL_H
