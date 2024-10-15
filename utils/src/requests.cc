#include <json/value.h>
#include <json/json.h>
#include <utils/requests.h>
#include <map>
#include <utility>
#include <optional>
#include <string>
#include <stdexcept>
#include <variant>

const std::string qqmusic::utils::Api::QQMUSIC_API = "https://u.y.qq.com/cgi-bin/musicu.fcg";
const std::string qqmusic::utils::Api::QQMUSIC_VERSION = "13.2.5.8";
const int qqmusic::utils::Api::QQMUSIC_VERSION_CODE = 13020508;

qqmusic::utils::Cookies::Cookies() {

}

void qqmusic::utils::Cookies::setCookie(const std::string &name, const std::string &value) {
    cookies[name] = value;
}

std::string qqmusic::utils::Cookies::getCookie(const std::string &name) const {
    auto it = cookies.find(name);
    if (it != cookies.end()) {
        return it->second;
    }
    return "";
}

qqmusic::utils::Api::Api(const std::string &url, const std::string &method)
    : url(url), method(method), module("")
{
    params = {};
    data = {};
    headers = {};

    headers["User-Agent"] = "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54";

    json_body = false;
    verify = false;
    ignore_code = false;
    extra_common = {};
    credential = Credential();
    comment = "";
    _result = std::optional<Json::Value>(Json::objectValue);
    _post_init();
}

void qqmusic::utils::Api::_post_init() {
    if (module.empty()) {
        for (auto &c : method) c = toupper(c);
    }
    std::map<std::string, std::variant<bool, int ,std::string>> original_params = params;
    std::map<std::string, std::variant<bool, int ,std::string>> original_data = data;
    for (const auto &key : data) {
        data[key.first] = "";
    }
    for (const auto &key : params) {
        params[key.first] = "";
    }
    for (const auto &key : extra_common) {
        extra_common[key.first] = "";
    }
    std::optional<std::map<std::string, std::string>> _result = {};
    auto* _session = get_session();
}

void qqmusic::utils::Api::_setattr_(const std::string &name, const Json::Value &value) {
    if (name != "_result" && _result.has_value()) {
        _result.reset();
    }
}

qqmusic::utils::Api &qqmusic::utils::Api::update_params(const std::map<std::string, std::string> &kwargs) {
   _update_field(params, original_params, kwargs, "params");
   return *this; 
}

qqmusic::utils::Api &qqmusic::utils::Api::update_data(const std::map<std::string, std::string>& kwargs) {
    for (const auto& [key, value] : kwargs) {
        if (data.find(key) != data.end()) {
            data[key] = value;
        } else {
            throw std::invalid_argument(key + " not in data");
        }
    }
    return *this;
}

qqmusic::utils::Api &qqmusic::utils::Api::update_extra_common(const std::map<std::string, std::string>& kwargs) {
    _update_field(extra_common, extra_common, kwargs, "extra common");
    return *this;
}

qqmusic::utils::Api &qqmusic::utils::Api::update_headers(const std::map<std::string, std::string>& kwargs) {
    _update_field(headers, headers, kwargs, "headers");
    return *this;
}

qqmusic::utils::Api &qqmusic::utils::Api::update_cookies(const Cookies& cookies) {
    for (const auto& [key, value] : cookies.cookies) {
        qqmusic::utils::Cookies::cookies[key] = value;
    }
    return *this;
}

template <typename T, typename U, typename V>
void qqmusic::utils::Api::_update_field(T &field, const U &original, const V &updates, const std::string &field_name) {
    // field: 要更新的字段名
    // original: 原始字段
    // updates: 更新内容
    // field_name: 字段名称
    for (const auto& key : updates) {
        if(original.find(key.first) != original.end()) {
            field[key.first] = key.second;
        } else {
            throw std::runtime_error(key.first + " not in " + field_name);
        }
    }
}

void qqmusic::utils::Api::_prepare_params_data() {
    std::map<std::string, int> new_params = {};
    std::map<std::string, int> new_data = {};
    for (const auto& pair : params) {
        const std::string& key = pair.first;
        const auto& value = pair.second;
        if (std::holds_alternative<bool>(value)) {
            new_params[key] = std::get<bool>(value)? 1 : 0;
        } else if (std::holds_alternative<int>(value)) {
            new_params[key] = std::get<int>(value);
        }
    }
    
    for (const auto& pair : data) {
        const std::string& key = pair.first;
        const auto& value = pair.second;
        if (std::holds_alternative<bool>(value)) {
            new_data[key] = std::get<bool>(value)? 1 : 0;
        } else if (std::holds_alternative<int>(value)) {
            new_data[key] = std::get<int>(value);
        }
    }
    
    params.clear();
    data.clear();
    
    for (const auto& pair : new_params) {
        const std::string& key = pair.first;
        const int& value = pair.second;
        params[key] = std::to_string(value);
    }
    
    for (const auto& pair : new_data) {
        const std::string& key = pair.first;
        const int& value = pair.second;
        data[key] = std::to_string(value);
    }
    
}
