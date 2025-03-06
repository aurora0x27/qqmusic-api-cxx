---
title: Exception
---

收集Api可能产生的异常

## ***class*** Exception

### ***enum*** Kind

- ResponseCodeError  

    回复的状态码异常

- CredendialExpiredError

    凭证过期

- CredentialInvalidError

    凭证不合法

- LoginError

    登陆错误

- SignInvalidError

    请求签名错误

- DataDestroy

    加密, 解码, 序列化, 反序列化过程中数据损毁, 或得到未预期的数据

- RuntimeError

    运行时异常(包括拦截第三方库的一般异常)

- NetworkError

    一般网络异常

- JsonError

    Json的生成和解析时出现错误

- OperationOutOfTime

    操作超时

- SslError

    SSL/TLS操作异常

- UnknownError

    未知错误

### ***fn*** Exception

- 从枚举构造

```cpp
Exception(Kind kind)
    : kind_code(kind)
    , reason(ReasonKindMap[kind]) {}
```

- 从枚举构造, 并附上原因

```cpp
Exception(Kind kind, std::string_view reason)
    : kind_code(kind)
    , reason(reason) {}

```

- 从http返回的状态码构造

```cpp
Exception(unsigned int http_rc)
    : kind_code(ResponseCodeError) {
    if (HttpResponseCodeMap.contains(http_rc)) {
        reason = HttpResponseCodeMap[http_rc];
    } else {
        reason = ReasonKindMap[Kind::ResponseCodeError];
    }
}
```

### ***fn*** get_error_enum

获取枚举值

```cpp
[[nodiscard]] unsigned int get_error_enum() const { return kind_code; }
```

### ***fn*** kind

获取字符串形式的错误类型

```cpp
[[nodiscard]] std::string kind() const { return ReasonKindMap[kind_code]; }
```

### ***fn*** what

获取错误原因

```cpp
[[nodiscard]] std::string what() const { return reason; }
```
