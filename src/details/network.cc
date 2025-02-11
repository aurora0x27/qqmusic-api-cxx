#include <boost/beast.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
#include <mutex>
#include <openssl/err.h>
#include <qqmusic/details/network.h>
#include <qqmusic/result.h>

qqmusic::Task<qqmusic::Result<qqmusic::details::HttpResponse>>
qqmusic::details::NetworkContextManager::perform_request(
    std::string host, boost::beast::http::request<boost::beast::http::string_body>& req) {
    try {
        using namespace boost::beast;
        using namespace boost::asio;
        auto resolver = use_awaitable_t<any_io_executor>::as_default_on(
            ip::tcp::resolver(co_await this_coro::executor));

        /*code from evento:HttpAccessManager.cc... I don't know what it does actually*/
        ssl_stream<boost::beast::tcp_stream::rebind_executor<
            boost::asio::use_awaitable_t<>::executor_with_default<any_io_executor>>::other>
            stream{use_awaitable_t<boost::asio::any_io_executor>::as_default_on(
                       tcp_stream(co_await this_coro::executor)),
                   this->ssl_ctx};

        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            auto ec = ERR_get_error();
            co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::SslError,
                                                    ERR_error_string(ec, nullptr)));
        }

        // Look up the domain name
        auto const results = co_await resolver.async_resolve(host, "http");

        // Set the timeout.
        boost::beast::get_lowest_layer(stream).expires_after(timeout);

        // Make the connection on the IP address we get from a lookup
        try {
            co_await boost::beast::get_lowest_layer(stream).async_connect(results);
        } catch (const boost::system::system_error& e) {
            if (e.code() == boost::asio::error::timed_out) {
                co_return qqmusic::utils::Exception(
                    qqmusic::utils::Exception::OperationOutOfTime,
                    std::format("[perform_request] -- connecting to host {} out of time", host));
            }
            co_return Err(qqmusic::utils::Exception(
                qqmusic::utils::Exception::NetworkError,
                std::format("[perform_request] -- connecting to host {} failed, reason: {}",
                            host,
                            e.what())));
        }

        // Set the timeout.
        get_lowest_layer(stream).expires_after(timeout);

        // Perform the SSL handshake
        try {
            co_await stream.async_handshake(ssl::stream_base::client);
        } catch (const boost::system::system_error& e) {
            co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::SslError, e.what()));
        }

        req.prepare_payload();

        // Set the timeout
        get_lowest_layer(stream).expires_after(timeout);

        // Send the HTTP request to the remote host
        try {
            co_await http::async_write(stream, req);
        } catch (const boost::system::system_error& e) {
            if (e.code() == net::error::timed_out) {
                co_return Err(
                    qqmusic::utils::Exception(qqmusic::utils::Exception::OperationOutOfTime,
                                              "Write operation timed out"));
            }
            co_return Err(
                qqmusic::utils::Exception(qqmusic::utils::Exception::NetworkError, e.what()));
        }

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        boost::beast::flat_buffer buffer;

        // Receive the HTTP response
        try {
            co_await http::async_read(stream, buffer, res);
        } catch (const boost::system::system_error& e) {
            if (e.code() == boost::asio::error::timed_out) {
                co_return Err(
                    qqmusic::utils::Exception(qqmusic::utils::Exception::OperationOutOfTime,
                                              "Read operation timed out"));
            }
            co_return Err(
                qqmusic::utils::Exception(qqmusic::utils::Exception::NetworkError, e.what()));
        }
        get_lowest_layer(stream).expires_after(std::chrono::seconds(2));

        // Gracefully close the stream - do not threat every error as an exception!
        auto [ec] = co_await stream.async_shutdown(net::as_tuple(net::use_awaitable));
        if (!ec || ec == net::error::eof || ec == boost::beast::error::timeout) {
            // If we get here then the connection is closed gracefully
            co_return Ok(res);
        }

        co_return Err(
            qqmusic::utils::Exception(qqmusic::utils::Exception::UnknownError, ec.message()));

    } catch (const std::exception& e) {
        co_return Err(
            qqmusic::utils::Exception(qqmusic::utils::Exception::UnknownError,
                                      std::format("[perform_request] -- Unknown error occured: {}",
                                                  e.what())));
    }
}

qqmusic::details::NetworkContextManager& qqmusic::details::NetworkContextManager::get_instance() {
    static NetworkContextManager ncm;
    return ncm;
}

void qqmusic::details::NetworkContextManager::set_cookie(const nlohmann::json& cookie) {
    std::lock_guard<std::mutex> lock_guard(lock);
    context.cookie = cookie;
}

void qqmusic::details::NetworkContextManager::set_credential(
    const qqmusic::utils::Credential& credential) {
    std::lock_guard<std::mutex> lock_guard(lock);
    context.credential = credential;
}

void qqmusic::details::NetworkContextManager::set_timeout(std::chrono::seconds& duration) {
    std::lock_guard<std::mutex> lock_guard(lock);
    timeout = duration;
}

qqmusic::details::ApiConfig qqmusic::details::NetworkContextManager::get_api_config() {
    std::lock_guard<std::mutex> lock_guard(lock);
    return context.api_config;
}

nlohmann::json qqmusic::details::NetworkContextManager::get_cookie() {
    std::lock_guard<std::mutex> lock_guard(lock);
    return context.cookie;
}

qqmusic::utils::Credential qqmusic::details::NetworkContextManager::get_credential() {
    std::lock_guard<std::mutex> lock_guard(lock);
    return context.credential;
}
