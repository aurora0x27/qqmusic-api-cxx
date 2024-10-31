# QRC Decoder

QQMusic的歌词是经过加密的`*.qrc`格式文件, 需要进行解码才能解析使用.

相关头文件: `qqmusic/utils/qrc_decode.h`

## ***function*** `qrc_decoder`

将缓冲区`src`中的内容进行解码, 并传入缓冲区`dest`中.

- 函数原型

```cpp
qqmusic::result qqmusic::utils::qrc_decode(qqmusic::utils::buffer*  src,
                                           qqmusic::utils::buffer*  dest,
                                           qqmusic::utils::qrc_type type);
```

- 参数介绍

该函数接受两个`qqmusic::utils::buffer`指针做参数, 分别是解码前后的缓冲区, 以及一个代表qrc文件类型的枚举类, 定义如下:

```cpp
enum class qrc_type
{
    cloud,
    local
};
```

- 返回值

其返回值可能是:

```
// 正常解码
qqmusic::result::excecuted_success;

// 内存分配错误
qqmusic::result::mem_alloc_error;

// 数据损毁
qqmusic::result::data_destroy;

// 未知错误
qqmusic::result::unknown_error;
```

- 注意

传入和传出的都是二进制的缓冲区, 可以通过下面的方法将buffer中的内容转换成`std::string`格式的歌词

```cpp
std::string lyric = std::string(dest->get_head(), dest->get_size());
```
