/*----------------------qqmusic/utils/qrc-decoder.h-----------------------------
 * qrc-decoder provides a function to decode *.qrc
 * lyrics
*-----------------------------------------------------------------------------*/
#ifndef QRC_DECODER_H
#define QRC_DECODER_H

#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>

namespace qqmusic::utils {

enum class qrc_type { cloud, local };

qqmusic::Result<std::string> qrc_decode(const qqmusic::utils::buffer& src,
                                        qqmusic::utils::qrc_type type);
} // namespace qqmusic::utils

#endif
