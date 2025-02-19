#include <format>
#include <nlohmann/json.hpp>
#include <qqmusic/details/cookie.h>
#include <regex>

qqmusic::details::CookieJar::CookieJar(std::string_view cookie_str,
                                       std::string_view domain,
                                       std::string_view path) {
    auto res = qqmusic::details::parse_cookie(cookie_str);
    if (res.isErr()) {
        throw std::runtime_error(res.unwrapErr().what());
    }
    auto json = res.unwrap();
    content = nlohmann::json{{domain, {{path, json}}}};
}

qqmusic::Result<void> qqmusic::details::CookieJar::set(const qqmusic::details::Cookie& cookie) {
    content[cookie.domain][cookie.path][cookie.key] = cookie.value;
    return Ok();
}

qqmusic::Result<std::string> qqmusic::details::CookieJar::get(std::string_view key,
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
                return Err(qqmusic::utils::Exception(
                    qqmusic::utils::Exception::JsonError,
                    "[CookieJar::get] -- failed to get cookie value, does not has that key"));
            }
        } else {
            /*Not provided with domain and path, search all the items*/
            for (auto& i : content.items()) {
                for (auto& j : i.value().items()) {
                    if (i.key() == key) {
                        return Ok(i.value().get<std::string>());
                    }
                }
            }
            return Err(qqmusic::utils::Exception(
                qqmusic::utils::Exception::JsonError,
                "[CookieJar::get] -- failed to get cookie value, does not has that key"));
        }
    } catch (const std::exception& e) {
        return Err(
            qqmusic::utils::Exception(qqmusic::utils::Exception::JsonError,
                                      std::format("[CookieJar::get] -- failed to get cookie: {}",
                                                  e.what())));
    }
}

qqmusic::Result<void> qqmusic::details::CookieJar::del(std::string_view key,
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
                    return Err(qqmusic::utils::Exception(
                        qqmusic::utils::Exception::JsonError,
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
                    return Err(qqmusic::utils::Exception(
                        qqmusic::utils::Exception::JsonError,
                        std::format("[CookieJar::del] -- failed to "
                                    "delete cookie of key {}: this domain does not has that key",
                                    key)));
                } else {
                    return Err(qqmusic::utils::Exception(
                        qqmusic::utils::Exception::JsonError,
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
            return Err(qqmusic::utils::Exception(
                qqmusic::utils::Exception::JsonError,
                std::format("[CookieJar::del] -- failed to "
                            "delete cookie of key {}: does not have that key",
                            key)));
        }
    } catch (const std::exception& e) {
        return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::JsonError,
            std::format("[CookieJar::del] -- Failed to delete cookie of key {}: {}", key, e.what())));
    }
}

qqmusic::Result<std::string> qqmusic::details::CookieJar::dump() {
    try {
        return Ok(nlohmann::to_string(content));
    } catch (const std::exception& e) {
        return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::JsonError,
            std::format("[CookieJar::dump] -- Error occured when dump content to string: {}",
                        e.what())));
    }
}

/*Serialize sellected domain's cookie to cookie string*/
qqmusic::Result<std::string> qqmusic::details::CookieJar::serialize(std::string_view domain,
                                                                    std::string_view path) {
    /*TODO: convert json to cookie string*/
    std::string res;
    if (!content.contains(domain)) {
        return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::JsonError,
            std::format("[CookieJar::serialize] -- Does not have that domain: {}", domain)));
    }

    if (!content[domain].contains(path)) {
        return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::JsonError,
            std::format("[CookieJar::serialize] -- Does not have that path: {}", path)));
    }

    for (auto& i : content[domain][path].items()) {
        res += i.key();
        res += "+";
        res += i.value().get<std::string>();
        res += "; ";
    }

    res.erase(res.size() - 2);

    return Ok(res);
}

qqmusic::Result<void> qqmusic::details::CookieJar::clear(std::optional<std::string> domain,
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
                        return Err(
                            qqmusic::utils::Exception(qqmusic::utils::Exception::JsonError,
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
                        return Err(
                            qqmusic::utils::Exception(qqmusic::utils::Exception::JsonError,
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
                    return Err(qqmusic::utils::Exception(
                        qqmusic::utils::Exception::JsonError,
                        "[CookieJar::get] -- Failed to get cookie value, does not has that key"));
                }
            }
        } else {
            /*default clear all*/
            content.clear();
            return Ok();
        }
    } catch (const std::exception& e) {
        return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::UnknownError,
            std::format("[CookieJar::clear] -- Cannot clear cookie jar: {}", e.what())));
    }
}

/*Replace cookie jar content with a new Cookie object*/
qqmusic::Result<void> qqmusic::details::CookieJar::update(
    const qqmusic::details::CookieJar& cookies) {
    content = cookies.content;
    return Ok();
}

qqmusic::Result<nlohmann::json> qqmusic::details::parse_cookie(std::string_view cookie_str) {
    /*
     * Slice cookie into single `key=value` strings, then parse values
     * */
    try {
        nlohmann::json res;
        std::regex cookie_pattern("(.*?)=(.*?)(?:;(?: |)|$)");
        std::smatch matches;
        std::string raw = std::string(cookie_str.begin(), cookie_str.end());
        while (std::regex_search(raw, matches, cookie_pattern)) {
            if (matches.size() != 3) {
                /*assert match size is 3*/
                return Err(qqmusic::utils::Exception(
                    qqmusic::utils::Exception::DataDestroy,
                    std::format("[parse_cookie] -- Error occured when parsing cookie: assert match "
                                "size is 3, but {} has {}",
                                matches.str(),
                                matches.size())));
            }
            res[matches[1].str()] = matches[2].str();
            raw = matches.suffix().str();
        }
        return Ok(res);
    } catch (const std::exception& e) {
        return Err(
            qqmusic::utils::Exception(qqmusic::utils::Exception::UnknownError,
                                      std::format("[parse_cookie] -- Cannot parse cookie: {}",
                                                  e.what())));
    }
}
