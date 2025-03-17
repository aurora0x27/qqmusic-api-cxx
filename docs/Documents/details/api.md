---
title: Api
---

负责Api请求构造

## ***struct*** RequestParam

打包的请求参数

```cpp
struct RequestParam {
    boost::url url;
    http::request<http::string_body> req;
};
```

## ***class*** Api

Api类, 用于快速构造请求

### ***fn*** Api

```cpp
Api(utils::Session& session,
    std::string module,
    std::string method,
    utils::Credential credential = {},
    nlohmann::json common = {{}})
    : session(session)
    , module(std::move(module))
    , method(std::move(method)){};
```

### ***fn*** prepare_request

根据请求参数构造请求

```cpp
qqmusic::Task<qqmusic::Result<RequestParam>> prepare_request(const nlohmann::json& params);
```

!!! tip "Cookie"

    由`prepare_request`函数构造的请求是不带Cookie的, 若需要, 自行添加.

### ***fn*** parse_response

将回复报文解析成Json对象

```cpp
qqmusic::Result<nlohmann::json> parse_response(utils::buffer&& response);
```
