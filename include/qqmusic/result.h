/**--------------------------------------qqmusic/result.h-------------------------------------------
 *
 * @file include/qqmusic/result.h
 *
 * @brief 提供返回值类型的封装
 *
 * @detail `Result<>`基于[Result.h](https://github.com/oktal/result)设计, 采用Rust风格的返回值, 同时
 *          封装正确的返回结果和异常类型.
 *
 * `Task<>`是基于`boost::asio`的通用协程异步任务, 作为协程异步函数的返回值
 *
 * @date 2025-4-2
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_RESULT_H
#define QQMUSIC_RESULT_H

#include <boost/asio/awaitable.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/details/result.h>
#include <qqmusic/utils/exception.h>

namespace qqmusic {

template<typename T>
using Result = Result<T, qqmusic::utils::Exception>;

/*task used to asynchronous execution*/
template<typename T>
using Task = boost::asio::awaitable<T>;

} // namespace qqmusic

#endif // !QQMUSIC_RESULT_H
