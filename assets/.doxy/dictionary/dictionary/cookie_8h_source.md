

# File cookie.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**utils**](dir_478616d8952f43e793f28d8ded6e3463.md) **>** [**cookie.h**](cookie_8h.md)

[Go to the documentation of this file](cookie_8h.md)


```C++

#ifndef QQMUSIC_UTILS_COOKIE_H
#define QQMUSIC_UTILS_COOKIE_H

#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <string_view>

namespace qqmusic::utils {

struct Cookie {
    std::string domain;

    std::string path = "/"; /*for each site use '/' as default path*/

    std::string key;

    std::string value;
};

qqmusic::Result<nlohmann::json> parse_cookie(std::string_view cookie_str);

class CookieJar {
public:

    CookieJar()
        : content({{"", {{"/", nlohmann::json::object()}}}}) {};

    CookieJar(std::string_view cache)
        : content(nlohmann::json::parse(cache)) {};

    CookieJar(std::string_view cookie_str, std::string_view domain, std::string_view path = "/");

    qqmusic::Result<void> set(const Cookie& cookie);

    qqmusic::Result<std::string> get(std::string_view key,
                                     std::optional<std::string> domain = std::nullopt,
                                     std::optional<std::string> path = std::nullopt);

    qqmusic::Result<void> del(std::string_view key,
                              std::optional<std::string> domain = std::nullopt,
                              std::optional<std::string> path = std::nullopt);

    qqmusic::Result<std::string> dump();

    qqmusic::Result<std::string> serialize(std::string_view domain, std::string_view path = "/");


    qqmusic::Result<void> clear(std::optional<std::string> domain = std::nullopt,
                                std::optional<std::string> path = std::nullopt,
                                std::optional<std::string> key = std::nullopt);

    qqmusic::Result<void> update(const CookieJar& cookies);

    qqmusic::Result<void> merge(const CookieJar& cookies);

private:
    nlohmann::json content;
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_COOKIE_H
```


