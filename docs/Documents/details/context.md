---
title: Context
---

上下文管理

## ***struct*** ApiConfig

- enable_sign: 是否需要签名

- endpoint: 未加密接口

- enc_endpoint: 加密接口

```cpp
struct ApiConfig {
    std::string version = VERSION;
    uint64_t version_code = VERSION_CODE;
    bool enable_sign = false;
    std::string endpoint = "https://u.y.qq.com/cgi-bin/musicu.fcg";
    std::string enc_endpoint = "https://u.y.qq.com/cgi-bin/musics.fcg";
};
```

## ***struct*** NetworkContext

### ***attr*** cookie

Cookie

```cpp
qqmusic::utils::CookieJar cookies;
```

### ***attr*** credential

凭证

```cpp
qqmusic::utils::Credential credential;
```

### ***attr*** api_config

常量和公用参数

```cpp
ApiConfig api_config;
```

### ***attr*** device

设备信息

```cpp
qqmusic::utils::Device device;
```

### ***attr*** qimei

设备识别码

```cpp
qqmusic::utils::QimeiResult qimei;
```

### ***attr*** verify

是否需要认证

```cpp
bool verify;
```

### ***attr*** ignore_ssl_error

是否忽略SSL错误

```cpp
bool ignore_ssl_error;
```

### ***attr*** timeout

超时时限

```cpp
std::chrono::seconds timeout;
```
