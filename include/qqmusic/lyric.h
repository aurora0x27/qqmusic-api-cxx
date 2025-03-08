/*----------------------------qqmusic/lyric.h-----------------------------------
 * Lyric utils -- get and decrypt lyrics 
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_LYRIC_H
#define QQMUSIC_LYRIC_H

#include <qqmusic/result.h>

namespace qqmusic {

struct Lyric {
    std::string lyric;
    std::string trans;
    std::string roma;
};

Task<Result<Lyric>> get_lyric(std::string_view mid,
                              bool qrc = false,
                              bool trans = false,
                              bool roma = false);

Task<Result<Lyric>> get_lyric(uint64_t id, bool qrc = false, bool trans = false, bool roma = false);

} // namespace qqmusic

#endif // !QQMUSIC_LYRIC_H
