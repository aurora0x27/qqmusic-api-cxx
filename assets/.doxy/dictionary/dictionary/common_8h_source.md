

# File common.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**utils**](dir_478616d8952f43e793f28d8ded6e3463.md) **>** [**common.h**](common_8h.md)

[Go to the documentation of this file](common_8h.md)


```C++

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

uint64_t hash33(std::string_view str, uint64_t prev = 0);

std::string get_search_id();

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_COMMON_H
```


