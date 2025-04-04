---
title: Search
---

搜索相关Api

```cpp
/**
 * @brief 搜索类型枚举
 * */
enum class SearchType : unsigned {
    SONG = 0,         ///< 歌曲
    SINGER = 1,       ///< 歌手
    ALBUM = 2,        ///< 专辑
    SONG_LIST = 3,    ///< 歌单
    MV = 4,           ///< MV
    LYRIC = 7,        ///< 歌词
    USER = 8,         ///< 用户
    AUDIO_ALBUM = 15, ///< 节目专辑
    AUDIO = 18,       ///< 节目
};
```

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: hotkey

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: keyword_complete

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: quick_search

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: general_search

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: search_by_type
