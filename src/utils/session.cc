#include <boost/asio/use_awaitable.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core/stream_traits.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/url.hpp>
#include <memory>
#include <mutex>
#include <qqmusic/result.h>
#include <qqmusic/utils/cookie.h>
#include <qqmusic/utils/session.h>
#include <utility>

static qqmusic::Task<qqmusic::Result<qqmusic::utils::HttpResponse>> handle_redirecting(
    qqmusic::utils::Session& self,
    boost::url_view url,
    boost::beast::http::request<boost::beast::http::string_body>& req,
    qqmusic::utils::HttpResponse& resp);

namespace qqmusic::utils {

SessionManager::SessionManager()
    : ioc(std::make_shared<asio::io_context>())
    , ctx(qqmusic::details::NetworkContext()) {}

SessionManager& SessionManager::get_instance() {
    static std::unique_ptr<SessionManager> instance;
    static std::once_flag flag;
    std::call_once(flag, []() { instance.reset(new SessionManager()); });
    return *instance;
}

Session SessionManager::get_session() {
    return std::move(Session(ctx, ioc, lock));
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
    boost::url_view url, http::request<http::string_body>& req) {
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
        if (res.base().find(http::field::set_cookie) != res.base().end()
            || res.base().find(http::field::set_cookie2) != res.base().end()) {
            /*set cookie*/
            auto field = res.base().find(http::field::set_cookie);
            if (field == res.base().end()) {
                auto field = res.base().find(http::field::set_cookie2);
            }
            auto cookie_res = parse_cookie(field->value());
            if (cookie_res.isErr()) {
                co_return Err(
                    Exception(Exception::JsonError,
                              std::format("[Session::perform_request] -- Cookie parse error: `{}`",
                                          cookie_res.unwrapErr().what())));
            }
            CookieJar cookie_new;
            auto cookie_dict = cookie_res.unwrap();
            std::string domain, path = "/";
            auto domain_field = req.base().find(http::field::host);
            if (cookie_dict.contains("domain")) {
                domain = cookie_dict["domain"].get<std::string>();
                cookie_dict.erase("domain");
            }
            if (cookie_dict.contains("path")) {
                path = cookie_dict["path"].get<std::string>();
                cookie_dict.erase("path");
            }
            for (auto& i : cookie_dict.items()) {
                cookie_new.set({
                    .domain = domain,
                    .path = path,
                    .key = i.key(),
                    .value = i.value().get<std::string>(),
                });
            }
            local_ctx.cookies.merge(cookie_new);
            /*write the change back to global context*/
            sync_global();
        }

        if (res.result() == http::status::found || res.result() == http::status::moved_permanently
            || res.result() == http::status::permanent_redirect) {
            /*handle redirecting*/
            auto final_res = co_await handle_redirecting(*this, url, req, res);
            if (final_res.isErr()) {
                co_return Err(Exception(final_res.unwrapErr()));
            }
            co_return Ok(final_res.unwrap());
        }

        // // For DEBUGING
        // std::cout << "Response headers" << std::endl;
        // std::cout << res.result_int() << std::endl;
        // std::cout << res.result() << std::endl;
        // std::cout << res.reason() << std::endl;
        // for (auto& h : res.base()) {
        //     std::cout << "Field: " << h.name() << "/text: " << h.name_string()
        //               << ", Value: " << h.value() << "\n";
        // }
        // std::cout << "Response body: " << std::endl
        //           << beast::buffers_to_string(res.body().data()) << std::endl;
        co_return Ok(res);
    }

    co_return Err(
        qqmusic::utils::Exception(qqmusic::utils::Exception::UnknownError,
                                  std::format("[perform_request] -- Unknown error ocurred: `{}`",
                                              ec.message())));
}

} // namespace qqmusic::utils

