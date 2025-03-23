/*----------------------------------qqmusic/utils/session.h-----------------------------------------
 *
 * @ file qqmusic/utils/session.h
 *
 * @brief 处理网络请求, 以及管理相关的上下文
 *
 * @date 2025-3-23
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_SESSION_H
#define QQMUSIC_UTILS_SESSION_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/url.hpp>
#include <memory>
#include <qqmusic/details/context.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>
#include <qqmusic/utils/device.h>
#include <qqmusic/utils/qimei.h>
#include <stack>
#include <utility>

namespace qqmusic::utils {

namespace http = boost::beast::http;
namespace asio = boost::asio;
using HttpResponse = http::response<http::dynamic_body>;

class SessionManager;
class SessionGuard;

/**
 * @brief 上下文
 * */
class Session {
    friend class SessionManager;
    friend class SessionGuard;

public:
    Session() = delete;

    /**
     * @brief 构造函数, 正常情况下不应该使用
     * */
    Session(qqmusic::details::NetworkContext& nc,
            std::shared_ptr<asio::io_context> ioc_ptr,
            std::shared_ptr<asio::ssl::context> ssl_ctx_ptr,
            std::mutex& lock)
        : lock(lock)
        , global_ctx(nc)
        , local_ctx(nc)
        , ioc(std::move(ioc_ptr))
        , ssl_ctx(std::move(ssl_ctx_ptr)) {};

    /**
     * @brief 获取`session`实例的上下文引用
     * */
    qqmusic::details::NetworkContext& get_context_ref();

    /**
     * @brief 将上下文的更改写回全局
     * */
    void sync_global();

    /**
     * @brief 将全局的更改同步到本实例
     * */
    void update_local();

    /**
     * @brief 发送网络请求, 返回回复报文
     *
     * @param url 请求url
     * @param req 请求对象
     * @param auto_redirecting 是否开启自动重定向, 默认`true`
     *
     * @return `qqmusic::Task<qqmusic::Result<HttpResponse>>`, 返回原始报文
     *
     * @note 异步函数, 返回`Task`
     * */
    qqmusic::Task<qqmusic::Result<HttpResponse>> perform_request(
        boost::url_view url, http::request<http::string_body>& req, bool auto_redirecting = true);

private:
    /**
     * @brief 处理http请求, 使用普通的`tcp_stream`
     * */
    qqmusic::Task<qqmusic::Result<HttpResponse>> handle_http_request(
        boost::url_view url, http::request<http::string_body>& req, bool auto_redirecting);

    /**
     * @brief 处理https请求, 使用`ssl_stream`
     * */
    qqmusic::Task<qqmusic::Result<HttpResponse>> handle_https_request(
        boost::url_view url, http::request<http::string_body>& req, bool auto_redirecting);

    /**
     * @brief 对全局上下文负责的互斥锁引用
     * */
    std::mutex& lock;

    /**
     * @brief 全局上下文引用
     * */
    qqmusic::details::NetworkContext& global_ctx;

    /**
     * @brief 实例局部的上下文引用, 只允许用户直接对这份拷贝进行修改.
     * */
    qqmusic::details::NetworkContext local_ctx;

    std::shared_ptr<asio::io_context> ioc;
    std::shared_ptr<asio::ssl::context> ssl_ctx;
};

/**
 * @brief 全局单例的Session管理器, 负责维护全局上下文栈, 并负责`session`实例的生成
 * */
class SessionManager {
public:
    SessionManager(const SessionManager&) = delete;
    SessionManager operator=(const SessionManager&) = delete;

    /**
     * @brief 获取一个`session`实例
     * */
    Session get_session();

    /**
     * @brief 替换上下文栈栈顶的上下文
     * */
    void set_context(const qqmusic::details::NetworkContext& context);

    /**
     * @brief 获取Session管理器实例
     * */
    static SessionManager& get_instance();

    /**
     * @brief 将指定的上下文压栈
     * */
    void push_context(qqmusic::details::NetworkContext&& context);

    /**
     * @brief 上下文栈弹出栈顶元素
     * */
    void pop_context();

private:
    SessionManager();
    /**
     * @brief 对全局上下文负责的互斥锁
     * */
    std::mutex lock;

    /**
     * @brief 全局上下文实例, 即上下文栈的栈顶
     * */
    qqmusic::details::NetworkContext ctx;

    /**
     * @brief 全局上下文栈
     * */
    std::stack<qqmusic::details::NetworkContext> context_stack;

    std::shared_ptr<asio::io_context> ioc;
    std::shared_ptr<asio::ssl::context> ssl_ctx;
};

/**
 * @brief 一个`RAII`风格的Session切换器
 * */
class SessionGuard {
public:
    SessionGuard(SessionManager& sm, Session s)
        : session_manager(sm) {
        session_manager.push_context(std::move(s.local_ctx));
    };
    SessionGuard(SessionManager& sm, details::NetworkContext ctx)
        : session_manager(sm) {
        session_manager.push_context(std::move(ctx));
    };
    ~SessionGuard() { session_manager.pop_context(); };

private:
    SessionManager& session_manager;
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_SESSION_H
