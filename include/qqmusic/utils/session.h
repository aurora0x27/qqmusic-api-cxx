/*------------------------qqmusic/utils/session.h-----------------------------
 * Provide a basic interface to perform http request, include handling cookie 
 * and redirecting
*-----------------------------------------------------------------------------*/
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

class Session {
    friend class SessionManager;
    friend class SessionGuard;

public:
    Session() = delete;

    Session(qqmusic::details::NetworkContext& nc,
            std::shared_ptr<asio::io_context> ioc_ptr,
            std::shared_ptr<asio::ssl::context> ssl_ctx_ptr,
            std::mutex& lock)
        : lock(lock)
        , global_ctx(nc)
        , local_ctx(nc)
        , ioc(std::move(ioc_ptr))
        , ssl_ctx(std::move(ssl_ctx_ptr)) {};

    /*get local context reference*/
    qqmusic::details::NetworkContext& get_context_ref();

    /*write back the change to global context*/
    void sync_global();
    /*update local context by global context*/
    void update_local();

    qqmusic::Task<qqmusic::Result<HttpResponse>> perform_request(
        boost::url_view url, http::request<http::string_body>& req, bool auto_redirecting = true);

private:
    qqmusic::Task<qqmusic::Result<HttpResponse>> handle_http_request(
        boost::url_view url, http::request<http::string_body>& req, bool auto_redirecting);

    qqmusic::Task<qqmusic::Result<HttpResponse>> handle_https_request(
        boost::url_view url, http::request<http::string_body>& req, bool auto_redirecting);

    /*Global context lock*/
    std::mutex& lock;
    qqmusic::details::NetworkContext& global_ctx;

    /*store a copy of local context*/
    qqmusic::details::NetworkContext local_ctx;
    std::shared_ptr<asio::io_context> ioc;
    std::shared_ptr<asio::ssl::context> ssl_ctx;
};

/*
 * Default global session manager
 * Global Singleton
 * provide global shared context and ioc_context
 **/
class SessionManager {
public:
    SessionManager(const SessionManager&) = delete;
    SessionManager operator=(const SessionManager&) = delete;

    Session get_session();

    void set_context(const qqmusic::details::NetworkContext& context);
    static SessionManager& get_instance();

    void push_context(qqmusic::details::NetworkContext&& context);
    void pop_context();

private:
    SessionManager();
    /*lock for global context*/
    std::mutex lock;
    qqmusic::details::NetworkContext ctx;
    std::stack<qqmusic::details::NetworkContext> context_stack;
    std::shared_ptr<asio::io_context> ioc;
    std::shared_ptr<asio::ssl::context> ssl_ctx;
};

/*
 * A `RAII` style session switcher
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
