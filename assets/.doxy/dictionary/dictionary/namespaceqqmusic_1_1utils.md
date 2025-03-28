

# Namespace qqmusic::utils



[**Namespace List**](namespaces.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md)



_简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖_ `nlohmann::json` _实现 目前不支持路径嵌套和_`Expires` _和_`HttpOnly` _这样的特性_[More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| class | [**AsyncExecutor**](classqqmusic_1_1utils_1_1AsyncExecutor.md) <br>_全局调度器类_  |
| struct | [**Cookie**](structqqmusic_1_1utils_1_1Cookie.md) <br>_代表一条Cookie_  |
| class | [**CookieJar**](classqqmusic_1_1utils_1_1CookieJar.md) <br>_简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖_ `nlohmann::json` _实现 目前不支持路径嵌套和_`Expires` _和_`HttpOnly` _这样的特性_ |
| class | [**Credential**](classqqmusic_1_1utils_1_1Credential.md) <br>_凭据信息类_  |
| struct | [**Device**](structqqmusic_1_1utils_1_1Device.md) <br>_设备信息_  |
| class | [**Exception**](classqqmusic_1_1utils_1_1Exception.md) <br>_全局异常类_  |
| struct | [**OSVersion**](structqqmusic_1_1utils_1_1OSVersion.md) <br> |
| class | [**PathManager**](classqqmusic_1_1utils_1_1PathManager.md) <br>_返回默认路径_  |
| struct | [**QimeiResult**](structqqmusic_1_1utils_1_1QimeiResult.md) <br> |
| class | [**Session**](classqqmusic_1_1utils_1_1Session.md) <br>_上下文_  |
| class | [**SessionGuard**](classqqmusic_1_1utils_1_1SessionGuard.md) <br>_一个_ `RAII` _风格的Session切换器_ |
| class | [**SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md) <br>_全局单例的Session管理器, 负责维护全局上下文栈, 并负责_ `session` _实例的生成_ |
| class | [**buffer**](classqqmusic_1_1utils_1_1buffer.md) <br>_二进制缓冲区的统一表示_  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef http::response&lt; http::dynamic\_body &gt; | [**HttpResponse**](#typedef-httpresponse)  <br> |
| enum  | [**qrc\_type**](#enum-qrc_type)  <br>_QRC歌词种类_  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  qqmusic::Result&lt; void &gt; | [**cache\_device**](#function-cache_device) (const [**Device**](structqqmusic_1_1utils_1_1Device.md) & device) <br>_将设备信息缓存到文件缓存路径下的_ `device.json` __ |
|  boost::asio::awaitable&lt; void &gt; | [**custom\_co\_spawn\_entry\_point**](#function-custom_co_spawn_entry_point) (boost::asio::awaitable&lt; T, Executor &gt; aw, std::function&lt; void(std::exception\_ptr, std::optional&lt; T &gt;)&gt; handler) <br>_Result&lt;&gt;不能进行拷贝, 也没有默认构造函数. boost::asio::co\_spawn中会调用默认构造函数. 所以需要包装原本的异步任务, 使用移动构造函数来构造result_  |
|  qqmusic::Result&lt; [**Device**](structqqmusic_1_1utils_1_1Device.md) &gt; | [**get\_device\_info**](#function-get_device_info) () <br>_获取随机设备信息_  |
|  qqmusic::Result&lt; [**QimeiResult**](structqqmusic_1_1utils_1_1QimeiResult.md) &gt; | [**get\_qimei**](#function-get_qimei) ([**qqmusic::utils::Device**](structqqmusic_1_1utils_1_1Device.md) & device, std::string\_view version) <br> |
|  std::string | [**get\_search\_id**](#function-get_search_id) () <br>_返回随机搜索ID_  |
|  uint64\_t | [**hash33**](#function-hash33) (std::string\_view str, uint64\_t prev=0) <br>_计算utf-8字符串的哈希值_  |
|  [**buffer**](classqqmusic_1_1utils_1_1buffer.md) | [**hex2buf**](#function-hex2buf) (std::string\_view hex) <br>_十六进制字符串转成buffer_  |
|  qqmusic::Result&lt; nlohmann::json &gt; | [**parse\_cookie**](#function-parse_cookie) (std::string\_view cookie\_str) <br>_将Cookie字符串解析成Json对象_  |
|  qqmusic::Result&lt; std::string &gt; | [**qrc\_decode**](#function-qrc_decode) (const [**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & src, [**qqmusic::utils::qrc\_type**](namespaceqqmusic_1_1utils.md#enum-qrc_type) type) <br>_解码加密的QRC歌词_  |
|  [**buffer**](classqqmusic_1_1utils_1_1buffer.md) | [**resp2buf**](#function-resp2buf) (http::response&lt; http::dynamic\_body &gt; && resp) <br>_将请求回复报文体转换成二进制buffer_  |
|  std::string | [**sign**](#function-sign) (const nlohmann::json & params) <br>_QQ音乐请求签名_  |
|  T | [**sync\_exec**](#function-sync_exec) (boost::asio::io\_context & ioc, qqmusic::Task&lt; T &gt; task) <br>_同步执行异步函数_  |
|  T | [**sync\_exec**](#function-sync_exec) (qqmusic::Task&lt; T &gt; task) <br>_同步执行异步函数_  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  int | [**decompress**](#function-decompress) (const [**buffer**](classqqmusic_1_1utils_1_1buffer.md) & src, [**buffer**](classqqmusic_1_1utils_1_1buffer.md) & dest) <br> |
|  std::string | [**head**](#function-head) (std::span&lt; uint8\_t &gt; data) <br> |
|  std::string | [**middle**](#function-middle) (std::span&lt; uint8\_t &gt; data) <br> |
|  void | [**qmc1\_decrypt**](#function-qmc1_decrypt) ([**buffer**](classqqmusic_1_1utils_1_1buffer.md) & src) <br> |
|  std::string | [**tail**](#function-tail) (std::span&lt; uint8\_t &gt; data) <br> |


























## Detailed Description


API异常类, 提供全局错误类型表示


设备信息提供设备信息描述, 随机生成设备信息


QQ音乐账号凭据, 部分和会员, 用户相关的Api需要依靠传入凭据才能生效, 凭据由`Login`部分的 Api获取


-------------------------------qqmusic/utils/common.h--------------------------------------


@ brief 工具函数


@ date 2025-3-21






------------------------------qqmusic/details/cookie.h-------------------------------------




**Date:**

2025-3-21






-------------------------------------utils/credential.h------------------------------------




**Date:**

2025-3-21






--------------------------------qqmusic/details/device.h-----------------------------------




**Date:**

2025-3-21






----------------------------qqmusic/utils/exception.h--------------------------------------




**Date:**

2025-3-21






---------------------------------------utils/paths.h--------------------------------------- [**PathManager**](classqqmusic_1_1utils_1_1PathManager.md) provide common interface to file system.


[**PathManager**](classqqmusic_1_1utils_1_1PathManager.md) provide 3 types of files for io : log, cache, download
* _log_ for debug log info
* _cache_ for cached credential, identification, recent cached songs
* _download_ for downloaded media and lyrics




Default paths are:
* Linux:
  * cache: $XDG\_CACHE\_HOME/qqmusic-api-cxx/cache OR $HOME/.cache/qqmusic-api-cxx/cache
  * log: $XDG\_CACHE\_HOME/qqmusic-api-cxx/log OR $HOME/.cache/qqmusic-api-cxx/log
  * download: $HOME/Download/qqmusic-api-cxx






 Applications can set the path by themselves Usage: 


### @icode



"get instance first" auto& pm = [**PathManager::get\_instance()**](classqqmusic_1_1utils_1_1PathManager.md#function-get_instance); auto cache\_path = cm.get\_cache\_path();  



    
## Public Types Documentation




### typedef HttpResponse 

```C++
using qqmusic::utils::HttpResponse = typedef http::response<http::dynamic_body>;
```




<hr>



### enum qrc\_type 

_QRC歌词种类_ 
```C++
enum qqmusic::utils::qrc_type {
    cloud,
    local
};
```




<hr>
## Public Functions Documentation




### function cache\_device 

_将设备信息缓存到文件缓存路径下的_ `device.json` __
```C++
qqmusic::Result< void > qqmusic::utils::cache_device (
    const Device & device
) 
```





**See also:** [**qqmusic::utils::PathManager**](classqqmusic_1_1utils_1_1PathManager.md) 



        

<hr>



### function custom\_co\_spawn\_entry\_point 

_Result&lt;&gt;不能进行拷贝, 也没有默认构造函数. boost::asio::co\_spawn中会调用默认构造函数. 所以需要包装原本的异步任务, 使用移动构造函数来构造result_ 
```C++
template<typename T, typename Executor>
boost::asio::awaitable< void > qqmusic::utils::custom_co_spawn_entry_point (
    boost::asio::awaitable< T, Executor > aw,
    std::function< void(std::exception_ptr, std::optional< T >)> handler
) 
```





**Parameters:**


* `aw` 异步任务 
* `handler` 回调函数



**Note:**

回调函数的签名是`void(std::exception_ptr, std::optional<T>)`, 前一个参数是处理异常的, 后一个参数是正常的返回结果. 如果出错, 则返回结果的`std::optional`置空




**See also:** qqmusic::Result 



        

<hr>



### function get\_device\_info 

_获取随机设备信息_ 
```C++
qqmusic::Result< Device > qqmusic::utils::get_device_info () 
```





**Returns:**

包含正确结果或异常的`Result < Device >` 





        

<hr>



### function get\_qimei 

```C++
qqmusic::Result< QimeiResult > qqmusic::utils::get_qimei (
    qqmusic::utils::Device & device,
    std::string_view version
) 
```




<hr>



### function get\_search\_id 

_返回随机搜索ID_ 
```C++
std::string qqmusic::utils::get_search_id () 
```




<hr>



### function hash33 

_计算utf-8字符串的哈希值_ 
```C++
uint64_t qqmusic::utils::hash33 (
    std::string_view str,
    uint64_t prev=0
) 
```





**Parameters:**


* `str` 待求值字符串 
* `prev` 多次求值时传入上次求出的结果, 默认为`0`



**Returns:**

64位整数格式的哈希 





        

<hr>



### function hex2buf 

_十六进制字符串转成buffer_ 
```C++
buffer qqmusic::utils::hex2buf (
    std::string_view hex
) 
```





**Parameters:**


* `hex` 十六进制编码字符串



**Returns:**

qqmusic::utils::buffer类型buffer 





        

<hr>



### function parse\_cookie 

_将Cookie字符串解析成Json对象_ 
```C++
qqmusic::Result< nlohmann::json > qqmusic::utils::parse_cookie (
    std::string_view cookie_str
) 
```




```C++
key1=value1; key2=value2; key3=value3; key4=value4
     |
     v
{
    "key1": "value1",
    "key2": "value2",
    "key3": "value3",
    "key4": "value4",
}
```
 


        

<hr>



### function qrc\_decode 

_解码加密的QRC歌词_ 
```C++
qqmusic::Result< std::string > qqmusic::utils::qrc_decode (
    const qqmusic::utils::buffer & src,
    qqmusic::utils::qrc_type type
) 
```





**Parameters:**


* `src` 被加密的歌词 
* `type` 歌词类型



**Returns:**

解码后的字符串, 如果出错会返回`Exception` 





        

<hr>



### function resp2buf 

_将请求回复报文体转换成二进制buffer_ 
```C++
buffer qqmusic::utils::resp2buf (
    http::response< http::dynamic_body > && resp
) 
```





**Parameters:**


* `resp` 回复报文



**Returns:**

qqmusic::utils::buffer类型buffer 





        

<hr>



### function sign 

_QQ音乐请求签名_ 
```C++
std::string qqmusic::utils::sign (
    const nlohmann::json & params
) 
```





**Parameters:**


* `params` 请求数据表单



**Returns:**

签名结果 





        

<hr>



### function sync\_exec 

_同步执行异步函数_ 
```C++
template<typename T>
T qqmusic::utils::sync_exec (
    boost::asio::io_context & ioc,
    qqmusic::Task< T > task
) 
```





**Parameters:**


* `ioc` 当前io\_context引用 
* `task` 异步任务



**Returns:**

返回执行结果




**Exception:**


* `std::runtime_error` 




        

<hr>



### function sync\_exec 

_同步执行异步函数_ 
```C++
template<typename T>
T qqmusic::utils::sync_exec (
    qqmusic::Task< T > task
) 
```





**Parameters:**


* `task` 异步任务



**Returns:**

返回执行结果 





        

<hr>
## Public Static Functions Documentation




### function decompress 

```C++
static int qqmusic::utils::decompress (
    const buffer & src,
    buffer & dest
) 
```




<hr>



### function head 

```C++
static std::string qqmusic::utils::head (
    std::span< uint8_t > data
) 
```




<hr>



### function middle 

```C++
static std::string qqmusic::utils::middle (
    std::span< uint8_t > data
) 
```




<hr>



### function qmc1\_decrypt 

```C++
static void qqmusic::utils::qmc1_decrypt (
    buffer & src
) 
```




<hr>



### function tail 

```C++
static std::string qqmusic::utils::tail (
    std::span< uint8_t > data
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/utils/async-executor.cc`

