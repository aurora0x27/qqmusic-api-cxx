---
title: Lyric
---

歌词相关Api

```cpp
/**
 * @struct Lyric
 *
 * @brief 歌词
 * */
struct Lyric {
    std::string lyric; ///< 原始歌词
    std::string trans; ///< 翻译
    std::string roma;  ///< 罗马音
};
```

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task< Result < Lyric > > get_lyric(std::string_view mid, bool qrc=false, bool trans=false, bool roma=false)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task< Result < Lyric > > get_lyric(uint64_t id, bool qrc=false, bool trans=false, bool roma=false)
