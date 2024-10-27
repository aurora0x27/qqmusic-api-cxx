//-------------utils/qrcdecoder.h---------------
//       decode qrc lyric files
#ifndef QRC_DECODER_H
#define QRC_DECODER_H
#include <qqmusic/error.h>
#include <utils/buffer.h>

namespace qqmusic::utils
{

    enum class qrc_type
    {
        cloud,
        local
    };

    qqmusic::result qrc_decode(qqmusic::utils::buffer*  src,
                               qqmusic::utils::buffer*  dest,
                               qqmusic::utils::qrc_type type);
}
// namespace qqmusic::utils

#endif
