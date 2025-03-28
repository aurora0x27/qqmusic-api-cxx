

# File session.cc

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**utils**](dir_313caf1132e152dd9b58bea13a4052ca.md) **>** [**session.cc**](session_8cc.md)

[Go to the documentation of this file](session_8cc.md)


```C++
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/stream_traits.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/url.hpp>
#include <memory>
#include <mutex>
#include <qqmusic/result.h>
#include <qqmusic/utils/cookie.h>
#include <qqmusic/utils/session.h>
#include <utility>

static qqmusic::Task<qqmusic::Result<qqmusic::utils::HttpResponse>> handle_http_redirecting(
    qqmusic::utils::Session& self,
    boost::url_view url,
    boost::beast::http::request<boost::beast::http::string_body>& req,
    qqmusic::utils::HttpResponse& resp);

static qqmusic::Task<qqmusic::Result<qqmusic::utils::HttpResponse>> handle_https_redirecting(
    qqmusic::utils::Session& self,
    boost::url_view url,
    boost::beast::http::request<boost::beast::http::string_body>& req,
    qqmusic::utils::HttpResponse& resp,
    std::shared_ptr<boost::asio::ssl::context> ssl_ctx);

namespace qqmusic::utils {

SessionManager::SessionManager()
    : ctx(qqmusic::details::NetworkContext())
    , ioc(std::make_shared<asio::io_context>())
    , ssl_ctx(std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv12_client)) {
    ssl_ctx->set_default_verify_paths();
}

SessionManager& SessionManager::get_instance() {
    static std::unique_ptr<SessionManager> instance;
    static std::once_flag flag;
    std::call_once(flag, []() { instance.reset(new SessionManager()); });
    return *instance;
}

Session SessionManager::get_session() {
    return {ctx, ioc, ssl_ctx, lock};
}

void SessionManager::set_context(const qqmusic::details::NetworkContext& context) {
    std::lock_guard<std::mutex> lg(lock);
    ctx = context;
}

void SessionManager::push_context(qqmusic::details::NetworkContext&& context) {
    std::lock_guard<std::mutex> lg(lock);
    context_stack.push(std::move(ctx));
    ctx = std::move(context);
}

void SessionManager::pop_context() {
    std::lock_guard<std::mutex> lg(lock);
    ctx = std::move(context_stack.top());
    context_stack.pop();
}

qqmusic::details::NetworkContext& Session::get_context_ref() {
    return local_ctx;
}

void Session::sync_global() {
    std::lock_guard<std::mutex> lg(lock);
    global_ctx = local_ctx;
}

void Session::update_local() {
    std::lock_guard<std::mutex> lg(lock);
    local_ctx = global_ctx;
}

qqmusic::Task<qqmusic::Result<HttpResponse>> Session::perform_request(
    boost::url_view url, http::request<http::string_body>& req, bool auto_redirecting) {
    namespace beast = boost::beast;
    namespace asio = boost::asio;

    if (url.scheme() == "http") {
        /*handle http requests*/
        auto res = co_await handle_http_request(url, req, auto_redirecting);
        if (res.isOk()) {
            co_return Ok(res.unwrap());
        } else {
            co_return Err(res.unwrapErr());
        }
    } else if (url.scheme() == "https") {
        /*handle https requests*/
        auto res = co_await handle_https_request(url, req, auto_redirecting);
        if (res.isOk()) {
            co_return Ok(res.unwrap());
        } else {
            co_return Err(res.unwrapErr());
        }
    } else {
        co_return Err(
            Exception(Exception::NetworkError,
                      std::format("[Session::perform_request] -- Invalid url scheme: `{}`",
                                  std::string(url.scheme()))));
    }
}

qqmusic::Task<qqmusic::Result<HttpResponse>> Session::handle_http_request(
    boost::url_view url, http::request<http::string_body>& req, bool auto_redirecting) {
    namespace beast = boost::beast;
    namespace asio = boost::asio;

    beast::tcp_stream tcps{asio::use_awaitable_t<asio::any_io_executor>::as_default_on(
        beast::tcp_stream(co_await asio::this_coro::executor))};

    auto resolver = asio::use_awaitable_t<asio::any_io_executor>::as_default_on(
        asio::ip::tcp::resolver(co_await asio::this_coro::executor));
    auto resolv_res = co_await resolver.async_resolve(url.host(), "http");

    beast::get_lowest_layer(tcps).expires_after(local_ctx.timeout);

    try {
        /*Connect to endpoint*/
        co_await boost::beast::get_lowest_layer(tcps).async_connect(resolv_res);
    } catch (const boost::system::system_error& e) {
        if (e.code() == boost::asio::error::timed_out) {
            co_return qqmusic::utils::Exception(
                qqmusic::utils::Exception::OperationOutOfTime,
                std::format("[perform_request] -- connecting to host `{}` out of time", url.host()));
        }
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[perform_request] -- connecting to host `{}` failed, reason: `{}`",
                        url.host(),
                        e.what())));
    }

    req.prepare_payload();

    try {
        /*Send the request*/
        co_await beast::http::async_write(tcps, req);
    } catch (const boost::system::system_error& e) {
        if (e.code() == boost::asio::error::timed_out) {
            co_return qqmusic::utils::Exception(
                qqmusic::utils::Exception::OperationOutOfTime,
                std::format("[perform_request] -- connecting to host `{}` out of time", url.host()));
        }
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[perform_request] -- connecting to host `{}` failed, reason: `{}`",
                        url.host(),
                        e.what())));
    }

    http::response<http::dynamic_body> res;
    boost::beast::flat_buffer buffer;
    try {
        /*Receive HTTP response*/
        co_await http::async_read(tcps, buffer, res);
    } catch (const boost::system::system_error& e) {
        if (e.code() == boost::asio::error::timed_out) {
            co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::OperationOutOfTime,
                                                    "Read operation timed out"));
        }
        co_return Err(
            qqmusic::utils::Exception(qqmusic::utils::Exception::NetworkError,
                                      std::format("[perform_request] -- Read operation error: `{}`",
                                                  e.what())));
    }

    /*Shutdown the connection*/
    boost::system::error_code ec = tcps.socket()
                                       .shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    if (!ec || ec == asio::error::eof || ec == boost::beast::error::timeout) {
        /*here we got the request performed successfully*/
        if (res.base().find(http::field::set_cookie) != res.base().end()) {
            /*set cookie*/
            CookieJar cookie_new;
            std::ostringstream oss;
            for (auto& field : res.base()) {
                if (field.name() == http::field::set_cookie) {
                    auto cookie_res = parse_cookie(field.value());
                    if (cookie_res.isErr()) {
                        co_return Err(Exception(
                            Exception::JsonError,
                            std::format("[Session::perform_request] -- Cookie parse error: `{}`",
                                        cookie_res.unwrapErr().what())));
                    }
                    auto cookie_dict = cookie_res.unwrap();
                    std::string domain, path = "/";
                    if (cookie_dict.contains("Domain")) {
                        domain = cookie_dict["Domain"].get<std::string>();
                        cookie_dict.erase("Domain");
                    }
                    if (cookie_dict.contains("Path")) {
                        path = cookie_dict["Path"].get<std::string>();
                        cookie_dict.erase("Path");
                    }
                    for (auto& i : cookie_dict.items()) {
                        cookie_new.set({
                            .domain = domain,
                            .path = path,
                            .key = i.key(),
                            .value = i.value().get<std::string>(),
                        });
                    }
                }
            }
            local_ctx.cookies.merge(cookie_new);
            /*write the change back to global context*/
            sync_global();
        }

        if (auto_redirecting
            && (res.result() == http::status::found
                || res.result() == http::status::moved_permanently
                || res.result() == http::status::permanent_redirect)) {
            /*handle redirecting*/

            auto final_res = co_await handle_http_redirecting(*this, url, req, res);
            if (final_res.isErr()) {
                co_return Err(Exception(final_res.unwrapErr()));
            }
            co_return Ok(final_res.unwrap());
        }

        co_return Ok(res);
    }

    co_return Err(
        qqmusic::utils::Exception(qqmusic::utils::Exception::UnknownError,
                                  std::format("[perform_request] -- Unknown error ocurred: `{}`",
                                              ec.message())));
}

qqmusic::Task<qqmusic::Result<HttpResponse>> Session::handle_https_request(
    boost::url_view url, http::request<http::string_body>& req, bool auto_redirecting) {
    using executor_with_default
        = asio::use_awaitable_t<>::executor_with_default<asio::any_io_executor>;
    using tcp_stream =
        typename boost::beast::tcp_stream::rebind_executor<executor_with_default>::other;
    using tcp = boost::asio::ip::tcp;

    auto resolver = asio::use_awaitable_t<boost::asio::any_io_executor>::as_default_on(
        tcp::resolver(co_await asio::this_coro::executor));

    // We construct the ssl stream from the already rebound tcp_stream.
    boost::beast::ssl_stream<tcp_stream>
        stream{boost::asio::use_awaitable_t<boost::asio::any_io_executor>::as_default_on(
                   boost::beast::tcp_stream(co_await asio::this_coro::executor)),
               *ssl_ctx};

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream.native_handle(), url.host().c_str()))
        throw boost::system::system_error(static_cast<int>(::ERR_get_error()),
                                          asio::error::get_ssl_category());

    // Look up the domain name
    auto const results = co_await resolver.async_resolve(url.host(), "https");

    // Set the timeout.
    boost::beast::get_lowest_layer(stream).expires_after(local_ctx.timeout);

    // Make the connection on the IP address we get from a lookup
    try {
        co_await boost::beast::get_lowest_layer(stream).async_connect(results);
    } catch (const boost::system::system_error& e) {
        if (e.code() == asio::error::timed_out) {
            co_return Err(Exception(Exception::OperationOutOfTime,
                                    "[Session::handle_https_request] -- Connection timed out"));
        }
        co_return Err(Exception(
            Exception::NetworkError,
            std::format("[Session::handle_https_request] -- Error occurred when connecting: `{}`",
                        e.what())));
    }

    // Set the timeout.
    boost::beast::get_lowest_layer(stream).expires_after(local_ctx.timeout);

    // Perform the SSL handshake
    try {
        co_await stream.async_handshake(asio::ssl::stream_base::client);
    } catch (const boost::system::system_error& e) {
        co_return Err(Exception(Exception::SslError,
                                std::format("[Session::handle_https_request] -- Error occurred "
                                            "when performing ssl handshake: `{}`",
                                            e.what())));
    }

    req.prepare_payload();

    // Set the timeout
    boost::beast::get_lowest_layer(stream).expires_after(local_ctx.timeout);

    // Send the HTTP request to the remote host
    try {
        co_await http::async_write(stream, req);
    } catch (const boost::system::system_error& e) {
        if (e.code() == asio::error::timed_out) {
            co_return Err(
                Exception(Exception::OperationOutOfTime,
                          "[Session::handle_https_request] -- Write operation timed out"));
        }
        co_return Err(Exception(Exception::NetworkError,
                                std::format("[Session::handle_https_request] -- Error occurred "
                                            "when doing writing operation: `{}`",
                                            e.what())));
    }

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    boost::beast::flat_buffer buffer;

    // Receive the HTTP response
    try {
        co_await http::async_read(stream, buffer, res);
    } catch (const boost::system::system_error& e) {
        if (e.code() == asio::error::timed_out) {
            co_return Err(Exception(Exception::OperationOutOfTime,
                                    "[Session::handle_https_request] -- Read operation timed out"));
        }
        co_return Err(Exception(Exception::NetworkError,
                                std::format("[Session::handle_https_request] -- Error occurred "
                                            "when doing reading operation: `{}`",
                                            e.what())));
    }

    boost::beast::get_lowest_layer(stream).expires_after(local_ctx.timeout);

    // Gracefully close the stream - do not threat every error as an exception!
    auto [ec] = co_await stream.async_shutdown(asio::as_tuple(asio::use_awaitable));
    if (!ec || ec == asio::error::eof
        || (local_ctx.ignore_ssl_error && ec == asio::ssl::error::stream_truncated)
        || ec == boost::beast::error::timeout || ec == boost::asio::ssl::error::stream_truncated) {
        /*here we got the request performed successfully*/
        if (res.base().find(http::field::set_cookie) != res.base().end()) {
            /*set cookie*/
            CookieJar cookie_new;
            std::ostringstream oss;
            for (auto& field : res.base()) {
                if (field.name() == http::field::set_cookie) {
                    auto cookie_res = parse_cookie(field.value());
                    if (cookie_res.isErr()) {
                        co_return Err(Exception(
                            Exception::JsonError,
                            std::format("[Session::perform_request] -- Cookie parse error: `{}`",
                                        cookie_res.unwrapErr().what())));
                    }
                    auto cookie_dict = cookie_res.unwrap();
                    std::string domain, path = "/";
                    if (cookie_dict.contains("Domain")) {
                        domain = cookie_dict["Domain"].get<std::string>();
                        cookie_dict.erase("Domain");
                    }
                    if (cookie_dict.contains("Path")) {
                        path = cookie_dict["Path"].get<std::string>();
                        cookie_dict.erase("Path");
                    }
                    for (auto& i : cookie_dict.items()) {
                        cookie_new.set({
                            .domain = domain,
                            .path = path,
                            .key = i.key(),
                            .value = i.value().get<std::string>(),
                        });
                    }
                }
            }
            local_ctx.cookies.merge(cookie_new);
            /*write the change back to global context*/
            sync_global();
        }

        if (auto_redirecting
            && (res.result() == http::status::found
                || res.result() == http::status::moved_permanently
                || res.result() == http::status::permanent_redirect)) {
            /*handle redirecting*/
            auto final_res = co_await handle_https_redirecting(*this, url, req, res, ssl_ctx);
            if (final_res.isErr()) {
                co_return Err(Exception(final_res.unwrapErr()));
            }
            co_return Ok(final_res.unwrap());
        }

        co_return Ok(res);
    }

    co_return Err(
        Exception(Exception::UnknownError,
                  std::format("[Session::handle_https_request] -- Error occurred in the end: `{}`",
                              ec.message())));
}

} // namespace qqmusic::utils

static qqmusic::Task<qqmusic::Result<qqmusic::utils::HttpResponse>> handle_http_redirecting(
    qqmusic::utils::Session& self,
    boost::url_view url,
    boost::beast::http::request<boost::beast::http::string_body>& req,
    qqmusic::utils::HttpResponse& resp) {
    using namespace qqmusic::utils;
    const unsigned int REDIRECT_MAX_COUNT = 100;
    unsigned int count = 0;
    try {
        boost::beast::tcp_stream tcps{asio::use_awaitable_t<asio::any_io_executor>::as_default_on(
            boost::beast::tcp_stream(co_await asio::this_coro::executor))};
        auto resolver = asio::use_awaitable_t<asio::any_io_executor>::as_default_on(
            asio::ip::tcp::resolver(co_await asio::this_coro::executor));
        auto& local_context = self.get_context_ref();
        /*filter location*/
        auto location = resp.base().find(http::field::location)->value();

        auto prepare_verb = [](http::request<http::string_body>& req_old,
                               HttpResponse& resp_old) -> http::verb {
            http::verb verb = req_old.method();
            if (resp_old.result() == http::status::see_other && verb != http::verb::head) {
                verb = http::verb::get;
            }
            if (resp_old.result() == http::status::found && verb != http::verb::head) {
                verb = http::verb::get;
            }
            if (resp_old.result() == http::status::moved_permanently && verb == http::verb::post) {
                verb = http::verb::get;
            }
            return verb;
        };

        auto prepare_url = [](boost::url_view old, std::string_view location) -> boost::url {
            boost::url url_new{location};
            /*if a url has no scheme and host, it is a relative url*/
            if (!url_new.has_scheme() && url_new.host().empty()) {
                url_new.set_host(old.host());
            }
            /*if url is not absolute*/
            if (!url_new.is_path_absolute()) {
                url_new.resolve(old);
            }
            /*add fragment*/
            if (old.has_fragment() && !url_new.has_fragment()) {
                url_new.set_fragment(old.fragment());
            }
            return url_new;
        };

        auto prepare_header = [](boost::url_view url_new,
                                 http::request<http::string_body>& old,
                                 http::request<http::string_body>& req_new) {
            req_new.set(http::field::host, url_new.host());
            for (auto& i : old.base()) {
                if (i.name_string() != "Authorization" || i.name_string() != "Content-Length"
                    || i.name_string() != "Cookie" || i.name_string() != "Host") {
                    req_new.set(i.name(), i.value());
                }
            }
        };

        boost::url url_next = prepare_url(url, location);
        http::request<http::string_body> req_next{prepare_verb(req, resp), url_next, 11};
        prepare_header(url_next, req, req_next);
        auto path = url_next.path();
        if (path.size() != 0) {
            auto cookie_res = local_context.cookies.serialize(url_next.host(), path);
            if (cookie_res.isOk()) {
                req_next.set(http::field::cookie, cookie_res.unwrap());
            }
        } else {
            auto cookie_res = local_context.cookies.serialize(url_next.host());
            if (cookie_res.isOk()) {
                req_next.set(http::field::cookie, cookie_res.unwrap());
            }
        }

        while (count < REDIRECT_MAX_COUNT) {
            /*redirecting*/
            qqmusic::utils::HttpResponse res;

            /*send request*/
            auto resolv_res = co_await resolver.async_resolve(url_next.host(), "http");
            boost::beast::get_lowest_layer(tcps).expires_after(local_context.timeout);
            try {
                /*Connect to endpoint*/
                co_await boost::beast::get_lowest_layer(tcps).async_connect(resolv_res);
            } catch (const boost::system::system_error& e) {
                if (e.code() == boost::asio::error::timed_out) {
                    co_return qqmusic::utils::Exception(
                        qqmusic::utils::Exception::OperationOutOfTime,
                        std::format(
                            "[handle_http_redirecting] -- connecting to host `{}` out of time",
                            url.host()));
                }
                co_return Err(
                    qqmusic::utils::Exception(qqmusic::utils::Exception::NetworkError,
                                              std::format("[handle_http_redirecting] -- connecting "
                                                          "to host `{}` failed, reason: `{}`",
                                                          url.host(),
                                                          e.what())));
            }

            req_next.prepare_payload();

            try {
                /*Send the request*/
                co_await boost::beast::http::async_write(tcps, req);
            } catch (const boost::system::system_error& e) {
                if (e.code() == boost::asio::error::timed_out) {
                    co_return qqmusic::utils::Exception(
                        qqmusic::utils::Exception::OperationOutOfTime,
                        std::format(
                            "[handle_http_redirecting] -- connecting to host `{}` out of time",
                            url.host()));
                }
                co_return Err(qqmusic::utils::Exception(
                    qqmusic::utils::Exception::NetworkError,
                    std::format("[handle_http_redirecting] -- connecting to "
                                "host `{}` failed, reason: `{}`",
                                url.host(),
                                e.what())));
            }

            boost::beast::flat_buffer buffer;
            try {
                /*Receive HTTP response*/
                co_await http::async_read(tcps, buffer, res);
            } catch (const boost::system::system_error& e) {
                if (e.code() == boost::asio::error::timed_out) {
                    co_return Err(
                        qqmusic::utils::Exception(qqmusic::utils::Exception::OperationOutOfTime,
                                                  "Read operation timed out"));
                }
                co_return Err(qqmusic::utils::Exception(
                    qqmusic::utils::Exception::NetworkError,
                    std::format("[handle_http_redirecting] -- Read operation error: `{}`",
                                e.what())));
            }

            /*Shutdown the connection*/
            boost::system::error_code ec
                = tcps.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            if (!ec || ec == asio::error::eof || ec == boost::beast::error::timeout) {
                if (res.base().find(http::field::set_cookie) != res.base().end()) {
                    /*set cookie*/
                    CookieJar cookie_new;
                    std::ostringstream oss;
                    for (auto& field : res.base()) {
                        if (field.name() == http::field::set_cookie) {
                            auto cookie_res = parse_cookie(field.value());
                            if (cookie_res.isErr()) {
                                co_return Err(Exception(Exception::JsonError,
                                                        std::format("[Session::perform_request] -- "
                                                                    "Cookie parse error: `{}`",
                                                                    cookie_res.unwrapErr().what())));
                            }
                            auto cookie_dict = cookie_res.unwrap();
                            std::string domain, path = "/";
                            if (cookie_dict.contains("Domain")) {
                                domain = cookie_dict["Domain"].get<std::string>();
                                cookie_dict.erase("Domain");
                            }
                            if (cookie_dict.contains("Path")) {
                                path = cookie_dict["Path"].get<std::string>();
                                cookie_dict.erase("Path");
                            }
                            for (auto& i : cookie_dict.items()) {
                                cookie_new.set({
                                    .domain = domain,
                                    .path = path,
                                    .key = i.key(),
                                    .value = i.value().get<std::string>(),
                                });
                            }
                        }
                    }
                    local_context.cookies.merge(cookie_new);
                    /*write the change back to global context*/
                    self.sync_global();
                }

                if (res.result() == http::status::found
                    || res.result() == http::status::moved_permanently
                    || res.result() == http::status::permanent_redirect) {
                    /*handle redirecting*/
                    auto url_prev = url_next;
                    auto req_old = req_next;
                    location = resp.base().find(http::field::location)->value();
                    url_next = prepare_url(url_prev, location);
                    req_next = http::request<http::string_body>{prepare_verb(req_old, res),
                                                                url_next,
                                                                11};
                    prepare_header(url_next, req_old, req_next);
                    auto path = url_next.path();
                    if (path.size() != 0) {
                        auto cookie_res = local_context.cookies.serialize(url_next.host(), path);
                        if (cookie_res.isOk()) {
                            req_next.set(http::field::cookie, cookie_res.unwrap());
                        }
                    } else {
                        auto cookie_res = local_context.cookies.serialize(url_next.host());
                        if (cookie_res.isOk()) {
                            req_next.set(http::field::cookie, cookie_res.unwrap());
                        }
                    }

                    count++;

                } else {
                    co_return Ok(res);
                }
            }
        }
        co_return Err(Exception(Exception::NetworkError,
                                "[handle_http_redirecting] -- Redirecting too many times"));
    } catch (const std::exception& e) {
        co_return Err(Exception(Exception::UnknownError,
                                std::format("[handle_http_redirecting] -- Error occurred when "
                                            "handling redirecting: `{}`",
                                            e.what())));
    }
}

static qqmusic::Task<qqmusic::Result<qqmusic::utils::HttpResponse>> handle_https_redirecting(
    qqmusic::utils::Session& self,
    boost::url_view url,
    boost::beast::http::request<boost::beast::http::string_body>& req,
    qqmusic::utils::HttpResponse& resp,
    std::shared_ptr<boost::asio::ssl::context> ssl_ctx) {
    using namespace qqmusic::utils;

    try {
        using executor_with_default
            = asio::use_awaitable_t<>::executor_with_default<asio::any_io_executor>;
        using tcp_stream =
            typename boost::beast::tcp_stream::rebind_executor<executor_with_default>::other;
        using tcp = boost::asio::ip::tcp;
        auto& local_ctx = self.get_context_ref();
        auto location = resp.base().find(http::field::location)->value();
        auto resolver = asio::use_awaitable_t<boost::asio::any_io_executor>::as_default_on(
            tcp::resolver(co_await asio::this_coro::executor));

        auto prepare_verb = [](http::request<http::string_body>& req_old,
                               HttpResponse& resp_old) -> http::verb {
            http::verb verb = req_old.method();
            if (resp_old.result() == http::status::see_other && verb != http::verb::head) {
                verb = http::verb::get;
            }
            if (resp_old.result() == http::status::found && verb != http::verb::head) {
                verb = http::verb::get;
            }
            if (resp_old.result() == http::status::moved_permanently && verb == http::verb::post) {
                verb = http::verb::get;
            }
            return verb;
        };

        auto prepare_url = [](boost::url_view old, std::string_view location) -> boost::url {
            boost::url url_new{location};
            /*if a url has no scheme and host, it is a relative url*/
            if (!url_new.has_scheme() && url_new.host().empty()) {
                url_new.set_host(old.host());
            }
            /*if url is not absolute*/
            if (!url_new.is_path_absolute()) {
                url_new.resolve(old);
            }
            /*add fragment*/
            if (old.has_fragment() && !url_new.has_fragment()) {
                url_new.set_fragment(old.fragment());
            }
            return url_new;
        };

        auto prepare_header = [](boost::url_view url_new,
                                 http::request<http::string_body>& old,
                                 http::request<http::string_body>& req_new) {
            req_new.set(http::field::host, url_new.host());
            for (auto& i : old.base()) {
                if (i.name() != http::field::authorization
                    && i.name() != http::field::content_length && i.name() != http::field::cookie
                    && i.name() != http::field::host) {
                    req_new.set(i.name(), i.value());
                }
            }
        };

        boost::url url_next = prepare_url(url, location);
        http::request<http::string_body> req_next{prepare_verb(req, resp), url_next, 11};
        prepare_header(url_next, req, req_next);
        auto path = url_next.path();
        if (path.size() != 0) {
            auto cookie_res = local_ctx.cookies.serialize(url_next.host(), path);
            if (cookie_res.isOk()) {
                req_next.set(http::field::cookie, cookie_res.unwrap());
            }
        } else {
            auto cookie_res = local_ctx.cookies.serialize(url_next.host());
            if (cookie_res.isOk()) {
                req_next.set(http::field::cookie, cookie_res.unwrap());
            }
        }

        unsigned int count = 0;
        const unsigned int REDIRECT_MAX_COUNT = 100;
        while (count < REDIRECT_MAX_COUNT) {
            /*redirecting*/
            qqmusic::utils::HttpResponse res;
            boost::beast::flat_buffer buffer;

            // We construct the ssl stream from the already rebound tcp_stream.
            boost::beast::ssl_stream<tcp_stream>
                stream{boost::asio::use_awaitable_t<boost::asio::any_io_executor>::as_default_on(
                           boost::beast::tcp_stream(co_await asio::this_coro::executor)),
                       *ssl_ctx};

            // Set SNI Hostname (many hosts need this to handshake successfully)
            if (!SSL_set_tlsext_host_name(stream.native_handle(), url_next.host().c_str()))
                throw boost::system::system_error(static_cast<int>(::ERR_get_error()),
                                                  asio::error::get_ssl_category());

            // Look up the domain name
            auto const results = co_await resolver.async_resolve(url_next.host(), "https");

            // Set the timeout.
            boost::beast::get_lowest_layer(stream).expires_after(local_ctx.timeout);

            // Make the connection on the IP address we get from a lookup
            try {
                co_await boost::beast::get_lowest_layer(stream).async_connect(results);
            } catch (const boost::system::system_error& e) {
                if (e.code() == asio::error::timed_out) {
                    co_return Err(Exception(Exception::OperationOutOfTime,
                                            "[handle_https_redirecting] -- Connection timed out"));
                }
                co_return Err(Exception(Exception::NetworkError,
                                        std::format("[handle_https_redirecting] -- Error occurred "
                                                    "when connecting: `{}`",
                                                    e.what())));
            }

            // Set the timeout.
            boost::beast::get_lowest_layer(stream).expires_after(local_ctx.timeout);

            // Perform the SSL handshake
            try {
                co_await stream.async_handshake(asio::ssl::stream_base::client);
            } catch (const boost::system::system_error& e) {
                co_return Err(Exception(Exception::SslError,
                                        std::format("[handle_https_redirecting] -- Error occurred "
                                                    "when performing ssl handshake: `{}`",
                                                    e.what())));
            }

            req_next.prepare_payload();

            // Set the timeout
            boost::beast::get_lowest_layer(stream).expires_after(local_ctx.timeout);

            // Send the HTTP request to the remote host
            try {
                co_await http::async_write(stream, req_next);
            } catch (const boost::system::system_error& e) {
                if (e.code() == asio::error::timed_out) {
                    co_return Err(
                        Exception(Exception::OperationOutOfTime,
                                  "[handle_https_redirecting] -- Write operation timed out"));
                }
                co_return Err(Exception(Exception::NetworkError,
                                        std::format("[handle_https_redirecting] -- Error occurred "
                                                    "when doing writing operation: `{}`",
                                                    e.what())));
            }

            // Receive the HTTP response
            try {
                co_await http::async_read(stream, buffer, res);
            } catch (const boost::system::system_error& e) {
                if (e.code() == asio::error::timed_out) {
                    co_return Err(
                        Exception(Exception::OperationOutOfTime,
                                  "[handle_https_redirecting] -- Read operation timed out"));
                }
                co_return Err(Exception(Exception::NetworkError,
                                        std::format("[handle_https_redirecting] -- Error occurred "
                                                    "when doing reading operation: `{}`",
                                                    e.what())));
            }

            boost::beast::get_lowest_layer(stream).expires_after(local_ctx.timeout);

            // Gracefully close the stream - do not threat every error as an exception!
            auto [ec] = co_await stream.async_shutdown(asio::as_tuple(asio::use_awaitable));
            if (!ec || ec == asio::error::eof
                || (local_ctx.ignore_ssl_error && ec == asio::ssl::error::stream_truncated)
                || ec == boost::beast::error::timeout
                || ec == boost::asio::ssl::error::stream_truncated) {
                // If we get here then the connection is closed gracefully
                if (res.base().find(http::field::set_cookie) != res.base().end()) {
                    /*set cookie*/
                    CookieJar cookie_new;
                    std::ostringstream oss;
                    for (auto& field : res.base()) {
                        if (field.name() == http::field::set_cookie) {
                            auto cookie_res = parse_cookie(field.value());
                            if (cookie_res.isErr()) {
                                co_return Err(Exception(Exception::JsonError,
                                                        std::format("[Session::perform_request] -- "
                                                                    "Cookie parse error: `{}`",
                                                                    cookie_res.unwrapErr().what())));
                            }
                            auto cookie_dict = cookie_res.unwrap();
                            std::string domain, path = "/";
                            if (cookie_dict.contains("Domain")) {
                                domain = cookie_dict["Domain"].get<std::string>();
                                cookie_dict.erase("Domain");
                            }
                            if (cookie_dict.contains("Path")) {
                                path = cookie_dict["Path"].get<std::string>();
                                cookie_dict.erase("Path");
                            }
                            for (auto& i : cookie_dict.items()) {
                                cookie_new.set({
                                    .domain = domain,
                                    .path = path,
                                    .key = i.key(),
                                    .value = i.value().get<std::string>(),
                                });
                            }
                        }
                    }
                    local_ctx.cookies.merge(cookie_new);
                    /*write the change back to global context*/
                    self.sync_global();
                }

                if (res.result() == http::status::found
                    || res.result() == http::status::moved_permanently
                    || res.result() == http::status::permanent_redirect) {
                    /*prepare next request*/
                    auto url_prev = url_next;
                    auto req_old = req_next;
                    location = resp.base().find(http::field::location)->value();
                    url_next = prepare_url(url_prev, location);
                    req_next = http::request<http::string_body>{prepare_verb(req_old, res),
                                                                url_next,
                                                                11};
                    prepare_header(url_next, req_old, req_next);
                    auto path = url_next.path();
                    if (path.size() != 0) {
                        auto cookie_res = local_ctx.cookies.serialize(url_next.host(), path);
                        if (cookie_res.isOk()) {
                            req_next.set(http::field::cookie, cookie_res.unwrap());
                        }
                    } else {
                        auto cookie_res = local_ctx.cookies.serialize(url_next.host());
                        if (cookie_res.isOk()) {
                            req_next.set(http::field::cookie, cookie_res.unwrap());
                        }
                    }

                    count++;
                } else {
                    co_return Ok(res);
                }
            }
            co_return Err(Exception(
                Exception::UnknownError,
                std::format("[handle_https_redirecting] -- Error occurred in the end: `{}`",
                            ec.message())));
        }
        co_return Err(Exception(Exception::NetworkError,
                                "[handle_https_redirecting] -- Redirecting too many times"));
    } catch (const std::exception& e) {
        co_return Err(Exception(Exception::UnknownError,
                                std::format("[handle_https_redirecting] -- Error occurred when "
                                            "handling redirecting: `{}`",
                                            e.what())));
    }
}
```


