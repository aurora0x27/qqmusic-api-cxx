#include <format>
#include <qqmusic/album.h>
#include <string>

std::string qqmusic::get_album_cover_url(std::string_view mid, qqmusic::cover_size size) {
    return std::format("https://y.gtimg.cn/music/photo_new/T002R{}x{}M000{}.jpg",
                       (int) size,
                       (int) size,
                       mid);
}
