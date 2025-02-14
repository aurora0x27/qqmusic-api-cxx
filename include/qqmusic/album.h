/*----------------------------------album.h-------------------------------------
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

struct AlbumEntity {
    std::string mid; /*field: albummid*/
    uint64_t id;
};

qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_detail();

// get_song(): get std::vector<song>

} // namespace qqmusic

#endif // !QQMUSIC_ALBUM_H
