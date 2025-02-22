/*--------------------------qqmusic/utils/common.h------------------------------
 * qqmusic api common tools
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_COMMON_H
#define QQMUSIC_UTILS_COMMON_H

#include <boost/beast.hpp>
#include <boost/beast/http/message.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>

namespace qqmusic::utils {

namespace http = boost::beast::http;

std::string sign(const nlohmann::json& params);

buffer to_buffer(http::response<http::dynamic_body>& resp);

enum class qrc_type { cloud, local };

qqmusic::Result<std::string> qrc_decode(const qqmusic::utils::buffer& src,
                                        qqmusic::utils::qrc_type type);

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_COMMON_H
