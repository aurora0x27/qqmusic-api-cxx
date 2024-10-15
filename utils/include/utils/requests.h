//-------------utils/requests.h---------------
//            http requests class
#ifndef QRC_DECODER_H
#define QRC_DECODER_H

#include <string>
#include <map>
#include <stdexcept>
#include <json/json.h> 
#include <curl/curl.h> 
#include <optional>
#include <variant>

namespace qqmusic::utils {

class CredentialExpiredError : public std::runtime_error {
public:
    explicit CredentialExpiredError(const Json::Value &data, const Json::Value &response)
        : std::runtime_error("Credential expired"), data(data), response(response) {}
    Json::Value data;
    Json::Value response;
};

class ResponseCodeError : public std::runtime_error {
public:
    ResponseCodeError(int code, const Json::Value &data, const Json::Value &response)
        : std::runtime_error("Response code error"), code(code), data(data), response(response) {}
    int code;
    Json::Value data;
    Json::Value response;
};

class Credential {
public:
    bool has_musicid() const;
    bool has_musickey() const;
    std::string musicid;
    std::string musickey;
    int login_type;
};

class QIMEI {
public:
    static std::string get_qimei(const std::string &version);//获取 QIMEI
};

class Cookies {
    public:
        Cookies();
        void setCookie(const std::string &name, const std::string &value);
        std::string getCookie(const std::string &name) const;
        static std::map<std::string, std::string> cookies;
    private:
};

class Api {
public:
    Api(const std::string &url = QQMUSIC_API, const std::string &method = "GET");

    Api &update_params(const std::map<std::string, std::string> &kwargs);
    Api &update_data(const std::map<std::string, std::string> &kwargs);
    Api &update_extra_common(const std::map<std::string, std::string> &kwargs);
    Api &update_headers(const std::map<std::string, std::string> &kwargs);
    Api &update_cookies(const Cookies &cookies);

    Json::Value request();//发起请求
    Json::Value fetch();//发起请求并处理响应
    Json::Value result();//获取请求结果

private:
    void _post_init();
    void _setattr_(const std::string &name, const Json::Value &value);

    template <typename T,typename U,typename V>
    void _update_field(T &field, const U &original, const V &updates, const std::string &field_name);
    void _prepare_params_data();//准备请求参数
    void _prepare_api_data();//准备API请求数据
    void _prepare_credential();//准备凭证
    Json::Value _prepare_request();//准备请求
    void _process_response(const Json::Value &resp);//处理响应
    void _process_response_code(const Json::Value &resp);//处理响应代码

    static std::string get_qimei();
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

    static const std::string QQMUSIC_API;
    static const std::string QQMUSIC_VERSION;
    static const int QQMUSIC_VERSION_CODE;
    static std::map<std::string, std::variant<bool, int, std::string>> original_params;
    static std::map<std::string, std::variant<bool, int, std::string>> original_data;
    //static const auto QIMEI36;
    //static const Json::Vaule _SESSION_POOL;
    //static const Json::Value HEADERS;

    std::string url;//请求地址
    std::string method;//请求方法
    std::string module;//请求模块
    std::map<std::string, std::variant<bool, int, std::string>> params;//请求参数
    std::map<std::string, std::variant<bool, int, std::string>> data;//请求数据
    std::map<std::string, std::string> headers;//请求头
    bool json_body = false;//是否以json格式发送请求数据
    bool verify = false;//是否验证凭据
    enum class platform;//平台类型
    bool ignore_code = false;//是否忽略返回值 code 检查直接返回
    std::map<std::string, std::string> extra_common;//额外公共参数                            
    Credential credential;//账号凭据
    std::string comment;//请求注释
    std::optional<Json::Value> _result;//请求结果
};

void clean();//程序退出清理操作
void *get_session();//获取当前 EventLoop 的 Session,用于自定义请求
void set_session();//设置当前 EventLoop 的 Session                

} // namespace qqmusic::utils

#endif
