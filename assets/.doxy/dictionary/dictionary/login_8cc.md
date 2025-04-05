

# File login.cc



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**login.cc**](login_8cc.md)

[Go to the source code of this file](login_8cc_source.md)



* `#include <boost/beast/core/buffers_to_string.hpp>`
* `#include <boost/beast/http.hpp>`
* `#include <boost/beast/http/message.hpp>`
* `#include <boost/uuid.hpp>`
* `#include <boost/uuid/uuid_io.hpp>`
* `#include <botan/auto_rng.h>`
* `#include <chrono>`
* `#include <cmath>`
* `#include <ctime>`
* `#include <qqmusic/details/api.h>`
* `#include <qqmusic/login.h>`
* `#include <qqmusic/result.h>`
* `#include <qqmusic/utils/common.h>`
* `#include <qqmusic/utils/session.h>`
* `#include <ratio>`
* `#include <regex>`
* `#include <sstream>`
* `#include <string>`
* `#include <string_view>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**qqmusic**](namespaceqqmusic.md) <br> |


























## Public Static Functions

| Type | Name |
| ---: | :--- |
|  qqmusic::Task&lt; qqmusic::Result&lt; [**qqmusic::utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; &gt; | [**auth\_qq\_qr**](#function-auth_qq_qr) (std::string\_view sigx, std::string\_view uin) <br> |
|  qqmusic::Task&lt; qqmusic::Result&lt; [**qqmusic::utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; &gt; | [**auth\_wx\_qr**](#function-auth_wx_qr) (std::string\_view code) <br> |


























## Public Static Functions Documentation




### function auth\_qq\_qr 

```C++
static qqmusic::Task< qqmusic::Result< qqmusic::utils::Credential > > auth_qq_qr (
    std::string_view sigx,
    std::string_view uin
) 
```




<hr>



### function auth\_wx\_qr 

```C++
static qqmusic::Task< qqmusic::Result< qqmusic::utils::Credential > > auth_wx_qr (
    std::string_view code
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/login.cc`

