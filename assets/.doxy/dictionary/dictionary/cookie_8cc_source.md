

# File cookie.cc

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**utils**](dir_313caf1132e152dd9b58bea13a4052ca.md) **>** [**cookie.cc**](cookie_8cc.md)

[Go to the documentation of this file](cookie_8cc.md)


```C++
#include "qqmusic/result.h"
#include <format>
#include <nlohmann/json.hpp>
#include <qqmusic/utils/cookie.h>
#include <regex>

namespace qqmusic::utils {

CookieJar::CookieJar(std::string_view cookie_str, std::string_view domain, std::string_view path) {
    auto res = parse_cookie(cookie_str);
    if (res.isErr()) {
        throw std::runtime_error(res.unwrapErr().what());
    }
    auto json = res.unwrap();
    content = nlohmann::json{{domain, {{path, json}}}};
}

qqmusic::Result<void> CookieJar::set(const Cookie& cookie) {
    content[cookie.domain][cookie.path][cookie.key] = cookie.value;
    return Ok();
}

qqmusic::Result<std::string> CookieJar::get(std::string_view key,
                                            std::optional<std::string> domain,
                                            std::optional<std::string> path) {
    try {
        if (domain.has_value()) {
            if (path.has_value()) {
                return Ok(content[domain.value()][path.value()][key].get<std::string>());
            } else {
                /*Not provided with path, search all the paths*/
                for (auto& i : content[domain.value()].items()) {
                    if (i.key() == key) {
                        return Ok(i.value().get<std::string>());
                    }
                }
                return Err(Exception(
                    Exception::JsonError,
                    "[CookieJar::get] -- failed to get cookie value, does not has that key"));
            }
        } else {
            /*Not provided with domain and path, search all the items*/
            for (auto& i : content.items()) {
                for (auto& j : i.value().items()) {
                    for (auto& k : j.value().items()) {
                        if (k.key() == key) {
                            return Ok(k.value().get<std::string>());
                        }
                    }
                }
            }
            return Err(
                Exception(Exception::JsonError,
                          "[CookieJar::get] -- failed to get cookie value, does not has that key"));
        }
    } catch (const std::exception& e) {
        return Err(Exception(Exception::JsonError,
                             std::format("[CookieJar::get] -- failed to get cookie: {}", e.what())));
    }
}

qqmusic::Result<void> CookieJar::del(std::string_view key,
                                     std::optional<std::string> domain,
                                     std::optional<std::string> path) {
    try {
        if (domain.has_value()) {
            if (path.has_value()) {
                if (content.contains(domain) && content[domain.value()].contains(path)
                    && content[domain.value()][path.value()].contains(key)) {
                    content[domain.value()][path.value()].erase(key);
                    return Ok();
                } else {
                    return Err(Exception(
                        Exception::JsonError,
                        std::format("[CookieJar::del] -- failed to delete cookie of key {}: error "
                                    "domain, path or key",
                                    key)));
                }
            } else {
                /*Not provided with path*/
                if (content.contains(domain)) {
                    for (auto& i : content[domain.value()].items()) {
                        if (i.value().contains(key)) {
                            i.value().erase(key);
                            return Ok();
                        }
                    }
                    return Err(Exception(
                        Exception::JsonError,
                        std::format("[CookieJar::del] -- failed to "
                                    "delete cookie of key {}: this domain does not has that key",
                                    key)));
                } else {
                    return Err(Exception(Exception::JsonError,
                                         std::format("[CookieJar::del] -- failed to "
                                                     "delete cookie of key {}: error domain",
                                                     key)));
                }
            }
        } else {
            /*Not provided with domain or path*/
            for (auto& i : content.items()) {
                for (auto& j : i.value().items()) {
                    if (j.value().contains(key)) {
                        j.value().erase(key);
                        return Ok();
                    }
                }
            }
            return Err(Exception(Exception::JsonError,
                                 std::format("[CookieJar::del] -- failed to "
                                             "delete cookie of key {}: does not have that key",
                                             key)));
        }
    } catch (const std::exception& e) {
        return Err(
            Exception(Exception::JsonError,
                      std::format("[CookieJar::del] -- Failed to delete cookie of key {}: {}",
                                  key,
                                  e.what())));
    }
}

qqmusic::Result<std::string> CookieJar::dump() {
    try {
        return Ok(nlohmann::to_string(content));
    } catch (const std::exception& e) {
        return Err(Exception(
            Exception::JsonError,
            std::format("[CookieJar::dump] -- Error occured when dump content to string: {}",
                        e.what())));
    }
}

/*Serialize sellected domain's cookie to cookie string*/
qqmusic::Result<std::string> CookieJar::serialize(std::string_view domain, std::string_view path) {
    /*TODO: convert json to cookie string*/
    std::string res;
    if (!content.contains(domain)) {
        return Err(Exception(Exception::JsonError,
                             std::format("[CookieJar::serialize] -- Does not have that domain: {}",
                                         domain)));
    }

    if (content[domain].contains(path)) {
        for (auto& i : content[domain][path].items()) {
            res += i.key();
            res += "=";
            res += i.value().get<std::string>();
            res += "; ";
        }
    } else {
        for (auto& j : content[domain].items()) {
            for (auto& i : content[domain][j.key()].items()) {
                res += i.key();
                res += "=";
                res += i.value().get<std::string>();
                res += "; ";
            }
        }
    }

    res.erase(res.size() - 2);

    return Ok(res);
}

qqmusic::Result<void> CookieJar::clear(std::optional<std::string> domain,
                                       std::optional<std::string> path,
                                       std::optional<std::string> key) {
    try {
        if (domain.has_value()) {
            if (path.has_value()) {
                if (key.has_value()) {
                    if (content.contains(domain.value())
                        && content[domain.value()].contains(path.value())
                        && content[domain.value()][path.value()].contains(key)) {
                        content[domain.value()][path.value()].erase(key);
                        return Ok();
                    } else {
                        /*Bad domain or path or key*/
                        return Err(Exception(Exception::JsonError,
                                             "[CookieJar::clear] -- Failed to clear "
                                             "cookie items: Bad domain or path or key"));
                    }
                } else {
                    /*Not provided with key, delete the given domain and path's cookie*/
                    if (content.contains(domain.value())
                        && content[domain.value()].contains(path.value())) {
                        content[domain.value()][path.value()].clear();
                        return Ok();
                    } else {
                        /*Bad domain or path*/
                        return Err(Exception(Exception::JsonError,
                                             "[CookieJar::clear] -- Failed to clear "
                                             "cookie items: Bad domain or path"));
                    }
                }
            } else {
                /*Not provided with path and key, delete given domain's cookie*/
                if (content.contains(domain.value())) {
                    content[domain.value()].clear();
                    return Ok();
                } else {
                    /*Bad domain*/
                    return Err(Exception(
                        Exception::JsonError,
                        "[CookieJar::get] -- Failed to get cookie value, does not has that key"));
                }
            }
        } else {
            /*default clear all*/
            content.clear();
            return Ok();
        }
    } catch (const std::exception& e) {
        return Err(
            Exception(Exception::UnknownError,
                      std::format("[CookieJar::clear] -- Cannot clear cookie jar: {}", e.what())));
    }
}

/*Replace cookie jar content with a new Cookie object*/
qqmusic::Result<void> CookieJar::update(const CookieJar& cookies) {
    content = cookies.content;
    return Ok();
}

qqmusic::Result<void> CookieJar::merge(const CookieJar& cookies) {
    try {
        for (auto& i : cookies.content.items()) {
            auto& domain = content[i.key()];
            for (auto& j : i.value().items()) {
                auto& path = domain[j.key()];
                for (auto& k : j.value().items()) {
                    // cover the old value by new ones
                    path[k.key()] = k.value();
                }
            }
        }
        return Ok();
    } catch (const std::exception& e) {
        return Err(
            Exception(Exception::UnknownError,
                      std::format("[CookieJar::merge] -- Cannot merge cookie: {}", e.what())));
    }
}

qqmusic::Result<nlohmann::json> parse_cookie(std::string_view cookie_str) {
    /*
     * Slice cookie into single `key=value` strings, then parse values
     * */
    try {
        nlohmann::json res;
        // std::regex cookie_pattern{R"REGEX((.*?)=(.*?)(?:;(?: |)|$))REGEX"};
        std::regex cookie_pattern{R"REGEX((\s*([^=;]+)(?:=([^;]*))?))REGEX"};
        std::smatch matches;
        std::string raw = std::string(cookie_str.begin(), cookie_str.end());
        while (std::regex_search(raw, matches, cookie_pattern)) {
            auto key = matches[2].str();
            auto val = matches[3].str();
            if (!(key == "Expires" || key == "Secure" || key == "HttpOnly" || key == "SameSite"
                  || val.size() == 0)) {
                res[key] = val;
            }
            raw = matches.suffix().str();
        }
        return Ok(res);
    } catch (const std::exception& e) {
        return Err(Exception(Exception::UnknownError,
                             std::format("[parse_cookie] -- Cannot parse cookie: {}", e.what())));
    }
}

} // namespace qqmusic::utils
```


