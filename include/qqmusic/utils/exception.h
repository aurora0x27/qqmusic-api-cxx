/**-------------------------------qqmusic/utils/exception.h-----------------------------------------
 *
 * @brief API异常类, 提供全局错误类型表示
 *
 * @date 2025-3-21
 *
 *------------------------------------------------------------------------------------------------*/
#include <string>
#include <unordered_map>

namespace qqmusic::utils {

/**
 * @brief 全局异常类
 * */
class Exception {
public:

    /**
     * @brief 错误类型枚举
     * */
    enum Kind {
        ResponseCodeError = 0,  //< Api returns an error response code
        CredendialExpiredError, //< Credential is out dated
        CredentialInvalidError, //< Credential is invalid
        LoginError,             //< Login failure
        SignInvalidError,       //< Request signature is invalid
        DataDestroy,            //< Data destroy during crypto, serialization, deserialization or decoding
        RuntimeError,           //< Any runtime error including exceptions thrown by 3rd libs
        NetworkError,           //< Unknown network error
        JsonError,              //< Error when handling json
        OperationOutOfTime,     //< Out of Time
        SslError,               //< Error when SSL/TLS operations
        UnknownError
    };

    /**
     * @brief 从枚举构造
     * */
    Exception(Kind kind)
        : kind_code(kind)
        , reason(ReasonKindMap[kind]) {}

    /**
     * @brief 从枚举构造并给出理由
     * */
    Exception(Kind kind, std::string_view reason)
        : kind_code(kind)
        , reason(reason) {}

    /**
     * @brief 从HTTP协议状态码构造
     * */
    Exception(unsigned int http_rc)
        : kind_code(ResponseCodeError) {
        if (HttpResponseCodeMap.contains(http_rc)) {
            reason = HttpResponseCodeMap[http_rc];
        } else {
            reason = ReasonKindMap[Kind::ResponseCodeError];
        }
    }

    /**
     * @brief 获取当前错误类型枚举
     * */
    [[nodiscard]] unsigned int get_error_enum() const { return kind_code; }

    /**
     * @brief 返回当前错误类型描述
     * */
    [[nodiscard]] std::string kind() const { return ReasonKindMap[kind_code]; }

    /**
     * @brief 返回错误原因
     * */
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
                                                                 "NetworkError",
                                                                 "JsonError",
                                                                 "OperationOutOfTime",
                                                                 "SslError",
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
