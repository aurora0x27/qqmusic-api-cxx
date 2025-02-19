/*------------------------qqmusic/details/cookie.h------------------------------
 * Provide a cookie manager
 *
 * Use json data structure to store and parse cookie
 * Example:
 * {
 *     "domain1" : {
 *         "path1": {
 *             "key1": "value1",
 *             "key2": "value2"
 *         },
 *         "path2": {
 *             "key1": "value1",
 *             "key2": "value2"
 *         }
 *     },
 *     "domain2" : {
 *         "path1": {
 *             "key1": "value1",
 *             "key2": "value2"
 *         },
 *         "path2": {
 *             "key1": "value1",
 *             "key2": "value2"
 *         }
 *     }
 * }
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETAILS_COOKIE_H
#define QQMUSIC_DETAILS_COOKIE_H

#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <string_view>

namespace qqmusic::details {

struct Cookie {
    std::string domain;
    std::string path = "/"; /*for each site use '/' as default path*/
    std::string key;
    std::string value;
};

/* Parse cookie string to json format
 *
 * key1=value1; key2=value2; key3=value3; key4=value4
 *      |
 *      v
 * {
 *     "key1": "value1",
 *     "key2": "value2",
 *     "key3": "value3",
 *     "key4": "value4",
 * }
 * */
qqmusic::Result<nlohmann::json> parse_cookie(std::string_view cookie_str);

class CookieJar {
public:
    CookieJar()
        : content({"NoDomain", {"/", nlohmann::json::object()}}){};
    /*Construct from cache string *created by CookieJar::dump()* */
    CookieJar(std::string_view cache)
        : content(nlohmann::json::parse(cache)){};
    /*Construct from raw cookie string in http posts*/
    CookieJar(std::string_view cookie_str, std::string_view domain, std::string_view path = "/");

    qqmusic::Result<void> set(const Cookie& cookie);
    qqmusic::Result<std::string> get(std::string_view key,
                                     std::optional<std::string> domain = std::nullopt,
                                     std::optional<std::string> path = std::nullopt);
    qqmusic::Result<void> del(std::string_view key,
                              std::optional<std::string> domain = std::nullopt,
                              std::optional<std::string> path = std::nullopt);

    /* Dump content to cache string
     * you may store cache string in a json file, and reload it to memory later
     * */
    qqmusic::Result<std::string> dump();

    /*Serialize sellected domain's cookie to cookie string*/
    qqmusic::Result<std::string> serialize(std::string_view domain, std::string_view path = "/");

    qqmusic::Result<void> clear(std::optional<std::string> domain = std::nullopt,
                                std::optional<std::string> path = std::nullopt,
                                std::optional<std::string> key = std::nullopt);

    /*Replace cookie jar content with a new Cookie object*/
    qqmusic::Result<void> update(const CookieJar& cookies);

private:
    nlohmann::json content;
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_COOKIE_H
