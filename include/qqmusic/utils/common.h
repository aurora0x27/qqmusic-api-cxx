/*--------------------------qqmusic/utils/common.h------------------------------
 * qqmusic api common tools
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_COMMON_H
#define QQMUSIC_UTILS_COMMON_H

#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>

namespace qqmusic::utils {

std::string sign(const nlohmann::json& params);

enum class qrc_type { cloud, local };

qqmusic::Result<std::string> qrc_decode(const qqmusic::utils::buffer& src,
                                        qqmusic::utils::qrc_type type);

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_COMMON_H
