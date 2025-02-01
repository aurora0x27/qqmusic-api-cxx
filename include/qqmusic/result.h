/*---------qmusic/utils/result.hpp------------
 * global api result -- based on
 * `https://github.com/oktal/result`
 *-------------------------------------------*/
#ifndef RESULT_H
#define RESULT_H

#include <boost/asio/awaitable.hpp>
#include <qqmusic/details/result.h>
#include <qqmusic/utils/exception.h>

namespace qqmusic {

/*define common return result of apis*/
template<typename T>
using result = Result<T, qqmusic::utils::Exception>;

/*task used to asynchronous execution*/
template<typename T>
using task = boost::asio::awaitable<T>;

} // namespace qqmusic

#endif // !RESULT_H
