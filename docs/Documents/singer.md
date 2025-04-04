---
title: Singer
---

## Singer

歌手相关Api

```cpp

/**
 * @brief 区域类型枚举
 */
enum class AreaType : int {
    ALL = -100,
    CHINA = 200,
    TAIWAN = 2,
    AMERICA = 5,
    JAPAN = 4,
    KOREA = 3
};

/**
 * @brief 风格类型枚举
 */
enum class GenreType : int {
    ALL = -100,
    POP = 7,
    RAP = 3,
    CHINESE_STYLE = 19,
    ROCK = 4,
    ELECTRONIC = 2,
    FOLK = 8,
    R_AND_B = 11,
    ETHNIC = 37,
    LIGHT_MUSIC = 93,
    JAZZ = 14,
    CLASSICAL = 33,
    COUNTRY = 13,
    BLUES = 10
};

/**
 * @brief 性别类型枚举
 * */
enum class GenderType : int {
    ALL = -100,
    MALE = 0,
    FEMALE = 1,
    GROUP = 2,
};

/**
 * @brief 索引类型枚举, 按首字母筛选时需要
 * */
enum class IndexType : int {
    A = 1,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    HASH,
    ALL = -100
};
```

::: doxy.dictionary.class
    name: qqmusic::TabType

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_singer_list

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_songs_list_all

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_singer_list_index

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_singer_list_index_all

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_singer_info

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_singer_desc

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_tab_detail

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_similar_singers

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_songs_list

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_songs_list_all

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_album_list

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_album_list_all

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_mv_list

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_mv_list_all
