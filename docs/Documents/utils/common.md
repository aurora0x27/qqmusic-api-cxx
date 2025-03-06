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
buffer to_buffer(http::response<http::dynamic_body>&& resp);
```

## ***fn*** qrc_decode

解码加密的QRC歌词

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
