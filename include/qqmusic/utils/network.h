/*------------------------qqmusic/utils/network.h-----------------------------
 * Provide a basic interface to perform http request, include handling cookie 
 * and redirecting
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_utils_NETWORK_H
#define QQMUSIC_utils_NETWORK_H

#include <boost/asio.hpp>
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <qqmusic/details/context.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>
#include <qqmusic/utils/device.h>
#include <qqmusic/utils/qimei.h>

namespace qqmusic::utils {

using HttpResponse = boost::beast::http::response<boost::beast::http::dynamic_body>;
namespace http = boost::beast::http;

// TODO: handle http requests, automatically manage cookie and do redirect jobs
class NetworkClient {};

} // namespace qqmusic::utils

#endif // !QQMUSIC_utils_NETWORK_H
