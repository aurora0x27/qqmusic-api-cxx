---
title: Cookie
---

简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖`nlohmann::json`实现, 目前不支持路径嵌套和`Expires`和`HttpOnly`这样的特性

## ***strut*** Cookie

Cookie类, 描述一条Cookie记录

```cpp
struct Cookie {
    std::string domain;     /*域名*/
    std::string path = "/"; /*路径*/
    std::string key;        /*键*/
    std::string value;      /*值*/
};
```

## ***fn*** parse_cookie

将原始的Cookie字符串转成键值对的形式

```cpp
qqmusic::Result<nlohmann::json> parse_cookie(std::string_view cookie_str);
```

## ***class*** CookieJar

Cookie管理器类

### ***fn*** CookieJar

- 默认构造

```cpp
CookieJar()
    : content({{"", {{"/", nlohmann::json::object()}}}}){};
```

- 通过缓存的Json构造

```cpp
CookieJar(std::string_view cache)
    : content(nlohmann::json::parse(cache)){};
```

- 通过某一域名的Cookie解析出的字符串构造

```cpp
CookieJar(std::string_view cookie_str, std::string_view domain, std::string_view path = "/");
```

### ***fn*** set

插入一条Cookie

!!! note "Note"

    若该键存在, 则新值覆盖旧值

```cpp
qqmusic::Result<void> set(const Cookie& cookie);
```

### ***fn*** get

根据键, 域名和路径来查询Cookie, 域名和路径可缺省

!!! info "Info"

    若缺省, 则返回第一个遇到的相同键的值

```cpp
qqmusic::Result<std::string> get(std::string_view key,
                                 std::optional<std::string> domain = std::nullopt,
                                 std::optional<std::string> path = std::nullopt);
```

### ***fn*** del

根据键, 域名和路径删除Cookie条目

!!! info "Info"

    缺省域名则删除所有Cookie, 缺省路径则删除给定域名下的所有Cookie

### ***fn*** dump

输出所有Cookie条目, 便于长期储存

```cpp
qqmusic::Result<std::string> dump();
```

输出的Cookie格式如下所示

```json
{
    "domain1" : {
        "path1": {
            "key1": "value1",
            "key2": "value2"
        },
        "path2": {
            "key1": "value1",
            "key2": "value2"
        }
    },
    "domain2" : {
        "path1": {
            "key1": "value1",
            "key2": "value2"
        },
        "path2": {
            "key1": "value1",
            "key2": "value2"
        }
    }
}
```

### ***fn*** serialize

将给定范围的Cookie序列化成Cookie字符串

```cpp
qqmusic::Result<std::string> serialize(std::string_view domain, std::string_view path = "/");
```

### ***fn*** clear

清除制定的Cookie条目

```cpp
qqmusic::Result<void> clear(std::optional<std::string> domain = std::nullopt,
                            std::optional<std::string> path = std::nullopt,
                            std::optional<std::string> key = std::nullopt);
```

### ***fn*** merge

将两个Cookie进行合并

!!! info "Info"

    如果有出现键值冲突, 则传入的Cookie的值替换本身的Cookie的值

```cpp
qqmusic::Result<void> merge(const CookieJar& cookies);
```