static qqmusic::Task<qqmusic::Result<qqmusic::utils::HttpResponse>> handle_redirecting(
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
        auto ctx = self.get_context_ref();
        qqmusic::utils::HttpResponse res;
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

        auto prepare_header = [](http::request<http::string_body>& old,
                                 http::request<http::string_body>& req_new) {
            for (auto& i : old.base()) {
                if (i.name_string() != "Authorization" || i.name_string() != "Content-Length"
                    || i.name_string() != "Cookie") {
                    req_new.set(i.name(), i.value());
                }
            }
        };

        boost::url url_next = prepare_url(url, location);
        http::request<http::string_body> req_next{prepare_verb(req, res), url_next, 11};
        prepare_header(req, req_next);
        auto cookie_res = ctx.cookies.serialize(url.host(), url.path());
        if (cookie_res.isOk()) {
            req_next.set(http::field::cookie, cookie_res.unwrap());
        }

        while (count < REDIRECT_MAX_COUNT) {
            /*redirecting*/

            /*send request*/
            auto resolv_res = co_await resolver.async_resolve(url_next.host(), "http");
            boost::beast::get_lowest_layer(tcps).expires_after(ctx.timeout);
            try {
                /*Connect to endpoint*/
                co_await boost::beast::get_lowest_layer(tcps).async_connect(resolv_res);
            } catch (const boost::system::system_error& e) {
                if (e.code() == boost::asio::error::timed_out) {
                    co_return qqmusic::utils::Exception(
                        qqmusic::utils::Exception::OperationOutOfTime,
                        std::format("[perform_request] -- connecting to host `{}` out of time",
                                    url.host()));
                }
                co_return Err(qqmusic::utils::Exception(
                    qqmusic::utils::Exception::NetworkError,
                    std::format("[perform_request] -- connecting to host `{}` failed, reason: `{}`",
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
                        std::format("[handle_redirecting] -- connecting to host `{}` out of time",
                                    url.host()));
                }
                co_return Err(
                    qqmusic::utils::Exception(qqmusic::utils::Exception::NetworkError,
                                              std::format("[handle_redirecting] -- connecting to "
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
                    std::format("[handle_redirecting] -- Read operation error: `{}`", e.what())));
            }

            /*Shutdown the connection*/
            boost::system::error_code ec
                = tcps.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            if (!ec || ec == asio::error::eof || ec == boost::beast::error::timeout) {
                if (res.base().find(http::field::set_cookie) != res.base().end()
                    || res.base().find(http::field::set_cookie2) != res.base().end()) {
                    /*set cookie*/
                    auto field = res.base().find(http::field::set_cookie);
                    if (field == res.base().end()) {
                        auto field = res.base().find(http::field::set_cookie2);
                    }
                    auto cookie_res = parse_cookie(field->value());
                    if (cookie_res.isErr()) {
                        co_return Err(Exception(
                            Exception::JsonError,
                            std::format("[handle_redirecting] -- Cookie parse error: `{}`",
                                        cookie_res.unwrapErr().what())));
                    }
                    CookieJar cookie_new;
                    auto cookie_dict = cookie_res.unwrap();
                    std::string domain, path = "/";
                    auto domain_field = req.base().find(http::field::host);
                    if (cookie_dict.contains("domain")) {
                        domain = cookie_dict["domain"].get<std::string>();
                        cookie_dict.erase("domain");
                    }
                    if (cookie_dict.contains("path")) {
                        path = cookie_dict["path"].get<std::string>();
                        cookie_dict.erase("path");
                    }
                    for (auto& i : cookie_dict.items()) {
                        cookie_new.set({
                            .domain = domain,
                            .path = path,
                            .key = i.key(),
                            .value = i.value().get<std::string>(),
                        });
                    }
                    ctx.cookies.merge(cookie_new);
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
                    req_next = http::request<http::string_body>(prepare_verb(req_next, res),
                                                                url_next,
                                                                11);
                    prepare_header(req_next, req_old);
                    auto cookie_res = ctx.cookies.serialize(url_prev.host(), url_prev.path());
                    if (cookie_res.isOk()) {
                        req_next.set(http::field::cookie, cookie_res.unwrap());
                    }

                    count++;

                } else {
                    co_return Ok(res);
                }
            }
        }
        co_return Err(Exception(Exception::NetworkError,
                                "[handle_redirecting] -- Redirecting too many times"));
    } catch (const std::exception& e) {
        co_return Err(Exception(
            Exception::UnknownError,
            std::format("[handle_redirecting] -- Error occurred when handling redirecting: `{}`",
                        e.what())));
    }
}
