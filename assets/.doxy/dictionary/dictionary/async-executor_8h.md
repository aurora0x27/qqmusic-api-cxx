

# File async-executor.h



[**FileList**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**utils**](dir_478616d8952f43e793f28d8ded6e3463.md) **>** [**async-executor.h**](async-executor_8h.md)

[Go to the source code of this file](async-executor_8h_source.md)

_提供了一个默认的简单协程调度器, 以及同步执行异步函数的方法._ [More...](#detailed-description)

* `#include <boost/asio/co_spawn.hpp>`
* `#include <boost/asio/detached.hpp>`
* `#include <boost/asio/io_context.hpp>`
* `#include <boost/asio/steady_timer.hpp>`
* `#include <boost/asio/use_future.hpp>`
* `#include <exception>`
* `#include <qqmusic/result.h>`
* `#include <stdexcept>`
* `#include <thread>`
* `#include <utility>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**qqmusic**](namespaceqqmusic.md) <br> |
| namespace | [**utils**](namespaceqqmusic_1_1utils.md) <br>_简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖_ `nlohmann::json` _实现 目前不支持路径嵌套和_`Expires` _和_`HttpOnly` _这样的特性_ |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**AsyncExecutor**](classqqmusic_1_1utils_1_1AsyncExecutor.md) <br>_全局调度器类_  |


















































## Detailed Description


-------------------------------qqmusic/utils/async-executor.h------------------------------




**Date:**

2025-3-21



 


    

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/async-executor.h`

