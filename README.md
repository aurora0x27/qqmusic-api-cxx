```
 $$$$$$\   $$$$$$\  $$\      $$\ $$\   $$\  $$$$$$\  $$$$$$\  $$$$$$\      $$$$$$\  $$$$$$$\  $$$$$$\ 
$$  __$$\ $$  __$$\ $$$\    $$$ |$$ |  $$ |$$  __$$\ \_$$  _|$$  __$$\    $$  __$$\ $$  __$$\ \_$$  _|
$$ /  $$ |$$ /  $$ |$$$$\  $$$$ |$$ |  $$ |$$ /  \__|  $$ |  $$ /  \__|   $$ /  $$ |$$ |  $$ |  $$ |  
$$ |  $$ |$$ |  $$ |$$\$$\$$ $$ |$$ |  $$ |\$$$$$$\    $$ |  $$ |         $$$$$$$$ |$$$$$$$  |  $$ |  
$$ |  $$ |$$ |  $$ |$$ \$$$  $$ |$$ |  $$ | \____$$\   $$ |  $$ |         $$  __$$ |$$  ____/   $$ |  
$$ $$\$$ |$$ $$\$$ |$$ |\$  /$$ |$$ |  $$ |$$\   $$ |  $$ |  $$ |  $$\    $$ |  $$ |$$ |        $$ |  
\$$$$$$ / \$$$$$$ / $$ | \_/ $$ |\$$$$$$  |\$$$$$$  |$$$$$$\ \$$$$$$  |   $$ |  $$ |$$ |      $$$$$$\ 
 \___$$$\  \___$$$\ \__|     \__| \______/  \______/ \______| \______/    \__|  \__|\__|      \______|
     \___|     \___|                                        
                                                            
                                                            
                                    $$$$$$\  $$\   $$\ $$\   $$\ 
                                   $$  __$$\ $$ |  $$ |$$ |  $$ |
                                   $$ /  \__|\$$\ $$  |\$$\ $$  |
                                   $$ |       \$$$$  /  \$$$$  / 
                                   $$ |       $$  $$<   $$  $$<  
                                   $$ |  $$\ $$  /\$$\ $$  /\$$\ 
                                   \$$$$$$  |$$ /  $$ |$$ /  $$ |
                                    \______/ \__|  \__|\__|  \__|
```

这是使用C++进行封装的部分QQMusic API, 被用于项目[qqmusic-cli](https://www.github.com/aurora0x27/qqmusic-cli)

使用详见[项目文档](https://aurora0x27.github.io/qqmusic-api-cxx/)

## Brief introduction

基于项目[QQMusicApi](https://github.com/luren-dc/QQMusicApi), 使用现代C++编写的QQ音乐Api接口:

- 使用`Boost Asio`提供的协程进行异步操作

- 使用[`Result.h`](https://github.com/oktal/result)提供统一的报错机制, 禁用异常

- 统一使用`Nlohmann Json`提供的`Json`数据结构作为返回值, 便于上层应用程序封装

## Dependencies

- [Boost Asio](https://www.boost.org/doc/libs/1_85_0/libs/beast/doc/html/index.html)

- [Boost Beast](https://www.boost.org/doc/libs/1_85_0/libs/beast/doc/html/index.html)

- [Boost Uuid](https://www.boost.org/doc/libs/1_73_0/libs/uuid/doc/uuid.html)

- [Boost Url](https://www.boost.org/doc/libs/1_85_0/libs/url/doc/html/index.html)

- [Nlohmann Json](https://json.nlohmann.me/)

- [zlib](https://github.com/madler/zlib)

- [Botan](https://botan.randombit.net/)

- [OpenSSL](https://openssl-library.org/)

## Download

> [!Important]
>
> 正常使用请从`main`分支下载, 如要获取最新的更改, 请切换到`dev`分支

## Build

支持的编译器版本: `gcc >= 13`, `clang >= 19`, `msvc`

### CMake

- 构建示例程序

```bash
cmake -B build -DQQMUSIC_API_BUILD_TYPE=Demo

cmake --build build
```

- 构建动态库

```bash
cmake -B build -DQQMUSIC_API_BUILD_TYPE=SharedLib

cmake --build build
```

- 构建测试

```bash
cmake -B build -DQQMUSIC_API_BUILD_TYPE=Test

cmake --build build
```

### xmake

> **Support Linux Only By Now**

- 构建示例

```bash
xmake build -y demo
```

- 构建动态库

```bash
xmake build -y qmapi
```

- 构建测试

```bash
xmake build -y test
```
