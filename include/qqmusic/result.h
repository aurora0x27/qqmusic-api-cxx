/*-----------------------------qqmusic/result.h---------------------------------
 * Provide global api result -- based on
 * `https://github.com/oktal/result`
 *
 * Provide some type aliases
*-----------------------------------------------------------------------------*/
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

using Json = nlohmann::json;

} // namespace qqmusic

#endif // !QQMUSIC_RESULT_H
