/*------------------------qqmusic/details/common.h------------------------------
 * qqmusic api common tools
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETAILS_COMMON_H
#define QQMUSIC_DETAILS_COMMON_H

#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>

namespace qqmusic::details {

std::string sign(const nlohmann::json& params);

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_COMMON_H
