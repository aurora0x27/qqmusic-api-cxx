---
hide: navigation
---

<div align="center">
    <h1> QQMusic-API-CXX </h1>
</div>

## 简介

本项目使用C++借助[Boost Beast](https://www.boost.org/doc/libs/1_85_0/doc/html/boost_asio.html){:target="_blank"}库封装QQ音乐的API.

## 使用的第三方库

- [Boost Asio](https://www.boost.org/doc/libs/1_85_0/libs/beast/doc/html/index.html){:target="_blank"}

- [Boost Beast](https://www.boost.org/doc/libs/1_85_0/libs/beast/doc/html/index.html){:target="_blank"}

- [Boost Uuid](https://www.boost.org/doc/libs/1_73_0/libs/uuid/doc/uuid.html){:target="_blank"}

- [Boost Url](https://www.boost.org/doc/libs/1_85_0/libs/url/doc/html/index.html){:target="_blank"}

- [Nlohmann Json](https://json.nlohmann.me/){:target="_blank"}

- [zlib](https://github.com/madler/zlib){:target="_blank"}

- [Botan](https://botan.randombit.net/){:target="_blank"}

- [OpenSSL](https://openssl-library.org/){:target="_blank"}

## 使用方法--如何在项目中使用

设置构建类型

```cmake
set(QQMUSIC_API_BUILD_TYPE SharedLib)
```

作为子项目引入

```cmake
add_subdirectory(path/to/qqmusic-api)

target_include_library(
    demo PRIVATE
    path/to/qqmusic-api/include
)

target_link_library(
    demo PRIVATE
    qmapi
)
```

## 项目架构

可用头文件目录:

```
./include
└── qqmusic
    ├── crypto
    │   ├── mflac.h
    │   ├── mgg.h
    │   ├── qmcflac.h
    │   ├── qmc.h
    │   ├── qmcogg.h
    │   ├── tkm.h
    │   └── tm.h
    ├── details
    │   ├── api.h
    │   ├── context.h
    │   ├── result.h
    │   └── tripledes.h
    ├── utils
    │   ├── async-executor.h
    │   ├── buffer.h
    │   ├── common.h
    │   ├── cookie.h
    │   ├── credential.h
    │   ├── device.h
    │   ├── exception.h
    │   ├── paths.h
    │   ├── qimei.h
    │   └── session.h
    ├── album.h
    ├── login.h
    ├── result.h
    ├── search.h
    ├── singer.h
    ├── song.h
    ├── song_lists.h
    └── user.h
```

## 参考项目

[luren-dc QQMusicApi](https://github.com/luren-dc/QQMusicApi){:target="_blank"}

[Unlock Music](https://git.unlock-music.dev/um/web){:target="_blank"}

## 项目维护者

<a href="https://github.com/aurora0x27/qqmusic-api-cxx/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=aurora0x27/qqmusic-api-cxx" />
</a>
