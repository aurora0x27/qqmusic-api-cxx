

# Class qqmusic::utils::CookieJar



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**CookieJar**](classqqmusic_1_1utils_1_1CookieJar.md)



_简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖_ `nlohmann::json` _实现 目前不支持路径嵌套和_`Expires` _和_`HttpOnly` _这样的特性_[More...](#detailed-description)

* `#include <cookie.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CookieJar**](#function-cookiejar-13) () <br>_空初始化_  |
|   | [**CookieJar**](#function-cookiejar-23) (std::string\_view cache) <br>_缓存反序列化, 从_ `dump` _函数产生的缓存构造_ |
|   | [**CookieJar**](#function-cookiejar-33) (std::string\_view cookie\_str, std::string\_view domain, std::string\_view path="/") <br>_从原始cookie字符串解析_  |
|  qqmusic::Result&lt; void &gt; | [**clear**](#function-clear) (std::optional&lt; std::string &gt; domain=std::nullopt, std::optional&lt; std::string &gt; path=std::nullopt, std::optional&lt; std::string &gt; key=std::nullopt) <br>_删除对应区域的cookie_  |
|  qqmusic::Result&lt; void &gt; | [**del**](#function-del) (std::string\_view key, std::optional&lt; std::string &gt; domain=std::nullopt, std::optional&lt; std::string &gt; path=std::nullopt) <br>_删除对应键的Cookie字段和他的值_  |
|  qqmusic::Result&lt; std::string &gt; | [**dump**](#function-dump) () <br>_序列化Cookie_  |
|  qqmusic::Result&lt; std::string &gt; | [**get**](#function-get) (std::string\_view key, std::optional&lt; std::string &gt; domain=std::nullopt, std::optional&lt; std::string &gt; path=std::nullopt) <br>_查询对应键的Cookie字段的值_  |
|  qqmusic::Result&lt; void &gt; | [**merge**](#function-merge) (const [**CookieJar**](classqqmusic_1_1utils_1_1CookieJar.md) & cookies) <br>_合并CookieJar内容_  |
|  qqmusic::Result&lt; std::string &gt; | [**serialize**](#function-serialize) (std::string\_view domain, std::string\_view path="/") <br>_将对应域名和路径的Cookie序列化成请求报文_  |
|  qqmusic::Result&lt; void &gt; | [**set**](#function-set) (const [**Cookie**](structqqmusic_1_1utils_1_1Cookie.md) & cookie) <br>_插入一条Cookie_  |
|  qqmusic::Result&lt; void &gt; | [**update**](#function-update) (const [**CookieJar**](classqqmusic_1_1utils_1_1CookieJar.md) & cookies) <br>_替换CookieJar的内容_  |




























## Detailed Description


使用Json数据结构来储存Cookie 
```C++
{
    "domain1" : {
        "path1": {
            "key1": "value1",
            "key2": "value2"
        },
        "path2": {
            "key1": "value1",
            "key2": "value2"
        }
    },
    "domain2" : {
        "path1": {
            "key1": "value1",
            "key2": "value2"
        },
        "path2": {
            "key1": "value1",
            "key2": "value2"
        }
    }
}
```





**Warning:**

不支持path的嵌套, `example.com/menu` 和 `example.com/menu/usr` 被认为是域名`example.com`下两个完全不同的路径.


不支持解析`HttpOnly`, `Expires`这样的字段, 不是键值对形式的 Cookie条目会被丢弃.




**待办事项**

需要支持嵌套的路径, 支持非键值对形式的Cookie 




    
## Public Functions Documentation




### function CookieJar [1/3]

_空初始化_ 
```C++
inline qqmusic::utils::CookieJar::CookieJar () 
```




<hr>



### function CookieJar [2/3]

_缓存反序列化, 从_ `dump` _函数产生的缓存构造_
```C++
inline qqmusic::utils::CookieJar::CookieJar (
    std::string_view cache
) 
```





**Parameters:**


* `cache` 缓存 




        

<hr>



### function CookieJar [3/3]

_从原始cookie字符串解析_ 
```C++
qqmusic::utils::CookieJar::CookieJar (
    std::string_view cookie_str,
    std::string_view domain,
    std::string_view path="/"
) 
```





**Parameters:**


* `cookie_str` 请求报文中cookie字符串, 一般存在于`Set-Cookie`字段 
* `domain` 域名 
* `path` 路径, 默认是`/` 




        

<hr>



### function clear 

_删除对应区域的cookie_ 
```C++
qqmusic::Result< void > qqmusic::utils::CookieJar::clear (
    std::optional< std::string > domain=std::nullopt,
    std::optional< std::string > path=std::nullopt,
    std::optional< std::string > key=std::nullopt
) 
```




<hr>



### function del 

_删除对应键的Cookie字段和他的值_ 
```C++
qqmusic::Result< void > qqmusic::utils::CookieJar::del (
    std::string_view key,
    std::optional< std::string > domain=std::nullopt,
    std::optional< std::string > path=std::nullopt
) 
```





**Parameters:**


* `key` 键 
* `domain` 域名, 默认`std::nullopt`, 若不给出则全局寻找 
* `path` 路径, 默认`std::nullopt`, 若不给出则在域名下寻找



**Returns:**

`Result <void>`, 如果没有对应的键, 会返回`JsonError`




**Note:**

缺省参数时总是删除第一个找到符合的键值对 





        

<hr>



### function dump 

_序列化Cookie_ 
```C++
qqmusic::Result< std::string > qqmusic::utils::CookieJar::dump () 
```





**Returns:**

已经序列化的json字符串, 如果出错会返回`JsonError` 





        

<hr>



### function get 

_查询对应键的Cookie字段的值_ 
```C++
qqmusic::Result< std::string > qqmusic::utils::CookieJar::get (
    std::string_view key,
    std::optional< std::string > domain=std::nullopt,
    std::optional< std::string > path=std::nullopt
) 
```





**Parameters:**


* `key` 键 
* `domain` 域名, 默认`std::nullopt`, 若不给出则全局寻找 
* `path` 路径, 默认`std::nullopt`, 若不给出则在域名下寻找



**Returns:**

`Result <std::string>`, 键值对的值是字符串形式的 找不到会返回`JsonError`




**Note:**

参数缺省时总是返回第一个找到的键值对的值 





        

<hr>



### function merge 

_合并CookieJar内容_ 
```C++
qqmusic::Result< void > qqmusic::utils::CookieJar::merge (
    const CookieJar & cookies
) 
```





**Parameters:**


* `cookies` 待合并cookie



**Note:**

原本Cookie不存在的字段被创建, 重合的字段被新值覆盖 





        

<hr>



### function serialize 

_将对应域名和路径的Cookie序列化成请求报文_ 
```C++
qqmusic::Result< std::string > qqmusic::utils::CookieJar::serialize (
    std::string_view domain,
    std::string_view path="/"
) 
```





**Parameters:**


* `domain` 域名 
* `path` 路径, 默认`/` 




        

<hr>



### function set 

_插入一条Cookie_ 
```C++
qqmusic::Result< void > qqmusic::utils::CookieJar::set (
    const Cookie & cookie
) 
```





**Parameters:**


* `cookie` 一个Cookie条目



**Note:**

如果对应域名和路径的这个Cookie的键已经存在, 那么会覆盖老的值 





        

<hr>



### function update 

_替换CookieJar的内容_ 
```C++
qqmusic::Result< void > qqmusic::utils::CookieJar::update (
    const CookieJar & cookies
) 
```





**Parameters:**


* `cookies` 新的Cookies 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/cookie.h`

