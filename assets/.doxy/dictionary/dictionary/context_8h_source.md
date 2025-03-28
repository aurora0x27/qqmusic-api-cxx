

# File context.h

[**File List**](files.md) **>** [**details**](dir_6e6556f41d7f85330fbda245682a6a39.md) **>** [**context.h**](context_8h.md)

[Go to the documentation of this file](context_8h.md)


```C++

#ifndef QQMUSIC_DETAILS_CONTEXT_H
#define QQMUSIC_DETAILS_CONTEXT_H

#include <chrono>
#include <format>
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

struct NetworkContext {
    /*Globally shared vars*/
    qqmusic::utils::CookieJar cookies;
    qqmusic::utils::Credential credential;
    ApiConfig api_config;
    qqmusic::utils::Device device;
    qqmusic::utils::QimeiResult qimei;
    bool ignore_ssl_error;
    std::chrono::seconds timeout;

    /*constructor*/
    NetworkContext()
        : ignore_ssl_error(false)
        , timeout(20) {
        api_config = ApiConfig();
        auto device_res = qqmusic::utils::get_device_info();
        if (device_res.isErr()) {
            throw std::runtime_error(
                std::format("[NetworkContext::NetworkContext] -- Cannot get device: {}",
                            device_res.unwrapErr().what()));
        }
        device = device_res.unwrap();
        auto qimei_res = get_qimei(device, api_config.version);
        if (qimei_res.isErr()) {
            throw std::runtime_error(
                std::format("[NetworkContext::NetworkContext] -- Cannot get qimei: {}",
                            qimei_res.unwrapErr().what()));
        }
        qimei = qimei_res.unwrap();
        device.qimei = qimei.q36;
        qqmusic::utils::cache_device(device);
    }
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_CONTEXT_H
```


