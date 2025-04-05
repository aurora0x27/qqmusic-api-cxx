

# File songlist.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**songlist.h**](songlist_8h.md)

[Go to the documentation of this file](songlist_8h.md)


```C++

#ifndef QQMUSIC_SONG_LIST_H
#define QQMUSIC_SONG_LIST_H

#include <cstdint>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic {

Task<Result<nlohmann::json>> get_songlist_detail(uint64_t dissid,
                                                 unsigned dirid = 0,
                                                 unsigned num = 10,
                                                 unsigned page = 1,
                                                 bool only_song = false,
                                                 bool tag = true,
                                                 bool user_info = true);

Task<Result<nlohmann::json>> get_songlist(uint64_t dissid, unsigned dirid = 0);

Task<Result<nlohmann::json>> songlist_create(
    std::string_view dirname, std::optional<utils::Credential> credential = std::nullopt);

Task<Result<bool>> songlist_delete(uint64_t dirid,
                                   std::optional<utils::Credential> credential = std::nullopt);

Task<Result<bool>> songlist_add_songs(uint64_t dirid,
                                      std::span<uint64_t> song_ids,
                                      std::optional<utils::Credential> credential = std::nullopt);

Task<Result<bool>> songlist_delete_songs(uint64_t dirid,
                                         std::span<uint64_t> song_ids,
                                         std::optional<utils::Credential> credential = std::nullopt);
} // namespace qqmusic

#endif // !QQMUSIC_SONG_LIST_H
```


