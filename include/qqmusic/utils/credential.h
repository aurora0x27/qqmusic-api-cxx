/*-------------utils/credential.h---------------
 * credential used for identification
*---------------------------------------------*/
#ifndef QQMUSIC_UTILS_CREDENTIAL_H
#define QQMUSIC_UTILS_CREDENTIAL_H

#include <cstdint>
#include <map>
#include <string>
#include <variant>

namespace qqmusic::utils {

class Credential {
public:
    Credential() = default;
    Credential(std::string_view cookie);
    bool is_valid();
    bool is_refreshable();
    std::string as_json();

private:
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
        int login_type;
        std::map<std::string, std::variant<uint64_t, std::string>> extra_fields;
    } inner;
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_CREDENTIAL_H
