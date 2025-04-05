

# Class qqmusic::details::Api



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**details**](namespaceqqmusic_1_1details.md) **>** [**Api**](classqqmusic_1_1details_1_1Api.md)



_Api类, 简化请求报文组装和返回报文解析过程_ 

* `#include <api.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Api**](#function-api) ([**utils::Session**](classqqmusic_1_1utils_1_1Session.md) & session, std::string module, std::string method, [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) credential={}, nlohmann::json common={{}}) <br>_构造函数_  |
|  qqmusic::Result&lt; nlohmann::json &gt; | [**parse\_response**](#function-parse_response) ([**utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) && response) <br>_将返回报文解析成Json结果, 必须是由Api类产生的请求产生的结果_  |
|  qqmusic::Task&lt; qqmusic::Result&lt; [**RequestParam**](structqqmusic_1_1details_1_1RequestParam.md) &gt; &gt; | [**prepare\_request**](#function-prepare_request) (const nlohmann::json & params, bool verify=false) <br>_使用给定的参数组装请求_  |




























## Public Functions Documentation




### function Api 

_构造函数_ 
```C++
inline qqmusic::details::Api::Api (
    utils::Session & session,
    std::string module,
    std::string method,
    utils::Credential credential={},
    nlohmann::json common={{}}
) 
```





**Parameters:**


* `session` session实例引用 
* `module` Api所属模块 
* `method` Api请求方法 
* `credential` 凭证, 默认为空 
* `common` 公共参数



**Note:**

credential的取舍优先级: `传入的参数 > session(全局默认) > 默认初始化credential` 





        

<hr>



### function parse\_response 

_将返回报文解析成Json结果, 必须是由Api类产生的请求产生的结果_ 
```C++
qqmusic::Result< nlohmann::json > qqmusic::details::Api::parse_response (
    utils::buffer && response
) 
```




<hr>



### function prepare\_request 

_使用给定的参数组装请求_ 
```C++
qqmusic::Task< qqmusic::Result< RequestParam > > qqmusic::details::Api::prepare_request (
    const nlohmann::json & params,
    bool verify=false
) 
```





**Parameters:**


* `params` 请求体数据表单 
* `verify` 是否需要合法的Credential进行请求



**Returns:**

产生`RequestParam`打包的请求参数 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/api.h`

