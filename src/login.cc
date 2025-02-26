#include "qqmusic/utils/common.h"
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <botan/auto_rng.h>
#include <qqmusic/login.h>
#include <qqmusic/utils/session.h>
#include <string>

namespace qqmusic {

qqmusic::Task<qqmusic::Result<QRCode>> get_qrcode(QRLoginType login_type) {
    namespace http = boost::beast::http;
    namespace asio = boost::asio;
    auto session = utils::SessionManager::get_instance().get_session();
    auto& context = session.get_context_ref();
    if (login_type == QRLoginType::QQ) {
        /*qq login*/
        boost::url url{"https://ssl.ptlogin2.qq.com/ptqrshow"};

        Botan::AutoSeeded_RNG rng;
        /*random unsigned long long generator*/
        auto randull = [&rng]() -> uint64_t {
            uint64_t res = 0;
            std::vector<uint8_t> buffer(sizeof(res));
            rng.randomize(buffer);
            memcpy(&res, buffer.data(), sizeof(res));
            return res;
        };
        url.set_params({{"appid", "716027609"},
                        {"e", "2"},
                        {"l", "M"},
                        {"s", "3"},
                        {"d", "72"},
                        {"v", "4"},
                        /* origin python code: `"t": str(random.random()),` */
                        {"t", "0." + std::to_string(randull() % 10000000000000000)},
                        {"daid", "383"},
                        {"pt_3rd_aid", "100497308"}});

        http::request<http::string_body> req{http::verb::get, url, 11};
        req.set(http::field::host, url.host());
        req.set(http::field::accept, "*/*");
        /*use raw buffer instead of compressed buffer when debuging*/
        req.set(http::field::accept_encoding, "gzip, deflate");
        req.set(http::field::connection, "keep-alive");
        req.set(http::field::referer, "y.qq.com");
        req.set(http::field::user_agent,
                "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");
        req.set(http::field::referer, "https://xui.ptlogin2.qq.com/");

        /*send request*/
        auto res = co_await session.perform_request(url, req);
        if (res.isErr()) {
            co_return Err(utils::Exception(
                utils::Exception::NetworkError,
                std::format("[get_qrcode] -- Error occurred when performing https request: `{}`",
                            res.unwrapErr().what())));
        }
        auto resp = res.unwrap();
        auto data = utils::to_buffer(resp);
        auto qrsig_res = context.cookies.get("qrsig");
        auto qrsig = qrsig_res.unwrap();
        if (qrsig_res.isErr()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           "[get_qrcode] -- Cannot get qrsig from cookie"));
        }
        co_return Ok(QRCode{.qr_type = QRLoginType::QQ,
                            .identifier = qrsig,
                            .mimie_type = "image/png",
                            .data = data});
    } else {
        /*wechat login*/
        co_return Err(utils::Exception(utils::Exception::UnknownError, "Not implemented yet"));
    }
}

} // namespace qqmusic
