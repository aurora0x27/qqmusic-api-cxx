---
title: Song
---

## Song

歌曲相关Api

::: doxy.dictionary.class
    name: qqmusic::BaseMediaFileType

::: doxy.dictionary.class
    name: qqmusic::SongFileType

::: doxy.dictionary.class
    name: qqmusic::EncryptedSongFileType

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: query_song

```cpp
/**
 * @struct SongInfo
 *
 * @var SongInfo::mid 歌曲mid
 * @var SongInfo url 歌曲文件地址, 如果没有则是`std::nullopt`
 * @var SongInfo ekey 歌曲密钥, 如果没有则是`std::nullopt`
 * */
struct SongInfo {
    std::string mid;
    std::optional<std::string> url = std::nullopt;
    std::optional<std::string> ekey = std::nullopt;
};
```

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_song_urls

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_try_url

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task < Result < nlohmann::json > > get_song_detail(std::string_view mid)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task < Result < nlohmann::json > > get_song_detail(uint64_t id)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_similar_songs

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_labels

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_related_songlist

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_related_mv

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task < Result < nlohmann::json > > get_other_version(std::string_view mid)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task < Result < nlohmann::json > > get_other_version(uint64_t id)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task < Result < nlohmann::json > > get_producer_info(std::string_view mid)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task < Result < nlohmann::json > > get_producer_info(uint64_t id)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_sheet

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_fav_num
