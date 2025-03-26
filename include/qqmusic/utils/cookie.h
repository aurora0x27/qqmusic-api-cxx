/**---------------------------------qqmusic/details/cookie.h----------------------------------------
 * 
 * @brief 简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖`nlohmann::json`实现
 *        目前不支持路径嵌套和`Expires`和`HttpOnly`这样的特性
 *
 * @date 2025-3-21
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_COOKIE_H
#define QQMUSIC_UTILS_COOKIE_H

#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <string_view>

namespace qqmusic::utils {

/**
 * @brief 代表一条Cookie
 * */
struct Cookie {
    /**
     * @brief 域名
     * */
    std::string domain;

    /**
     * @brief 路径
     * */
    std::string path = "/"; /*for each site use '/' as default path*/

    /**
     * @brief 键
     * */
    std::string key;

    /**
     * @brief 值
     * */
    std::string value;
};

/**
 * @brief 将Cookie字符串解析成Json对象
 *
 * @code{text}
 * key1=value1; key2=value2; key3=value3; key4=value4
 *      |
 *      v
 * {
 *     "key1": "value1",
 *     "key2": "value2",
 *     "key3": "value3",
 *     "key4": "value4",
 * }
 * @endcode
 * */
qqmusic::Result<nlohmann::json> parse_cookie(std::string_view cookie_str);

/**
 * @brief 简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖`nlohmann::json`实现
 *        目前不支持路径嵌套和`Expires`和`HttpOnly`这样的特性
 *
 * 使用Json数据结构来储存Cookie
 * @code{json}
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
 * @endcode
 *
 * @warning 不支持path的嵌套, `example.com/menu` 和 `example.com/menu/usr` 
 *          被认为是域名`example.com`下两个完全不同的路径.
 *
 *          不支持解析`HttpOnly`, `Expires`这样的字段, 不是键值对形式的
 *          Cookie条目会被丢弃.
 *
 * @todo 需要支持嵌套的路径, 支持非键值对形式的Cookie
 * */
class CookieJar {
public:

    /**
     * @brief 空初始化
     * */
    CookieJar()
        : content({{"", {{"/", nlohmann::json::object()}}}}) {};

    /**
     * @brief 缓存反序列化, 从`dump`函数产生的缓存构造
     *
     * @param cache 缓存
     * */
    CookieJar(std::string_view cache)
        : content(nlohmann::json::parse(cache)) {};

    /**
     * @brief 从原始cookie字符串解析
     *
     * @param cookie_str 请求报文中cookie字符串, 一般存在于`Set-Cookie`字段
     * @param domain 域名
     * @param path 路径, 默认是`/`
     * */
    CookieJar(std::string_view cookie_str, std::string_view domain, std::string_view path = "/");

    /**
     * @brief 插入一条Cookie
     *
     * @param cookie 一个Cookie条目
     *
     * @note 如果对应域名和路径的这个Cookie的键已经存在, 那么会覆盖老的值
     * */
    qqmusic::Result<void> set(const Cookie& cookie);

    /**
     * @brief 查询对应键的Cookie字段的值
     *
     * @param key 键
     * @param domain 域名, 默认`std::nullopt`, 若不给出则全局寻找
     * @param path 路径, 默认`std::nullopt`, 若不给出则在域名下寻找
     *
     * @return `Result<std::string>`, 键值对的值是字符串形式的
     *          找不到会返回`JsonError`
     *
     * @note 参数缺省时总是返回第一个找到的键值对的值
     * */
    qqmusic::Result<std::string> get(std::string_view key,
                                     std::optional<std::string> domain = std::nullopt,
                                     std::optional<std::string> path = std::nullopt);

    /**
     * @brief 删除对应键的Cookie字段和他的值
     *
     * @param key 键
     * @param domain 域名, 默认`std::nullopt`, 若不给出则全局寻找
     * @param path 路径, 默认`std::nullopt`, 若不给出则在域名下寻找
     *
     * @return `Result<void>`, 如果没有对应的键, 会返回`JsonError`
     *
     * @note 缺省参数时总是删除第一个找到符合的键值对
     * */
    qqmusic::Result<void> del(std::string_view key,
                              std::optional<std::string> domain = std::nullopt,
                              std::optional<std::string> path = std::nullopt);

    /**
     * @brief 序列化Cookie
     *
     * @return 已经序列化的json字符串, 如果出错会返回`JsonError`
     * */
    qqmusic::Result<std::string> dump();

    /**
     * @brief 将对应域名和路径的Cookie序列化成请求报文
     *
     * @param domain 域名
     * @param path 路径, 默认`/`
     * */
    qqmusic::Result<std::string> serialize(std::string_view domain, std::string_view path = "/");


    /**
     * @brief 删除对应区域的cookie
     * */
    qqmusic::Result<void> clear(std::optional<std::string> domain = std::nullopt,
                                std::optional<std::string> path = std::nullopt,
                                std::optional<std::string> key = std::nullopt);

    /**
     * @brief 替换CookieJar的内容
     *
     * @param cookies 新的Cookies
     * */
    qqmusic::Result<void> update(const CookieJar& cookies);

    /**
     * @brief 合并CookieJar内容
     *
     * @param cookies 待合并cookie
     *
     * @note 原本Cookie不存在的字段被创建, 重合的字段被新值覆盖
     * */
    qqmusic::Result<void> merge(const CookieJar& cookies);

private:
    nlohmann::json content;
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_COOKIE_H
