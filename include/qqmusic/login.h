/**-------------------------------------qqmusic/login.h---------------------------------------------
 *
 * @file include/qqmusic/login.h
 *
 * @brief 登陆相关Api, 包含三种登陆方式 -- QQ扫码, 微信扫码, 手机号登陆
 *
 * @warning 手机号登陆的Api仍然有问题, 不能使用.
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_LOGIN_H
#define QQMUSIC_LOGIN_H

#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic {

/**
 * @brief 登陆类型枚举
 * */
enum class QRLoginType { QQ, WX };

/**
 * @brief 手机号登陆状态枚举
 * */
enum class PhoneLoginEvent : int { 
    SEND = 0,           ///< 已发送
    CAPTCHA = 20276,    ///< 需要滑块验证
    FREQUENCY = 100001, ///< 操作过于频繁
    OTHER = -1          ///< 其他
};

/**
 * @struct PhoneLoginEvent
 *
 * @brief 手机号登陆事件包装器
 * */
struct PhoneLoginResult {
    PhoneLoginEvent event;
    std::string msg;
};

/**
 * @struct QRCode
 *
 * @brief 二维码信息
 * */
struct QRCode {
    QRLoginType qr_type;         ///< qr code type
    std::string identifier;      ///< qr code identifier
    std::string mimie_type;      ///< qr code image type
    qqmusic::utils::buffer data; ///< qr code image binary data
};

/**
 * @brief 二维码登陆事件
 * */
class QRCodeLoginEvent {
public:

    /**
     * @brief 二维码状态
     * */
    enum class Status { 
        DONE,    ///< 认证完毕
        SCAN,    ///< 待扫描
        CONF,    ///< 正在认证
        REFUSE,  ///< 拒绝登陆
        TIMEOUT, ///< 超时
        OTHER    ///< 其他
    };

    /**
     * @brief 从状态构造
     * */
    QRCodeLoginEvent(Status stat) { status = stat; }

    /**
     * @brief 从Http请求返回的状态码构造
     * */
    QRCodeLoginEvent(unsigned int code) {
        switch (code) {
        case 0:
        case 405:
            status = Status::DONE;
            break;
        case 66:
        case 408:
            status = Status::SCAN;
            break;
        case 67:
        case 404:
            status = Status::CONF;
            break;
        case 65:
            status = Status::TIMEOUT;
            break;
        case 68:
        case 403:
            status = Status::REFUSE;
            break;
        default:
            status = Status::OTHER;
        }
    };

    /**
     * @brief 状态是否相同
     * */
    inline bool operator==(const QRCodeLoginEvent& e) { return this->status == e.status; }

    /**
     * @brief 状态是否不相同
     * */
    inline bool operator!=(const QRCodeLoginEvent& e) { return this->status != e.status; }

    /**
     * @brief 和枚举直接比较是否相同
     * */
    inline bool operator==(Status stat) { return status == stat; }

    /**
     * @brief 和枚举直接比较是否不相同
     * */
    inline bool operator!=(Status stat) { return status != stat; }

    /**
     * @brief 获取当前状态的名字
     * */
    inline std::string_view name() const { return name_map.at(status); }

private:
    Status status;
    inline static const std::map<Status, std::string> name_map = {
        {Status::DONE, "DONE"},
        {Status::SCAN, "SCAN"},
        {Status::CONF, "CONF"},
        {Status::TIMEOUT, "TIMEOUT"},
        {Status::REFUSE, "REFUSE"},
        {Status::OTHER, "OTHER"},
    };
};

/**
 * @brief 二维码登陆结果封装
 * */
struct QRCodeLoginResult {
    QRCodeLoginEvent status{QRCodeLoginEvent::Status::OTHER};
    std::optional<utils::Credential> credential;
};

/**
 * @brief 获取登陆二维码
 *
 * @param login_type 登陆类型
 *
 * @return 包含正确结果的`Task<Result<QRCode>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
qqmusic::Task<qqmusic::Result<QRCode>> get_qrcode(QRLoginType login_type);

/**
 * @brief 检查QQ二维码状态
 *
 * @param qrc QQ二维码
 *
 * @return 包含正确结果的`Task<Result<QRCodeLoginResult>>`.
 *         如果登陆成功会包含Credential
 *
 * @note 异步函数, 返回`Task`
 * */
qqmusic::Task<qqmusic::Result<QRCodeLoginResult>> check_qq_qr(QRCode& qrc);

/**
 * @brief 检查微信二维码状态
 *
 * @param qrc 微信二维码
 *
 * @return 包含正确结果的`Task<Result<QRCodeLoginResult>>`.
 *         如果登陆成功会包含Credential
 *
 * @note 异步函数, 返回`Task`
 * */
qqmusic::Task<qqmusic::Result<QRCodeLoginResult>> check_wx_qr(QRCode& qrc);

/**
 * @brief 向指定手机号发送验证码
 *
 * @param phone 手机号
 * @param country_code 地区前缀
 *
 * @return `qqmusic::Task<qqmusic::Result<PhoneLoginResult>>`
 *         出错就会返回`Err`
 *
 * @note 异步函数, 返回`Task`
 * */
qqmusic::Task<qqmusic::Result<PhoneLoginResult>> send_authcode(
    std::string_view phone, std::string_view country_code = "86");

/**
 * @brief 手机号认证
 *
 * @param phone 手机号
 * @param auth_code 验证码
 * @param country_code 地区前缀
 *
 * @return `qqmusic::Task<qqmusic::Result<qqmusic::utils::Credential>>`
 *         出错就会返回`Err`
 *
 * @note 异步函数, 返回`Task`
 * */
qqmusic::Task<qqmusic::Result<utils::Credential>> phone_authorize(
    std::string_view phone, std::string_view auth_code, std::string_view country_code = "86");

} // namespace qqmusic

#endif // !QQMUSIC_LOGIN_H
