/*---------qmusic/utils/result.hpp------------
 * global api result -- based on
 * `https://github.com/oktal/result`
 *-------------------------------------------*/
#ifndef RESULT_H
#define RESULT_H

#include <qqmusic/details/result.h>
#include <qqmusic/utils/exception.h>

namespace qqmusic {

template <typename T>
using result = Result<T, qqmusic::utils::Exception>;

} // namespace qqmusic

#endif // !RESULT_H
