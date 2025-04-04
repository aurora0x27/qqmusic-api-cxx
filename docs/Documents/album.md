---
title: Album
---

专辑相关Api

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: get_album_cover_url

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task< Result < nlohmann::json > > get_album_detail(std::string_view album_mid)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task< Result < nlohmann::json > > get_album_detail(uint64_t album_id)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task< Result < nlohmann::json > > get_album_songs(std::string_view album_mid, unsigned int num=10, unsigned int page=1)

::: doxy.dictionary.namespace.function
    namespace: qqmusic
    name: Task< Result < nlohmann::json > > get_album_songs(uint64_t album_id, unsigned int num=10, unsigned int page=1)
