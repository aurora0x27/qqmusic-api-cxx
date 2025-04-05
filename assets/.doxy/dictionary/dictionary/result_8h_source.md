

# File result.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**result.h**](result_8h.md)

[Go to the documentation of this file](result_8h.md)


```C++

#ifndef QQMUSIC_RESULT_H
#define QQMUSIC_RESULT_H

#include <boost/asio/awaitable.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/details/result.h>
#include <qqmusic/utils/exception.h>

namespace qqmusic {

template<typename T>
using Result = Result<T, qqmusic::utils::Exception>;

template<typename T>
using Task = boost::asio::awaitable<T>;

} // namespace qqmusic

#endif // !QQMUSIC_RESULT_H
```


