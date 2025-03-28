

# Class qqmusic::utils::Exception



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**Exception**](classqqmusic_1_1utils_1_1Exception.md)



_全局异常类_ 

* `#include <exception.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Kind**](#enum-kind)  <br>_错误类型枚举_  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Exception**](#function-exception-13) ([**Kind**](classqqmusic_1_1utils_1_1Exception.md#enum-kind) kind) <br>_从枚举构造_  |
|   | [**Exception**](#function-exception-23) ([**Kind**](classqqmusic_1_1utils_1_1Exception.md#enum-kind) kind, std::string\_view reason) <br>_从枚举构造并给出理由_  |
|   | [**Exception**](#function-exception-33) (unsigned int http\_rc) <br>_从HTTP协议状态码构造_  |
|  unsigned int | [**get\_error\_enum**](#function-get_error_enum) () const<br>_获取当前错误类型枚举_  |
|  std::string | [**kind**](#function-kind) () const<br>_返回当前错误类型描述_  |
|  std::string | [**what**](#function-what) () const<br>_返回错误原因_  |




























## Public Types Documentation




### enum Kind 

_错误类型枚举_ 
```C++
enum qqmusic::utils::Exception::Kind {
    ResponseCodeError = 0,
    CredendialExpiredError,
    CredentialInvalidError,
    LoginError,
    SignInvalidError,
    DataDestroy,
    RuntimeError,
    NetworkError,
    JsonError,
    OperationOutOfTime,
    SslError,
    UnknownError
};
```




<hr>
## Public Functions Documentation




### function Exception [1/3]

_从枚举构造_ 
```C++
inline qqmusic::utils::Exception::Exception (
    Kind kind
) 
```




<hr>



### function Exception [2/3]

_从枚举构造并给出理由_ 
```C++
inline qqmusic::utils::Exception::Exception (
    Kind kind,
    std::string_view reason
) 
```




<hr>



### function Exception [3/3]

_从HTTP协议状态码构造_ 
```C++
inline qqmusic::utils::Exception::Exception (
    unsigned int http_rc
) 
```




<hr>



### function get\_error\_enum 

_获取当前错误类型枚举_ 
```C++
inline unsigned int qqmusic::utils::Exception::get_error_enum () const
```




<hr>



### function kind 

_返回当前错误类型描述_ 
```C++
inline std::string qqmusic::utils::Exception::kind () const
```




<hr>



### function what 

_返回错误原因_ 
```C++
inline std::string qqmusic::utils::Exception::what () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/exception.h`

