/*---------qmusic/utils/result.hpp------------
 * global api result
 *-------------------------------------------*/
#ifndef RESULT_H
#define RESULT_H

#include <qqmusic/details/result.h>
#include <qqmusic/details/exception.h>

namespace qqmusic {

template <typename T>
using result = Result<T, qqmusic::details::Exception>;

} // namespace qqmusic

#endif // !RESULT_H
