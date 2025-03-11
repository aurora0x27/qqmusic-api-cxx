/*-------------------------------qqmusic/album.h---------------------------------
 * Api about albums
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_ALBUM_H
#define QQMUSIC_ALBUM_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <string>
#include <string_view>

namespace qqmusic {

enum class cover_size : int { SMALL = 150, DEFAULT = 300, MIDIUM = 500, LARGE = 800 };

std::string get_album_cover_url(std::string_view mid, cover_size size);

/*Get album detailed info from *album mid* */
qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_detail(std::string_view album_mid);

/*Get album detailed info from *album id* */
qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_detail(uint64_t album_id);

qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_songs(std::string_view album_mid,
                                                               unsigned int num = 10,
                                                               unsigned int page = 1);

qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_songs(uint64_t album_id,
                                                               unsigned int num = 10,
                                                               unsigned int page = 1);

} // namespace qqmusic

#endif // !QQMUSIC_ALBUM_H
