---
title: Cookie
---

::: doxy.dictionary.namespace.function
    namespace: qqmusic::utils
    name: parse_cookie

## Struct Cookie

```cpp
/**
 * @brief 代表一条Cookie
 * */
struct Cookie {
    /**
     * @brief 域名
     * */
    std::string domain;

    /**
     * @brief 路径
     * */
    std::string path = "/"; /*for each site use '/' as default path*/

    /**
     * @brief 键
     * */
    std::string key;

    /**
     * @brief 值
     * */
    std::string value;
};
```

::: doxy.dictionary.class
    name: qqmusic::utils::CookieJar
