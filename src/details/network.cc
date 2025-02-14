#include <boost/beast.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/write.hpp>
#include <mutex>
#include <openssl/err.h>
#include <qqmusic/details/network.h>
#include <qqmusic/result.h>

qqmusic::Task<qqmusic::Result<qqmusic::details::HttpResponse>>
qqmusic::details::NetworkContextManager::perform_request(
    std::string host, boost::beast::http::request<boost::beast::http::string_body>& req) {
    namespace beast = boost::beast;
    namespace asio = boost::asio;

    /*Initialize TCP Stream*/
    beast::tcp_stream tcps{asio::use_awaitable_t<boost::asio::any_io_executor>::as_default_on(
        beast::tcp_stream(co_await asio::this_coro::executor))};

    /*Resolve host*/
    // auto resolver = asio::ip::tcp::resolver(ioc);
    auto resolver = asio::use_awaitable_t<asio::any_io_executor>::as_default_on(
        asio::ip::tcp::resolver(co_await asio::this_coro::executor));
    auto resolv_res = co_await resolver.async_resolve(host, "http");

    /*Set timeout for connection*/
    beast::get_lowest_layer(tcps).expires_after(timeout);

    try {
        /*Connect to endpoint*/
        co_await boost::beast::get_lowest_layer(tcps).async_connect(resolv_res);
    } catch (const boost::system::system_error& e) {
        if (e.code() == boost::asio::error::timed_out) {
            co_return qqmusic::utils::Exception(
                qqmusic::utils::Exception::OperationOutOfTime,
                std::format("[perform_request] -- connecting to host `{}` out of time", host));
        }
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[perform_request] -- connecting to host `{}` failed, reason: `{}`",
                        host,
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
                std::format("[perform_request] -- connecting to host `{}` out of time", host));
        }
        co_return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::NetworkError,
            std::format("[perform_request] -- connecting to host `{}` failed, reason: `{}`",
                        host,
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

qqmusic::details::NetworkContextManager& qqmusic::details::NetworkContextManager::get_instance() {
    static NetworkContextManager ncm;
    return ncm;
}

qqmusic::details::GlobalContext qqmusic::details::NetworkContextManager::get_global_context() {
    std::lock_guard<std::mutex> lg(this->lock);
    return this->ctx;
}

void qqmusic::details::NetworkContextManager::set_global_context(const GlobalContext& ctx) {
    std::lock_guard<std::mutex> lg(this->lock);
    this->ctx = ctx;
}
