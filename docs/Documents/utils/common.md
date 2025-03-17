# Common

一组工具函数

## ***fn*** sign

对请求参数进行签名

```cpp
std::string sign(const nlohmann::json& params);
```

## ***fn*** to_buffer

将原始的请求转换成通用buffer

```cpp
buffer resp2buf(http::response<http::dynamic_body>&& resp);
```

## ***fn*** hex2buf

将十六进制的字符串转换成二进制buffer

```cpp
buffer hex2buf(std::string_view hex);
```

## ***fn*** qrc_decode

解码加密的QRC歌词

!!! note "枚举值cloud和local"

    其中local与cloud的区别是local需要进行qmc1解码, 并丢弃前11个字节, cloud是直接处理

```cpp
enum class qrc_type { cloud, local };

qqmusic::Result<std::string> qrc_decode(const qqmusic::utils::buffer& src,
                                        qqmusic::utils::qrc_type type);
```

## ***fn*** hash33

求utf-8字符串的哈希值, prev是多次求取的时候上一次计算的结果.

```cpp
uint64_t hash33(std::string_view str, uint64_t prev = 0);
```
