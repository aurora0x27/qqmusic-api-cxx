

# File user.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**user.h**](user_8h.md)

[Go to the documentation of this file](user_8h.md)


```C++

#ifndef QQMUSIC_USER_H
#define QQMUSIC_USER_H

#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>
#include <string_view>

namespace qqmusic {

Task<Result<std::string>> get_euin(uint64_t musicid);

Task<Result<uint64_t>> get_musicid(std::string_view euin);

Task<Result<nlohmann::json>> get_homepage(
    std::string_view euin, std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_vip_info(std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_follow_singers(
    std::string_view euin,
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_fans(std::string_view euin,
                                      unsigned page = 1,
                                      unsigned num = 10,
                                      std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_friend_list(
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_follow_usr(
    std::string_view euin,
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_created_songlist(
    uint64_t musicid, std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_fav_song(std::string_view euin,
                                          unsigned page = 1,
                                          unsigned num = 10,
                                          std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_fav_songlist(
    std::string_view euin,
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_fav_album(
    std::string_view euin,
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_fav_mv(std::string_view euin,
                                        unsigned page = 1,
                                        unsigned num = 10,
                                        std::optional<utils::Credential> credential = std::nullopt);

Task<Result<nlohmann::json>> get_music_gene(
    std::string_view euin, std::optional<utils::Credential> credential = std::nullopt);

} // namespace qqmusic

#endif // !QQMUSIC_USER_H
```


