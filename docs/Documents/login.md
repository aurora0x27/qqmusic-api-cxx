---
title: Login
---

登陆相关Api

```cpp
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
 * @brief 二维码登陆结果封装
 * */
struct QRCodeLoginResult {
    QRCodeLoginEvent status{QRCodeLoginEvent::Status::OTHER};
    std::optional<utils::Credential> credential;
};
```

::: doxy.dictionary.class
    name: qqmusic::QRCodeLoginEvent

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_qrcode

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: check_qq_qr

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: check_wx_qr

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: send_authcode

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: phone_authorize
