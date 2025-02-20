/*------------------------qqmusic/details/context.h-----------------------------
 * class NetworkContext provide global shared api info.
 * Also provide global default context
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETAILS_CONTEXT_H
#define QQMUSIC_DETAILS_CONTEXT_H

#include <mutex>
#include <qqmusic/utils/cookie.h>
#include <qqmusic/utils/credential.h>
#include <qqmusic/utils/device.h>
#include <qqmusic/utils/qimei.h>
#include <stdexcept>

namespace qqmusic::details {

const char VERSION[] = "13.2.5.8";
const uint64_t VERSION_CODE = 13020508;

struct ApiConfig {
    std::string version = VERSION;
    uint64_t version_code = VERSION_CODE;
    bool enable_sign = false;
    std::string endpoint = "https://u.y.qq.com/cgi-bin/musicu.fcg";
    std::string enc_endpoint = "https://u.y.qq.com/cgi-bin/musics.fcg";
};

// FIXME: desperated after debugging
struct GlobalContext {
    nlohmann::json cookie;
    qqmusic::utils::Credential credential;
    ApiConfig api_config;
    qqmusic::utils::Device device;
    qqmusic::utils::QimeiResult qimei;
};

struct NetworkContext {
    /*Globally shared vars*/
    qqmusic::utils::CookieJar cookies;
    qqmusic::utils::Credential credential;
    ApiConfig api_config;
    qqmusic::utils::Device device;
    qqmusic::utils::QimeiResult qimei;

    NetworkContext() {
        api_config = ApiConfig();
        auto device_res = qqmusic::utils::get_device_info();
        if (device_res.isErr()) {
            throw std::runtime_error(device_res.unwrapErr().what());
        }
        device = device_res.unwrap();
        auto qimei_res = get_qimei(device, api_config.version);
        if (qimei_res.isErr()) {
            throw std::runtime_error(qimei_res.unwrapErr().what());
        }
        qimei = qimei_res.unwrap();
    }
};

class NetworkContextManager {
public:
    NetworkContextManager(const NetworkContextManager&) = delete;
    NetworkContextManager operator=(const NetworkContextManager&) = delete;

    NetworkContext get();
    void set(const NetworkContext& ctx);

    /*Only entry*/
    static NetworkContextManager& get_instance();

private:
    NetworkContextManager();
    std::mutex lock;
    NetworkContext inner;
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_CONTEXT_H
