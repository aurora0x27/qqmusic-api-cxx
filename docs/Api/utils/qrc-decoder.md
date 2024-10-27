# QRC Decoder

QQMusic的歌词是经过加密的`*.qrc`格式文件, 需要进行解码才能解析使用.

头文件: `#include <qqmusic/utils/qrcdecode.h>`

## 函数原型

```cpp
qqmusic::result qqmusic::utils::qrc_decode(qqmusic::utils::buffer*  src,
                                           qqmusic::utils::buffer*  dest,
                                           qqmusic::utils::qrc_type type);
```

该函数接受两个`qqmusic::utils::buffer`指针做参数, 分别是解码前后的缓冲区, 以及一个代表qrc文件类型的枚举类, 定义如下:

```cpp
enum class qrc_type
{
    cloud,
    local
};
```

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

通过下面的方法将buffer中的内容转换成`xml`字符串格式的歌词

```cpp
std::string lyric = std::string(dest->get_head(), dest->get_size());
```
