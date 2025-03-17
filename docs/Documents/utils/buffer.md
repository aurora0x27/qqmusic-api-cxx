---
title: Buffer
---

`qqmusic::utils::buffer` 提供了二进制缓冲区的通用接口.

相关头文件: `qqmusic/utils/buffer.h`

## ***class*** buffer

### ***fn*** buffer

- 通过size构造

```cpp
buffer(size_t size)
    : std::vector<uint8_t>(size){};
```

- 通过裸指针指向的缓冲区构造

```cpp
buffer(const uint8_t* src_head, size_t src_size)
    : std::vector<uint8_t>(src_head, src_head + src_size){};
```

- 从内存块进行移动构造

```cpp
buffer(std::vector<uint8_t>&& other) noexcept
    : std::vector<uint8_t>(std::move(other)) {}
```

- 移动构造

```cpp
buffer(buffer&& other) noexcept
    : std::vector<uint8_t>(std::move(other)) {}
```

- 给定字符填充

```cpp
buffer(size_t size, char c)
    : std::vector<uint8_t>(size, c) {}
```

### ***fn*** append

```cpp
void append(const void* src_data_buf, size_t data_buf_size);
```

将内存段拷贝到本段buffer的最后

### ***opt*** =

使用默认的赋值预算符作为拷贝构造

```cpp
buffer& operator=(const buffer& other) = default;
```

### ***opt*** +

```cpp
qqmusic::utils::buffer a, b;
auto c = a + b;
```

拼接两段buffer

### ***opt*** +=

```cpp
qqmusic::utils::buffer a, b;
a += b;
```

将b缓冲区拼接到a后面
