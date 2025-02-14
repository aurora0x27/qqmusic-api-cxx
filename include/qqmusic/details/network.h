/*------------------------qqmusic/details/network.h-----------------------------
 * Provide a basic interface to perform http request and store global api info
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETAILS_NETWORK_H
#define QQMUSIC_DETAILS_NETWORK_H

#include <boost/asio.hpp>
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <format>
#include <qqmusic/details/device.h>
#include <qqmusic/details/qimei.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic::details {

using HttpResponse = boost::beast::http::response<boost::beast::http::dynamic_body>;
namespace http = boost::beast::http;

const char VERSION[] = "13.2.5.8";
const uint64_t VERSION_CODE = 12020508;

struct ApiConfig {
    std::string version;
    uint64_t version_code = 0;
    bool enable_sign = false;
    std::string endpoint;
    std::string enc_endpoint;
};

struct GlobalContext {
    nlohmann::json cookie;
    qqmusic::utils::Credential credential;
    ApiConfig api_config;
    qqmusic::details::Device device;
    qqmusic::details::QimeiResult qimei;
};

class NetworkContextManager {
public:
    NetworkContextManager(const NetworkContextManager&) = delete;
    NetworkContextManager& operator=(const NetworkContextManager&) = delete;

    /*THIS function perform common http request*/
    qqmusic::Task<qqmusic::Result<HttpResponse>> perform_request(
        std::string host, http::request<http::string_body>& req);

    static NetworkContextManager& get_instance();

    /*get a copy of global context for param building and write back the change*/
    GlobalContext get_global_context();
    void set_global_context(const GlobalContext& ctx);

private:
    NetworkContextManager()
        : timeout(std::chrono::seconds(20)) {
        ctx.cookie = nlohmann::json();
        ctx.credential = qqmusic::utils::Credential();
        ctx.api_config = {
            .version = VERSION,
            .version_code = VERSION_CODE,
            .enable_sign = false,
            .endpoint = "https://u.y.qq.com/cgi-bin/musicu.fcg",
            .enc_endpoint = "https://u.y.qq.com/cgi-bin/musics.fcg",
        };
        auto dev_res = qqmusic::details::get_device_info();
        if (dev_res.isErr()) {
            throw(std::runtime_error(std::format("Unknown error: get device failed because of {}",
                                                 dev_res.unwrapErr().what())));
        }
        ctx.device = dev_res.unwrap();
        if (ctx.device.qimei == "") {
            auto qimei_res = get_qimei(ctx.device, VERSION);
            if (qimei_res.isErr()) {
                throw(std::runtime_error(std::format("Unkown error: get qimei failed because of {}",
                                                     qimei_res.unwrapErr().what())));
            } else {
                ctx.qimei = qimei_res.unwrap();
            }
        } else {
            ctx.qimei = {.q16 = "", .q36 = ctx.device.qimei};
        }
        ctx.device.qimei = ctx.qimei.q36;
        cache_device(ctx.device);
    };

    boost::asio::io_context ioc;
    std::chrono::seconds timeout;

    /*global context*/
    struct GlobalContext ctx;

    /*mutex on global context*/
    std::mutex lock;
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_NETWORK_H
