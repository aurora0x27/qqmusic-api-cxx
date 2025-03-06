---
title: Buffer
---

`qqmusic::utils::buffer` 提供了二进制缓冲区的通用接口.

相关头文件: `qqmusic/utils/buffer.h`

## ***class*** buffer

### ***fn*** append

```cpp
void append(const void* src_data_buf, size_t data_buf_size);
```

将内存段拷贝到本段buffer的最后

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
