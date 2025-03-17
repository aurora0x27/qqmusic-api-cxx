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

buffer resp2buf(http::response<http::dynamic_body>&& resp);

buffer hex2buf(std::string_view hex);

enum class qrc_type { cloud, local };

qqmusic::Result<std::string> qrc_decode(const qqmusic::utils::buffer& src,
                                        qqmusic::utils::qrc_type type);

/*get hash value of utf-8 string*/
uint64_t hash33(std::string_view str, uint64_t prev = 0);

std::string get_search_id();

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_COMMON_H
