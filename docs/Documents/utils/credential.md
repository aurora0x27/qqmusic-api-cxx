---
title: Credential
---

凭证信息

## ***class*** Credential

Credential类描述用户的凭证信息

### ***attr*** openid

```cpp
std::string openid;
```

### ***attr*** refresh_token

```cpp
std::string refresh_token;
```

### ***attr*** access_token

```cpp
std::string access_token;
```

### ***attr*** expired_at

到期时间: uint64_t格式的时间戳

```cpp
uint64_t expired_at = 0;
```

### ***attr*** musicid

用户ID

```cpp
uint64_t musicid = 0;
```

### ***attr*** musickey

QQMusicKey

```cpp
std::string musickey;
```

### ***attr*** unionid

```cpp
std::string unionid;
```

### ***attr*** str_musicid

字符串形式的用户ID

```cpp
std::string str_musicid;
```

### ***attr*** refresh_key

```cpp
std::string refresh_key;
```

### ***attr*** encryptUin

```cpp
std::string encryptUin;
```

### ***attr*** loginType 

登陆类型: 微信登陆(1), QQ登陆(2)

```cpp
int loginType = 2;
```

### ***attr*** extra_fields

未确定字段

```cpp
nlohmann::json extra_fields;
```

### ***fn*** Credential

- 默认构造

```cpp
Credential() = default;
```

- 从std::string_view格式的字符串创建

```cpp
explicit Credential(std::string_view cookie);
```

- 从Json对象创建

```cpp
explicit Credential(nlohmann::json& cookie);
```

### ***fn*** is_expired

检查Credential是否过期

```cpp
qqmusic::Task<qqmusic::Result<bool>> Credential::is_expired();
```

### ***fn*** refresh

刷新Credential

```cpp
qqmusic::Task<qqmusic::Result<void>> Credential::refresh();
```

### ***fn*** to_string

以Json字符串形式导出Credential

```cpp
qqmusic::Result<std::string> Credential::to_string();
```

### ***fn*** to_json

以Json对象格式导出Credential

```cpp
qqmusic::Result<nlohmann::json> Credential::to_json();
```
