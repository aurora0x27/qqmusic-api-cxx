

# File album.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**album.h**](album_8h.md)

[Go to the documentation of this file](album_8h.md)


```C++

#ifndef QQMUSIC_ALBUM_H
#define QQMUSIC_ALBUM_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <string>
#include <string_view>

namespace qqmusic {

enum class CoverSize : int { SMALL = 150, DEFAULT = 300, MIDIUM = 500, LARGE = 800 };

std::string get_album_cover_url(std::string_view mid, CoverSize size);

qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_detail(std::string_view album_mid);

qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_detail(uint64_t album_id);

qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_songs(std::string_view album_mid,
                                                               unsigned int num = 10,
                                                               unsigned int page = 1);

qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_songs(uint64_t album_id,
                                                               unsigned int num = 10,
                                                               unsigned int page = 1);

} // namespace qqmusic

#endif // !QQMUSIC_ALBUM_H
```


