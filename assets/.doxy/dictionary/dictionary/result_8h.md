

# File result.h



[**FileList**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**result.h**](result_8h.md)

[Go to the source code of this file](result_8h_source.md)

_提供返回值类型的封装_ [More...](#detailed-description)

* `#include <boost/asio/awaitable.hpp>`
* `#include <nlohmann/json.hpp>`
* `#include <qqmusic/details/result.h>`
* `#include <qqmusic/utils/exception.h>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**qqmusic**](namespaceqqmusic.md) <br> |




















































## Detailed Description


-----------------------------------qqmusic/result.h----------------------------------------


@detail `Result <>`基于[Result.h](https://github.com/oktal/result)设计, 采用Rust风格的返回值, 同时 封装正确的返回结果和异常类型.


`Task<>`是基于`boost::asio`的通用协程异步任务, 作为协程异步函数的返回值




**Date:**

2025-4-2



 


    

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/result.h`

