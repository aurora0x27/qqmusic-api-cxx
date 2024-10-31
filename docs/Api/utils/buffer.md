# buffer 缓冲区

`qqmusic::utils::buffer`提供了二进制缓冲区类, 以及用于curl库写缓冲区的回调函数

相关头文件: `qqmusic/utils/buffer.h`

## ***class*** `buffer`

二进制缓冲区类

### ***constructor***

构造函数, 共3个重载

```cpp
qqmusic::utils::buffer();
```

默认构造函数, 初始化空的缓冲区

```cpp
qqmusic::utils::buffer(size_t size);
```

预先分配长度为`size`的缓冲区, 内容全为`\0`

```cpp
qqmusic::utils::buffer(const uint8_t* src_head, size_t size);
```

将开头为`src_head`, 长度为`size`的内存拷贝到新创建的缓冲区内

### ***func*** `get_size`

```cpp
size_t
qqmusic::utils::buffer::get_size() const;
```

获取当前缓冲区的长度, 返回`size_t`类型

### ***func*** `get_head`

```cpp
uint8_t*
qqmusic::utils::buffer::get_head() const;
```

返回`uint8* const`类型的缓冲区开头地址, 不可更改.

### ***func*** `append`

```cpp
size_t
qqmusic::utils::buffer::append(const void* src_data_buf,
                               size_t data_buf_size);
```

将`src_data_buf`开头的`data_buf_size`字节数据插入缓冲区后面.

如果成功操作, 就返回插入的数据大小, 否则返回`-1`

### ***func*** `clear`

```cpp
void clear();
```

将缓冲区内容清除

### ***func*** `resize`

```cpp
bool resize(size_t size)
```

将缓冲区大小设置为`size`, 如果成功就返回`false`, 否则返回`true`

## ***func*** `http_writebuf`

```cpp
size_t http_writebuf(void* src_data_buf, size_t data_buf_size, size_t data_buf_num, void* buffer_ptr)
```

`http_writebuf`函数是为与`libcurl`交互设计的回调函数.

使用案例

```cpp
#include <fstream>
#include <string>

#include <curl/curl.h>
#include <utils/buffer.h>

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("Usage -- %s url output_file_name\n", argv[0]);
        return 1;
    }
    std::string url = std::string(argv[1]);
    std::string output_file_name = std::string(argv[2]);
    utils::buffer buf;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* handle = curl_easy_init();


    // set handler options
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    // set http_writebuf as CURLOPT_WRITEFUNCTION
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, utils::http_writebuf);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) "
                                                "AppleWebKit/537.36 (KHTML, like Gecko) "
                                                "Chrome/122.0.0.0 Safari/537.36");

    // pass the address of buffer object to CURLOPT_WRITEDATA
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buf);
    curl_easy_perform(handle);

    curl_easy_cleanup(handle);
    curl_global_cleanup();

    std::fstream output_file;
    output_file.open(output_file_name, std::ios::out);
    output_file.write((char*)buf.get_head(), buf.get_size());
    output_file.close();
    return 0;
}
```
