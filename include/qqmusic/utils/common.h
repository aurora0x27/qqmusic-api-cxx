/**----------------------------------qqmusic/utils/common.h-----------------------------------------
 *
 * @ brief 工具函数
 *
 * @ date 2025-3-21
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_COMMON_H
#define QQMUSIC_UTILS_COMMON_H

#include <boost/beast.hpp>
#include <boost/beast/http/message.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>

namespace qqmusic::utils {

namespace http = boost::beast::http;

/**
 * @brief QQ音乐请求签名
 *
 * @param params 请求数据表单
 *
 * @return 签名结果
 * */
std::string sign(const nlohmann::json& params);

/**
 * @brief 将请求回复报文体转换成二进制buffer
 *
 * @param resp 回复报文
 *
 * @return qqmusic::utils::buffer类型buffer
 * */
buffer resp2buf(http::response<http::dynamic_body>&& resp);

/**
 * @brief 十六进制字符串转成buffer
 *
 * @param hex 十六进制编码字符串
 *
 * @return qqmusic::utils::buffer类型buffer
 * */
buffer hex2buf(std::string_view hex);

/**
 * @brief QRC歌词种类
 * */
enum class qrc_type { cloud, local };

/**
 * @brief 解码加密的QRC歌词
 *
 * @param src 被加密的歌词
 * @param type 歌词类型
 *
 * @return 解码后的字符串, 如果出错会返回`Exception`
 * */
qqmusic::Result<std::string> qrc_decode(const qqmusic::utils::buffer& src,
                                        qqmusic::utils::qrc_type type);

/**
 * @brief 计算utf-8字符串的哈希值
 *
 * @param str 待求值字符串
 * @param prev 多次求值时传入上次求出的结果, 默认为`0`
 *
 * @return 64位整数格式的哈希
 * */
uint64_t hash33(std::string_view str, uint64_t prev = 0);

/**
 * @brief 返回随机搜索ID
 * */
std::string get_search_id();

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_COMMON_H
