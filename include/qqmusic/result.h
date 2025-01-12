/*---------qmusic/utils/result.hpp------------
 * global api result
 *-------------------------------------------*/
#ifndef RESULT_H
#define RESULT_H

#include <optional>
#include <string>

namespace qqmusic {

/*if execute success, return {}; if failure, return a string to describe reason*/
using result = std::optional<std::string>;

} // namespace qqmusic

#endif // !RESULT_H
