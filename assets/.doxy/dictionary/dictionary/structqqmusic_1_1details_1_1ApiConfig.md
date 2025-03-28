

# Struct qqmusic::details::ApiConfig



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**details**](namespaceqqmusic_1_1details.md) **>** [**ApiConfig**](structqqmusic_1_1details_1_1ApiConfig.md)



_Api相关常量_ 

* `#include <context.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  bool | [**enable\_sign**](#variable-enable_sign)   = `false`<br>_请求参数是否需要签名_  |
|  std::string | [**enc\_endpoint**](#variable-enc_endpoint)   = `"https://u.y.qq.com/cgi-bin/musics.fcg"`<br>_加密接口url_  |
|  std::string | [**endpoint**](#variable-endpoint)   = `"https://u.y.qq.com/cgi-bin/musicu.fcg"`<br>_未加密接口url_  |
|  std::string | [**version**](#variable-version)   = `[**VERSION**](namespaceqqmusic_1_1details.md#variable-version)`<br> |
|  uint64\_t | [**version\_code**](#variable-version_code)   = `[**VERSION\_CODE**](namespaceqqmusic_1_1details.md#variable-version_code)`<br> |












































## Public Attributes Documentation




### variable enable\_sign 

_请求参数是否需要签名_ 
```C++
bool qqmusic::details::ApiConfig::enable_sign;
```




<hr>



### variable enc\_endpoint 

_加密接口url_ 
```C++
std::string qqmusic::details::ApiConfig::enc_endpoint;
```




<hr>



### variable endpoint 

_未加密接口url_ 
```C++
std::string qqmusic::details::ApiConfig::endpoint;
```




<hr>



### variable version 

```C++
std::string qqmusic::details::ApiConfig::version;
```




<hr>



### variable version\_code 

```C++
uint64_t qqmusic::details::ApiConfig::version_code;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/context.h`

