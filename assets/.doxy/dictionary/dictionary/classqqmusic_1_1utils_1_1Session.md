

# Class qqmusic::utils::Session



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**Session**](classqqmusic_1_1utils_1_1Session.md)



_上下文_ 

* `#include <session.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Session**](#function-session-12) () = delete<br> |
|   | [**Session**](#function-session-22) ([**qqmusic::details::NetworkContext**](structqqmusic_1_1details_1_1NetworkContext.md) & nc, std::shared\_ptr&lt; asio::io\_context &gt; ioc\_ptr, std::shared\_ptr&lt; asio::ssl::context &gt; ssl\_ctx\_ptr, std::mutex & lock) <br>_构造函数, 正常情况下不应该使用_  |
|  [**qqmusic::details::NetworkContext**](structqqmusic_1_1details_1_1NetworkContext.md) & | [**get\_context\_ref**](#function-get_context_ref) () <br>_获取_ `session` _实例的上下文引用_ |
|  qqmusic::Task&lt; qqmusic::Result&lt; HttpResponse &gt; &gt; | [**perform\_request**](#function-perform_request) (boost::url\_view url, http::request&lt; http::string\_body &gt; & req, bool auto\_redirecting=true) <br>_发送网络请求, 返回回复报文_  |
|  void | [**sync\_global**](#function-sync_global) () <br>_将上下文的更改写回全局_  |
|  void | [**update\_local**](#function-update_local) () <br>_将全局的更改同步到本实例_  |




























## Public Functions Documentation




### function Session [1/2]

```C++
qqmusic::utils::Session::Session () = delete
```




<hr>



### function Session [2/2]

_构造函数, 正常情况下不应该使用_ 
```C++
inline qqmusic::utils::Session::Session (
    qqmusic::details::NetworkContext & nc,
    std::shared_ptr< asio::io_context > ioc_ptr,
    std::shared_ptr< asio::ssl::context > ssl_ctx_ptr,
    std::mutex & lock
) 
```




<hr>



### function get\_context\_ref 

_获取_ `session` _实例的上下文引用_
```C++
qqmusic::details::NetworkContext & qqmusic::utils::Session::get_context_ref () 
```




<hr>



### function perform\_request 

_发送网络请求, 返回回复报文_ 
```C++
qqmusic::Task< qqmusic::Result< HttpResponse > > qqmusic::utils::Session::perform_request (
    boost::url_view url,
    http::request< http::string_body > & req,
    bool auto_redirecting=true
) 
```





**Parameters:**


* `url` 请求url 
* `req` 请求对象 
* `auto_redirecting` 是否开启自动重定向, 默认`true`



**Returns:**

`qqmusic::Task<qqmusic::Result<HttpResponse>>`, 返回原始报文




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function sync\_global 

_将上下文的更改写回全局_ 
```C++
void qqmusic::utils::Session::sync_global () 
```




<hr>



### function update\_local 

_将全局的更改同步到本实例_ 
```C++
void qqmusic::utils::Session::update_local () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/session.h`

