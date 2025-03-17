/*---------------------------qqmusic/details/api.h------------------------------
 * Api class prepare a request with provided infomation and global context
 *
 * private possessed by each api:
 *     - method
 *     - module
 *
 * globally shared by all apis:
 *     - credential
 *     - common
 *
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETIALS_API_H
#define QQMUSIC_DETIALS_API_H

#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/url.hpp>
#include <boost/url/param.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/details/context.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/credential.h>
#include <qqmusic/utils/session.h>

namespace qqmusic::details {

namespace http = boost::beast::http;
namespace asio = boost::asio;

struct RequestParam {
    boost::url url;
    http::request<http::string_body> req;
};

class Api {
public:
    Api(utils::Session& session,
        std::string module,
        std::string method,
        utils::Credential credential = {},
        nlohmann::json common = {{}})
        : session(session)
        , module(std::move(module))
        , method(std::move(method)){
        if (credential.is_valid()) {
            this->credential = credential;
        } else if (session.get_context_ref().credential.is_valid()) {
            this->credential = session.get_context_ref().credential;
        }
    };

    /*prepare request by given infomation*/
    qqmusic::Task<qqmusic::Result<RequestParam>> prepare_request(const nlohmann::json& params, bool verify = false);

    /*parse result buffer to json format*/
    qqmusic::Result<nlohmann::json> parse_response(utils::buffer&& response);

private:
    utils::Session& session;
    std::string module;
    std::string method;
    nlohmann::json common;
    utils::Credential credential;
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETIALS_API_H
