#include "qqmusic/result.h"
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/url/url.hpp>
#include <botan/auto_rng.h>
#include <qqmusic/details/network.h>
#include <qqmusic/login.h>
#include <string>

static qqmusic::Task<qqmusic::Result<qqmusic::details::HttpResponse>> handle_redirect();

qqmusic::Task<qqmusic::Result<qqmusic::QRCode>> qqmusic::get_qrcode(qqmusic::QRLoginType login_type) {
    if (login_type == qqmusic::QRLoginType::QQ) {
        /*qq login*/
        boost::urls::url url{"https://ssl.ptlogin2.qq.com/ptqrshow"};
        Botan::AutoSeeded_RNG rng;
        /*random unsigned long long generator*/
        auto randull = [&rng]() -> uint64_t {
            uint64_t res = 0;
            std::vector<uint8_t> buffer(sizeof(res));
            rng.randomize(buffer);
            memcpy(&res, buffer.data(), sizeof(res));
            return res;
        };
        url.params() = {
            {"appid", "716027609"},
            {"e", "2"},
            {"l", "M"},
            {"s", "3"},
            {"d", "72"},
            {"v", "4"},
            {"t", "0." + std::to_string(randull())},
            {"daid", "383"},
            {"pt_3rd_aid", "100497308"},
        };
        boost::beast::http::request<boost::beast::http::string_body>
            req{boost::beast::http::verb::get, url, 11};
        nlohmann::json headers
            = {{"host", "ssl.ptlogin2.qq.com"},
               {"accept", "*/*"},
               {"accept-encoding", "ideflate"},
               {"connection", "keep-alive"},
               {"user-agent",
                "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54"},
               {"referer", "https://xui.ptlogin2.qq.com/"}};

        for (auto& i : headers.items()) {
            req.set(i.key(), i.value());
        }

        std::cout << std::endl << req << std::endl;
        auto& ncm = qqmusic::details::NetworkContextManager::get_instance();
        auto response_res = co_await ncm.perform_request(url.host(), req);
        if (response_res.isErr()) {
            co_return Err(qqmusic::utils::Exception(response_res.unwrapErr()));
        }
        auto response = response_res.unwrap();
        /* TODO: handle redirecting*/
    } else {
        /*wechat*/
    }
    co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::UnknownError));
}
