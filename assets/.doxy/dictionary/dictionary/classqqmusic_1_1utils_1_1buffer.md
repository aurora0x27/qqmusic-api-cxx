

# Class qqmusic::utils::buffer



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**buffer**](classqqmusic_1_1utils_1_1buffer.md)



_二进制缓冲区的统一表示_ 

* `#include <buffer.h>`



Inherits the following classes: std::vector< uint8_t >


































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**append**](#function-append) (const void \* src\_data\_buf, size\_t data\_buf\_size) <br>_将一段内存插到这段buffer后面_  |
|   | [**buffer**](#function-buffer-17) () = default<br>_默认初始化_  |
|   | [**buffer**](#function-buffer-27) (size\_t size) <br>_预先分配异丁醇长度_  |
|   | [**buffer**](#function-buffer-37) (const uint8\_t \* src\_head, size\_t src\_size) <br>_直接从内存区域进行构造_  |
|   | [**buffer**](#function-buffer-47) (std::vector&lt; uint8\_t &gt; && other) noexcept<br>_从std::vector&lt;uint8\_t&gt;移动构造_  |
|   | [**buffer**](#function-buffer-57) ([**buffer**](classqqmusic_1_1utils_1_1buffer.md) && other) noexcept<br>_移动构造_  |
|   | [**buffer**](#function-buffer-67) (size\_t size, char c) <br>_初始化为特定字符_  |
|   | [**buffer**](#function-buffer-77) (const [**buffer**](classqqmusic_1_1utils_1_1buffer.md) & other) = default<br>_拷贝构造函数_  |
|  [**buffer**](classqqmusic_1_1utils_1_1buffer.md) | [**operator+**](#function-operator) (const [**buffer**](classqqmusic_1_1utils_1_1buffer.md) & a) <br>_两段buffer拼接, 并返回构造新的buffer_  |
|  [**buffer**](classqqmusic_1_1utils_1_1buffer.md) | [**operator+=**](#function-operator_1) (const [**buffer**](classqqmusic_1_1utils_1_1buffer.md) & a) <br>_将一段buffer插入本段buffer末尾_  |
|  [**buffer**](classqqmusic_1_1utils_1_1buffer.md) & | [**operator=**](#function-operator_2) (const [**buffer**](classqqmusic_1_1utils_1_1buffer.md) & other) = default<br>_拷贝构造函数, 重载_ `=` __ |
|   | [**~buffer**](#function-buffer) () = default<br> |




























## Public Functions Documentation




### function append 

_将一段内存插到这段buffer后面_ 
```C++
inline void qqmusic::utils::buffer::append (
    const void * src_data_buf,
    size_t data_buf_size
) 
```





**Parameters:**


* `src_data_buf` 待插入数据的首地址 
* `data_buf_size` 待插入数据的长度 




        

<hr>



### function buffer [1/7]

_默认初始化_ 
```C++
qqmusic::utils::buffer::buffer () = default
```




<hr>



### function buffer [2/7]

_预先分配异丁醇长度_ 
```C++
inline qqmusic::utils::buffer::buffer (
    size_t size
) 
```





**Parameters:**


* `size` 预先分配长度 




        

<hr>



### function buffer [3/7]

_直接从内存区域进行构造_ 
```C++
inline qqmusic::utils::buffer::buffer (
    const uint8_t * src_head,
    size_t src_size
) 
```





**Parameters:**


* `src_head` 内存区域的首地址 
* `src_size` 内存区域长度



**Note:**

是拷贝初始化 





        

<hr>



### function buffer [4/7]

_从std::vector&lt;uint8\_t&gt;移动构造_ 
```C++
inline qqmusic::utils::buffer::buffer (
    std::vector< uint8_t > && other
) noexcept
```





**Parameters:**


* `other` 




        

<hr>



### function buffer [5/7]

_移动构造_ 
```C++
inline qqmusic::utils::buffer::buffer (
    buffer && other
) noexcept
```





**Parameters:**


* `other` 




        

<hr>



### function buffer [6/7]

_初始化为特定字符_ 
```C++
inline qqmusic::utils::buffer::buffer (
    size_t size,
    char c
) 
```





**Parameters:**


* `size` 预先初始化长度 
* `c` 需要填充的字符 




        

<hr>



### function buffer [7/7]

_拷贝构造函数_ 
```C++
qqmusic::utils::buffer::buffer (
    const buffer & other
) = default
```





**Parameters:**


* `other` 




        

<hr>



### function operator+ 

_两段buffer拼接, 并返回构造新的buffer_ 
```C++
inline buffer qqmusic::utils::buffer::operator+ (
    const buffer & a
) 
```





**Parameters:**


* `a` 




        

<hr>



### function operator+= 

_将一段buffer插入本段buffer末尾_ 
```C++
inline buffer qqmusic::utils::buffer::operator+= (
    const buffer & a
) 
```





**Parameters:**


* `a` 




        

<hr>



### function operator= 

_拷贝构造函数, 重载_ `=` __
```C++
buffer & qqmusic::utils::buffer::operator= (
    const buffer & other
) = default
```





**Parameters:**


* `other` 




        

<hr>



### function ~buffer 

```C++
qqmusic::utils::buffer::~buffer () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/buffer.h`

