/**----------------------------------qqmusic/details/context.h--------------------------------------
 *
 * @file qqmusic/details/context.h
 *
 * @brief Api上下文定义
 *
 * @date 2025-3-25
 *
 *------------------------------------------------------------------------------------------------*/
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

/**
 * @brief Api版本号
 * */
const char VERSION[] = "13.2.5.8";

/**
 * @brief 版本代码
 * */
const uint64_t VERSION_CODE = 13020508;

/**
 * @brief Api相关常量
 * */
struct ApiConfig {
    std::string version = VERSION;
    uint64_t version_code = VERSION_CODE;

    /**
     * @brief 请求参数是否需要签名
     * */
    bool enable_sign = false;

    /**
     * @brief 未加密接口url
     * */
    std::string endpoint = "https://u.y.qq.com/cgi-bin/musicu.fcg";

    /**
     * @brief 加密接口url
     * */
    std::string enc_endpoint = "https://u.y.qq.com/cgi-bin/musics.fcg";
};

/**
 * @brief 打包的上下文
 * */
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
