---
hide: navigation
---

<div align="center">
    <h1> QQMusic-API-CXX </h1>
</div>

## 简介

本项目使用C++借助[Boost Beast](https://www.boost.org/doc/libs/1_85_0/libs/beast/doc/html/index.html){:target="_blank"}库封装QQ音乐的API.

## 使用的第三方库

- [Boost Beast](https://www.boost.org/doc/libs/1_85_0/libs/beast/doc/html/index.html){:target="_blank"}

- [Nlohmann Json](https://json.nlohmann.me/){:target="_blank"}

- [zlib](https://github.com/madler/zlib){:target="_blank"}

- [Botan](https://botan.randombit.net/){:target="_blank"}

## 使用方法--如何在项目中使用

设置构建类型

```cmake
set(QQMUSIC_API_BUILD_TYPE SharedLib)
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
    │   ├── device.h
    │   ├── network.h
    │   ├── qimei.h
    │   ├── result.h
    │   └── tripledes.h
    ├── utils               
    │  ├── async-executor.h
    │  ├── buffer.h        
    │  ├── cache.h         
    │  ├── credential.h    
    │  ├── exception.h     
    │  ├── qrc-decoder.h   
    │  └── requests.h      
    ├── album.h
    ├── login.h
    ├── membership.h
    ├── recommend.h
    ├── result.h
    ├── search.h
    ├── singer.h
    ├── song.h
    └── song_lists.h
```

## 参考项目

[luren-dc QQMusicApi](https://github.com/luren-dc/QQMusicApi){:target="_blank"}

## 项目维护者

<a href="https://github.com/aurora0x27/qqmusic-api-cxx/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=aurora0x27/qqmusic-api-cxx" />
</a>
