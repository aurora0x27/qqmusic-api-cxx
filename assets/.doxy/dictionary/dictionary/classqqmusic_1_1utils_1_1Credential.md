

# Class qqmusic::utils::Credential



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**Credential**](classqqmusic_1_1utils_1_1Credential.md)



_凭据信息类_ 

* `#include <credential.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::string | [**access\_token**](#variable-access_token)  <br>_AccessToken_  |
|  std::string | [**encryptUin**](#variable-encryptuin)  <br>_加密的euin, euin和账号ID一一对应_  |
|  uint64\_t | [**expired\_at**](#variable-expired_at)   = `0`<br>_到期时间, 秒级时间戳_  |
|  nlohmann::json | [**extra\_fields**](#variable-extra_fields)  <br>_没有收录在上面的字段_  |
|  int | [**loginType**](#variable-logintype)   = `2`<br>_登录模式_ `1` _: 微信登陆_`2` _: QQ登陆_ |
|  uint64\_t | [**musicid**](#variable-musicid)   = `0`<br>_账号MusicID, 一般是QQ号_  |
|  std::string | [**musickey**](#variable-musickey)  <br>_账号MusicKey_  |
|  std::string | [**openid**](#variable-openid)  <br>_OpenID_  |
|  std::string | [**refresh\_key**](#variable-refresh_key)  <br>_RefreshKey_  |
|  std::string | [**refresh\_token**](#variable-refresh_token)  <br>_RefreshToken_  |
|  std::string | [**str\_musicid**](#variable-str_musicid)  <br>_std::string格式的账号MusicID, 一般是QQ号_  |
|  std::string | [**unionid**](#variable-unionid)  <br>_UnionID_  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Credential**](#function-credential-13) () = default<br>_默认空初始化_  |
|   | [**Credential**](#function-credential-23) (std::string\_view cookie) <br>_从login api获取的返回结果构造Credential_  |
|   | [**Credential**](#function-credential-33) (nlohmann::json & cookie) <br>_从login api返回的结果构造Credential_  |
|   | [**NLOHMANN\_DEFINE\_TYPE\_INTRUSIVE**](#function-nlohmann_define_type_intrusive) ([**Credential**](classqqmusic_1_1utils_1_1Credential.md), [**openid**](classqqmusic_1_1utils_1_1Credential.md#variable-openid), [**refresh\_token**](classqqmusic_1_1utils_1_1Credential.md#variable-refresh_token), [**access\_token**](classqqmusic_1_1utils_1_1Credential.md#variable-access_token), [**expired\_at**](classqqmusic_1_1utils_1_1Credential.md#variable-expired_at), [**musicid**](classqqmusic_1_1utils_1_1Credential.md#variable-musicid), [**unionid**](classqqmusic_1_1utils_1_1Credential.md#variable-unionid), [**str\_musicid**](classqqmusic_1_1utils_1_1Credential.md#variable-str_musicid), [**musickey**](classqqmusic_1_1utils_1_1Credential.md#variable-musickey), [**refresh\_key**](classqqmusic_1_1utils_1_1Credential.md#variable-refresh_key), [**encryptUin**](classqqmusic_1_1utils_1_1Credential.md#variable-encryptuin), [**loginType**](classqqmusic_1_1utils_1_1Credential.md#variable-logintype)) <br> |
|  bool | [**from\_cache**](#function-from_cache-12) (std::string\_view cache) <br>_从序列化出的cache生成Credential_  |
|  bool | [**from\_cache**](#function-from_cache-22) (nlohmann::json & cache) <br>_从序列化出的cache生成Credential_  |
|  qqmusic::Task&lt; qqmusic::Result&lt; bool &gt; &gt; | [**is\_expired**](#function-is_expired) () <br>_判断Credential是否过期_  |
|  bool | [**is\_valid**](#function-is_valid) () const<br>_判断Credential是否合法, 即有没有_ `musicid` _和_`musickey` _字段_ |
|  qqmusic::Task&lt; qqmusic::Result&lt; void &gt; &gt; | [**refresh**](#function-refresh) () <br>_刷新Credential_  |
|  qqmusic::Result&lt; nlohmann::json &gt; | [**to\_json**](#function-to_json) () <br>_将Credential序列化为Json对象_  |
|  qqmusic::Result&lt; std::string &gt; | [**to\_string**](#function-to_string) () <br>_将Credential序列化为Json字符串_  |




























## Public Attributes Documentation




### variable access\_token 

_AccessToken_ 
```C++
std::string qqmusic::utils::Credential::access_token;
```




<hr>



### variable encryptUin 

_加密的euin, euin和账号ID一一对应_ 
```C++
std::string qqmusic::utils::Credential::encryptUin;
```




<hr>



### variable expired\_at 

_到期时间, 秒级时间戳_ 
```C++
uint64_t qqmusic::utils::Credential::expired_at;
```




<hr>



### variable extra\_fields 

_没有收录在上面的字段_ 
```C++
nlohmann::json qqmusic::utils::Credential::extra_fields;
```




<hr>



### variable loginType 

_登录模式_ `1` _: 微信登陆_`2` _: QQ登陆_
```C++
int qqmusic::utils::Credential::loginType;
```




<hr>



### variable musicid 

_账号MusicID, 一般是QQ号_ 
```C++
uint64_t qqmusic::utils::Credential::musicid;
```





**Note:**

常用字段 





        

<hr>



### variable musickey 

_账号MusicKey_ 
```C++
std::string qqmusic::utils::Credential::musickey;
```





**Note:**

常用 





        

<hr>



### variable openid 

_OpenID_ 
```C++
std::string qqmusic::utils::Credential::openid;
```




<hr>



### variable refresh\_key 

_RefreshKey_ 
```C++
std::string qqmusic::utils::Credential::refresh_key;
```




<hr>



### variable refresh\_token 

_RefreshToken_ 
```C++
std::string qqmusic::utils::Credential::refresh_token;
```




<hr>



### variable str\_musicid 

_std::string格式的账号MusicID, 一般是QQ号_ 
```C++
std::string qqmusic::utils::Credential::str_musicid;
```





**Note:**

常用字段 





        

<hr>



### variable unionid 

_UnionID_ 
```C++
std::string qqmusic::utils::Credential::unionid;
```




<hr>
## Public Functions Documentation




### function Credential [1/3]

_默认空初始化_ 
```C++
qqmusic::utils::Credential::Credential () = default
```




<hr>



### function Credential [2/3]

_从login api获取的返回结果构造Credential_ 
```C++
explicit qqmusic::utils::Credential::Credential (
    std::string_view cookie
) 
```





**Parameters:**


* `cookie` Json字符串 




        

<hr>



### function Credential [3/3]

_从login api返回的结果构造Credential_ 
```C++
explicit qqmusic::utils::Credential::Credential (
    nlohmann::json & cookie
) 
```





**Parameters:**


* `cookie` Json对象 




        

<hr>



### function NLOHMANN\_DEFINE\_TYPE\_INTRUSIVE 

```C++
qqmusic::utils::Credential::NLOHMANN_DEFINE_TYPE_INTRUSIVE (
    Credential,
    openid,
    refresh_token,
    access_token,
    expired_at,
    musicid,
    unionid,
    str_musicid,
    musickey,
    refresh_key,
    encryptUin,
    loginType
) 
```




<hr>



### function from\_cache [1/2]

_从序列化出的cache生成Credential_ 
```C++
bool qqmusic::utils::Credential::from_cache (
    std::string_view cache
) 
```





**Parameters:**


* `cache` 由`Credential::to_string()`和`Credential::to_json()` 生成的序列化结果导入Credential



**Returns:**

`true`, 如果处理过程出现问题 `false`, 如果正常处理完毕 





        

<hr>



### function from\_cache [2/2]

_从序列化出的cache生成Credential_ 
```C++
bool qqmusic::utils::Credential::from_cache (
    nlohmann::json & cache
) 
```





**Parameters:**


* `cache` 由`Credential::to_string()`和`Credential::to_json()` 生成的序列化结果导入Credential



**Returns:**

`true`, 如果处理过程出现问题 `false`, 如果正常处理完毕 





        

<hr>



### function is\_expired 

_判断Credential是否过期_ 
```C++
qqmusic::Task< qqmusic::Result< bool > > qqmusic::utils::Credential::is_expired () 
```





**Returns:**

包含正确JSON格式结果的`Task< Result <bool>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举 返回`true`说明未过期, `false`说明过期, 需要刷新




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function is\_valid 

_判断Credential是否合法, 即有没有_ `musicid` _和_`musickey` _字段_
```C++
bool qqmusic::utils::Credential::is_valid () const
```





**Returns:**

`true`, 合法 `false`, 不合法 





        

<hr>



### function refresh 

_刷新Credential_ 
```C++
qqmusic::Task< qqmusic::Result< void > > qqmusic::utils::Credential::refresh () 
```





**Returns:**

包含正确JSON格式结果的`Task< Result <void>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function to\_json 

_将Credential序列化为Json对象_ 
```C++
qqmusic::Result< nlohmann::json > qqmusic::utils::Credential::to_json () 
```





**Returns:**

包含结果或错误的`Result <nlohmann::json>` 





        

<hr>



### function to\_string 

_将Credential序列化为Json字符串_ 
```C++
qqmusic::Result< std::string > qqmusic::utils::Credential::to_string () 
```





**Returns:**

包含结果或错误的`Result <std::string>` 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/credential.h`

