

# File session.cc



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**utils**](dir_313caf1132e152dd9b58bea13a4052ca.md) **>** [**session.cc**](session_8cc.md)

[Go to the source code of this file](session_8cc_source.md)



* `#include <boost/asio/use_awaitable.hpp>`
* `#include <boost/beast.hpp>`
* `#include <boost/beast/core/buffers_to_string.hpp>`
* `#include <boost/beast/core/stream_traits.hpp>`
* `#include <boost/beast/http/message.hpp>`
* `#include <boost/beast/ssl.hpp>`
* `#include <boost/url.hpp>`
* `#include <memory>`
* `#include <mutex>`
* `#include <qqmusic/result.h>`
* `#include <qqmusic/utils/cookie.h>`
* `#include <qqmusic/utils/session.h>`
* `#include <utility>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**qqmusic**](namespaceqqmusic.md) <br> |
| namespace | [**utils**](namespaceqqmusic_1_1utils.md) <br>_简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖_ `nlohmann::json` _实现 目前不支持路径嵌套和_`Expires` _和_`HttpOnly` _这样的特性_ |


























## Public Static Functions

| Type | Name |
| ---: | :--- |
|  qqmusic::Task&lt; qqmusic::Result&lt; qqmusic::utils::HttpResponse &gt; &gt; | [**handle\_http\_redirecting**](#function-handle_http_redirecting) ([**qqmusic::utils::Session**](classqqmusic_1_1utils_1_1Session.md) & self, boost::url\_view url, boost::beast::http::request&lt; boost::beast::http::string\_body &gt; & req, qqmusic::utils::HttpResponse & resp) <br> |
|  qqmusic::Task&lt; qqmusic::Result&lt; qqmusic::utils::HttpResponse &gt; &gt; | [**handle\_https\_redirecting**](#function-handle_https_redirecting) ([**qqmusic::utils::Session**](classqqmusic_1_1utils_1_1Session.md) & self, boost::url\_view url, boost::beast::http::request&lt; boost::beast::http::string\_body &gt; & req, qqmusic::utils::HttpResponse & resp, std::shared\_ptr&lt; boost::asio::ssl::context &gt; ssl\_ctx) <br> |


























## Public Static Functions Documentation




### function handle\_http\_redirecting 

```C++
static qqmusic::Task< qqmusic::Result< qqmusic::utils::HttpResponse > > handle_http_redirecting (
    qqmusic::utils::Session & self,
    boost::url_view url,
    boost::beast::http::request< boost::beast::http::string_body > & req,
    qqmusic::utils::HttpResponse & resp
) 
```




<hr>



### function handle\_https\_redirecting 

```C++
static qqmusic::Task< qqmusic::Result< qqmusic::utils::HttpResponse > > handle_https_redirecting (
    qqmusic::utils::Session & self,
    boost::url_view url,
    boost::beast::http::request< boost::beast::http::string_body > & req,
    qqmusic::utils::HttpResponse & resp,
    std::shared_ptr< boost::asio::ssl::context > ssl_ctx
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/utils/session.cc`

