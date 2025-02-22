/*---------------------------qqmusic/details/api.h------------------------------
 * Api class prepare a request with provided infomation and global context
 *
 * private possessed by each api:
 *     - method
 *     - module
 *
 * globally shared by all apis:
 *     - credential
 *     - common
 *
 * * NOTE: 笔记 -- 调试完应删除*
 *   Api函数内部调用各个部分, 完成请求和结果处理, 统一返回json结果
 *   ** 所有api函数最后隐含参数std::optional<NetworkClient>, 默认使用用全局
 *   上下文初始化的全局单例 NetworkClient
 *   1. 根据mehod和module建立Api实例
 *   2. prepare_request函数设置api请求的全局上下文无关部分, 如url, 请求体
 *   3. NetworkClient.perform_request(req), 发送请求获得响应, 期间会添加
 *   全局上下文相关信息, 如cookie
 *   *
 *   NetworkClient根据全局上下文SessionContext初始化, SessionContext提供序列化
 *   导出的方法, 提供缓存
 *
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETIALS_API_H
#define QQMUSIC_DETIALS_API_H

#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/url.hpp>
#include <boost/url/param.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/details/context.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/credential.h>
#include <qqmusic/utils/session.h>

namespace qqmusic::details {

namespace http = boost::beast::http;
namespace asio = boost::asio;

class Api {
public:
    Api(utils::Session& session,
        std::string module,
        std::string method,
        utils::Credential credential = {},
        nlohmann::json common = {{}})
        : session(session)
        , module(std::move(module))
        , method(std::move(method)){};

    /*prepare request by given infomation*/
    qqmusic::Task<qqmusic::Result<http::request<http::string_body>>> prepare_request(
        nlohmann::json& params);

    /*parse result buffer to json format*/
    qqmusic::Result<nlohmann::json> parse_response(utils::HttpResponse& resp);

private:
    utils::Session& session;
    std::string module;
    std::string method;
    nlohmann::json common;
    utils::Credential credential;
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETIALS_API_H
