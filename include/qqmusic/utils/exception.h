/*-----------------utils/exception.h-----------------
 * api exception, used by class result to avoid throw
 * exceptions
 *--------------------------------------------------*/
#include <string>
#include <unordered_map>

namespace qqmusic::utils {

class Exception {
public:
    enum Kind {
        ResponseCodeError = 0,  /*Api returns an error response code*/
        CredendialExpiredError, /*Credential is out dated*/
        CredentialInvalidError, /*Credential is invalid*/
        LoginError,             /*Login failure*/
        SignInvalidError,       /*Request signature is invalid*/
        DataDestroy,            /*Data destroy during crypto, serialization,
                                deserialization or decoding*/
        RuntimeError,           /*Any runtime error including exceptions thrown by 3rd libs*/
        UnknownError
    };

    Exception(Kind kind)
        : kind_code(kind)
        , reason(ReasonKindMap[kind]) {}
    Exception(Kind kind, std::string_view reason)
        : kind_code(kind)
        , reason(reason) {}
    Exception(unsigned int http_rc)
        : kind_code(ResponseCodeError) {
        if (HttpResponseCodeMap.contains(http_rc)) {
            reason = HttpResponseCodeMap[http_rc];
        } else {
            reason = ReasonKindMap[Kind::ResponseCodeError];
        }
    }

    [[nodiscard]] std::string kind() const { return ReasonKindMap[kind_code]; }
    [[nodiscard]] std::string what() const { return reason; }

private:
    enum Kind kind_code;
    std::string reason;

    inline static std::string ReasonKindMap[UnknownError + 1] = {"ResponseCodeError",
                                                                 "CredendialExpiredError",
                                                                 "CredentialInvalidError",
                                                                 "LoginError",
                                                                 "SignInvalidError",
                                                                 "DataDestroy",
                                                                 "RuntimeError",
                                                                 "Unknown Error"};

    inline static std::unordered_map<unsigned, std::string> HttpResponseCodeMap = {
        {400u, "400 Bad Request"},
        {401u, "401 Unauthorized"},
        {403u, "403 Forbidden"},
        {404u, "404 Not Found"},
        {500u, "500 Internal Server Error"},
        {502u, "502 Bad Gateway"},
        {503u, "503 Service Unavailable"},
    };
};

} // namespace qqmusic::utils
