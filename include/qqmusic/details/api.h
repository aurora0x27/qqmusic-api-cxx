/**------------------------------------qqmusic/details/api.h----------------------------------------
 *
 * @file qqmusic/details/api.h
 *
 * @brief Api类, 简化请求和返回报文解析过程
 *
 * @date 2025-3-21
 *
 *------------------------------------------------------------------------------------------------*/
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

/**
 * @brief 包装请求所需的参数
 * */
struct RequestParam {
    boost::url url;
    http::request<http::string_body> req;
};

/**
 * @brief Api类, 简化请求报文组装和返回报文解析过程
 * */
class Api {
public:
    /**
     * @brief 构造函数
     *
     * @param session session实例引用
     * @param module Api所属模块
     * @param method Api请求方法
     * @param credential 凭证, 默认为空
     * @param common 公共参数
     *
     * @note credential的取舍优先级: `传入的参数 > session(全局默认) > 默认初始化credential`
     * */
    Api(utils::Session& session,
        std::string module,
        std::string method,
        utils::Credential credential = {},
        nlohmann::json common = {{}})
        : session(session)
        , module(std::move(module))
        , method(std::move(method)) {
        if (credential.is_valid()) {
            this->credential = credential;
        } else if (session.get_context_ref().credential.is_valid()) {
            this->credential = session.get_context_ref().credential;
        }
    };

    /**
     * @brief 使用给定的参数组装请求
     *
     * @param params 请求体数据表单
     * @param verify 是否需要合法的Credential进行请求
     *
     * @return 产生`RequestParam`打包的请求参数
     * */
    qqmusic::Task<qqmusic::Result<RequestParam>> prepare_request(const nlohmann::json& params,
                                                                 bool verify = false);

    /**
     * @brief 将返回报文解析成Json结果, 必须是由Api类产生的请求产生的结果
     * */
    qqmusic::Result<nlohmann::json> parse_response(utils::buffer&& response);

private:
    utils::Session& session;
    std::string module;
    std::string method;
    nlohmann::json common;
    utils::Credential credential;
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETIALS_API_H
