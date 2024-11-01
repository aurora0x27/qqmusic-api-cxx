---
hide: navigation
---

<div align="center">
    <h1> QQMusic-API-CXX </h1>
</div>

## 简介

本项目使用C++借助[cURL](https://curl.se/){:target="_blank"}库封装QQ音乐的API.

## 使用的第三方库

- [libcurl](https://github.com/curl/curl){:target="_blank"}

- [JsonCpp](https://github.com/open-source-parsers/jsoncpp){:target="_blank"}

- [zlib](https://github.com/madler/zlib){:target="_blank"}

## 使用方法--如何在项目中使用

设置构建类型

```cmake
set(QQMUSIC_API_BUILD_TYPE SHARED_LIB)
```

作为子项目引入

```cmake
add_subdirectory(path/to/qqmusic-api)
target_include_library(
    demo PUBLIC
    path/to/qqmusic-api/include
)

target_link_library(
    demo PUBLIC
    qmapi
)
```

## 项目架构

可用头文件目录:

```
./include/
└── qqmusic/
    ├── album.h
    ├── api.h
    ├── data.h
    ├── error.h
    ├── login.h
    ├── membership.h
    ├── recommend.h
    ├── search.h
    ├── singer.h
    ├── song.h
    ├── song_lists.h
    └── utils/
        ├── buffer.h
        ├── cache.h
        ├── credential.h
        ├── qimei.h
        ├── qrc_decoder.h
        ├── requests.h
        └── tripledes.h
```

## 参考项目

[luren-dc QQMusicApi](https://github.com/luren-dc/QQMusicApi){:target="_blank"}

## 项目维护者

<a href="https://github.com/aurora0x27/qqmusic-api-cxx/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=aurora0x27/qqmusic-api-cxx" />
</a>
