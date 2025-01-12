/*-------------utils/credential.h---------------
 * credential used for identification
*---------------------------------------------*/
#ifndef QQMUSIC_UTILS_CREDENTIAL_H
#define QQMUSIC_UTILS_CREDENTIAL_H

#include <json/json.h>
#include <map>
#include <stdexcept>
#include <string>

namespace qqmusic::utils {
// 自定义异常类，用于处理凭据无效的情况
class CredentialInvalidError : public std::runtime_error {
public:
    explicit CredentialInvalidError(const std::string& message)
        : std::runtime_error(message) {}
};
// Credential 类用于存储和管理凭据信息
class Credential {
public:
    // 构造函数
    Credential();

    // 成员变量
    std::string openid;                              // 用户的 openid
    std::string refresh_token;                       // 刷新令牌
    std::string access_token;                        // 访问令牌
    int64_t expired_at;                              // 过期时间
    int musicid;                                     // 音乐 ID
    std::string musickey;                            // 音乐密钥
    std::string unionid;                             // 用户的 unionid
    std::string str_musicid;                         // 音乐 ID 的字符串表示
    std::string refresh_key;                         // 刷新密钥
    std::string encrypt_uin;                         // 加密的 UIN
    int login_type;                                  // 登录类型
    std::map<std::string, std::string> extra_fields; // 额外字段

    // 成员函数
    void post_init();               // 初始化后设置登录类型
    bool has_musicid() const;       // 检查是否提供 musicid
    bool has_musickey() const;      // 检查是否提供 musickey
    void raise_for_invalid() const; // 检查凭据有效性
    bool refresh();                 // 刷新凭据
    bool can_refresh();             // 判断凭据是否可刷新
    bool is_expired();              // 判断凭据是否过期
    Json::Value as_dict() const;    // 获取凭据字典
    std::string as_json() const;    // 获取 JSON 字符串
    static Credential from_cookies_dict(
        const std::map<std::string, std::string>& cookies); // 从 cookies 创建凭据

private:
    bool starts_with(const std::string& str,
                     const std::string& prefix) const; // 检查字符串是否以特定前缀开头
};
} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_CREDENTIAL_H
