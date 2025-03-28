

# File credential.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**utils**](dir_478616d8952f43e793f28d8ded6e3463.md) **>** [**credential.h**](credential_8h.md)

[Go to the documentation of this file](credential_8h.md)


```C++

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

    explicit Credential(std::string_view cookie);

    explicit Credential(nlohmann::json& cookie);

    bool from_cache(std::string_view cache);

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
```


