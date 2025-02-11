/*---------------------------qqmusic/details/api.h------------------------------
 * Base api class, common interface to send http requests
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETIALS_API_H
#define QQMUSIC_DETIALS_API_H

#include <boost/beast/http.hpp>
#include <boost/url.hpp>
#include <boost/url/param.hpp>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic::details {

class Api {
public:
    Api() = delete;
    Api(std::string_view method,
        std::string_view module,
        bool need_verify = false,    /*can 'credential' be left blank*/
        bool ignore_code = false,    /*whether check return code of api*/
        bool proceduce_bool = true); /*whether convert bool to int*/

    qqmusic::Task<qqmusic::utils::buffer> request(
        boost::beast::http::verb verb,
        boost::urls::url_view url,
        std::initializer_list<boost::urls::param> const& params);

private:
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETIALS_API_H
