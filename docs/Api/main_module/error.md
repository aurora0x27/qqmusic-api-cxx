# error 全局报错码

`qqmusic-api-cxx`中所有的标准API函数均返回`qqmusic::result`枚举类, 可以根据该错误码处理API异常

相关头文件: `qqmusic/error.h`

## 各成员介绍

### 全局错误类型

- `qqmusic::result::excecuted_success` API函数操作正常完成

- `qqmusic::result::mem_alloc_error` 内存分配错误

- `qqmusic::result::data_destroy` 数据损毁

- `qqmusic::result::unknown_error` 未知错误

### 网络部分报错

- `qqmusic::result::network_connect_timeout` 请求超时
