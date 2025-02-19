/*---------------------------qqmusic/details/api.h------------------------------
 * Api class prepare a request with provided infomation and global context
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETIALS_API_H
#define QQMUSIC_DETIALS_API_H

#include <boost/beast/http.hpp>
#include <boost/url.hpp>
#include <boost/url/param.hpp>
#include <qqmusic/details/network.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic::details {

class Api {
public:
    Api(std::string module,
        std::string method,
        /*optional params*/
        qqmusic::utils::Credential credential = {},
        nlohmann::json common = {},
        bool need_verify = false,   /*can 'credential' be left blank*/
        bool ignore_code = false,   /*whether check return code of api*/
        bool proceduce_bool = true) /*whether convert bool to int*/
        : method(std::move(method))
        , module(std::move(module))
        , credential(std::move(credential))
        , common(std::move(common))
        , need_verify(need_verify)
        , ignore_code(ignore_code)
        , proceduce_bool(proceduce_bool){};

    qqmusic::Task<qqmusic::Result<qqmusic::utils::buffer>> do_request(const nlohmann::json& params);

    /*parse json response*/
    qqmusic::Result<nlohmann::json> parse_response(qqmusic::utils::buffer& response);

private:
    std::string method;
    std::string module;
    nlohmann::json common;
    qqmusic::utils::Credential credential;
    bool need_verify;
    bool ignore_code;
    bool proceduce_bool;
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETIALS_API_H
