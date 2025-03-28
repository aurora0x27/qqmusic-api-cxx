

# File result.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**result.h**](result_8h.md)

[Go to the documentation of this file](result_8h.md)


```C++
/*---------------------------------------qqmusic/result.h-------------------------------------------
 * Provide global api result -- based on
 * `https://github.com/oktal/result`
 *
 * Provide some type aliases
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_RESULT_H
#define QQMUSIC_RESULT_H

#include <boost/asio/awaitable.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/details/result.h>
#include <qqmusic/utils/exception.h>

namespace qqmusic {

/*define common return result of apis*/
template<typename T>
using Result = Result<T, qqmusic::utils::Exception>;

/*task used to asynchronous execution*/
template<typename T>
using Task = boost::asio::awaitable<T>;

} // namespace qqmusic

#endif // !QQMUSIC_RESULT_H
```


