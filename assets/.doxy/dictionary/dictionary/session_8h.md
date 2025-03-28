

# File session.h



[**FileList**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**utils**](dir_478616d8952f43e793f28d8ded6e3463.md) **>** [**session.h**](session_8h.md)

[Go to the source code of this file](session_8h_source.md)



* `#include <boost/asio.hpp>`
* `#include <boost/asio/ssl.hpp>`
* `#include <boost/beast/http/dynamic_body.hpp>`
* `#include <boost/beast/http/message.hpp>`
* `#include <boost/beast/http/string_body.hpp>`
* `#include <boost/url.hpp>`
* `#include <memory>`
* `#include <qqmusic/details/context.h>`
* `#include <qqmusic/result.h>`
* `#include <qqmusic/utils/credential.h>`
* `#include <qqmusic/utils/device.h>`
* `#include <qqmusic/utils/qimei.h>`
* `#include <stack>`
* `#include <utility>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**qqmusic**](namespaceqqmusic.md) <br> |
| namespace | [**utils**](namespaceqqmusic_1_1utils.md) <br>_简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖_ `nlohmann::json` _实现 目前不支持路径嵌套和_`Expires` _和_`HttpOnly` _这样的特性_ |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**Session**](classqqmusic_1_1utils_1_1Session.md) <br>_上下文_  |
| class | [**SessionGuard**](classqqmusic_1_1utils_1_1SessionGuard.md) <br>_一个_ `RAII` _风格的Session切换器_ |
| class | [**SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md) <br>_全局单例的Session管理器, 负责维护全局上下文栈, 并负责_ `session` _实例的生成_ |



















































------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/session.h`

