/*------------------------qqmusic/details/network.h-----------------------------
 * Provide a basic interface to perform http request and store global api info
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETAILS_NETWORK_H
#define QQMUSIC_DETAILS_NETWORK_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <exception>
#include <format>
#include <qqmusic/details/device.h>
#include <qqmusic/details/qimei.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>
#include <stdexcept>

namespace qqmusic::details {

using HttpResponse = boost::beast::http::response<boost::beast::http::dynamic_body>;
namespace http = boost::beast::http;

const char VERSION[] = "13.2.5.8";
const uint64_t VERSION_CODE = 12030508;

struct ApiConfig {
    std::string version;
    uint64_t version_code = 0;
    bool enable_sign = false;
    std::string endpoint;
    std::string enc_endpoint;
};

class NetworkContextManager {
public:
    NetworkContextManager(const NetworkContextManager&) = delete;
    NetworkContextManager& operator=(const NetworkContextManager&) = delete;

    /*THIS function perform common http request*/
    qqmusic::Task<qqmusic::Result<HttpResponse>> perform_request(
        std::string host, http::request<http::string_body>& req);

    static NetworkContextManager& get_instance();

    nlohmann::json get_cookie();
    qqmusic::utils::Credential get_credential();
    ApiConfig get_api_config();
    void set_timeout(std::chrono::seconds& duration);
    void set_cookie(const nlohmann::json& cookie);
    void set_credential(const qqmusic::utils::Credential& credential);

private:
    NetworkContextManager()
        : ssl_ctx(boost::asio::ssl::context::tlsv12_client)
        , timeout(std::chrono::seconds(20)) {
        context.cookie = nlohmann::json();
        context.credential = qqmusic::utils::Credential();
        context.api_config = {
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
        context.device = dev_res.unwrap();
        auto qimei_res = get_qimei(context.device, VERSION);
        if (qimei_res.isErr()) {
            throw(std::runtime_error(std::format("Unkown error: get qimei failed because of {}",
                                                 qimei_res.unwrapErr().what())));
        }
        context.qimei = qimei_res.unwrap();
        context.device.qimei = context.qimei.q36;
        cache_device(context.device);
    };

    boost::asio::ssl::context ssl_ctx;
    std::chrono::seconds timeout;

    /*global context*/
    struct {
        nlohmann::json cookie;
        qqmusic::utils::Credential credential;
        ApiConfig api_config;
        qqmusic::details::Device device;
        qqmusic::details::QimeiResult qimei;
    } context;

    /*mutex on global context*/
    std::mutex lock;
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_NETWORK_H
