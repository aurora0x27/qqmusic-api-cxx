---
title: Mv
---

MV相关Api

```cpp
/**
 * @struct MvUrl
 *
 * @brief Mv url封裝
 * */
struct MvUrl {
    std::string vid;                        ///< Mv的vid
    std::map<std::string, std::string> mp4; ///< mp4格式url
    std::map<std::string, std::string> hls; ///< hls格式url
};
```

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_mv_detail

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_mv_urls
