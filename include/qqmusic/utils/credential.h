/**----------------------------------------utils/credential.h---------------------------------------
 *
 * @brief QQ音乐账号凭据, 部分和会员, 用户相关的Api需要依靠传入凭据才能生效, 凭据由`Login`部分的
 *        Api获取
 *
 * @date 2025-3-21
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_CREDENTIAL_H
#define QQMUSIC_UTILS_CREDENTIAL_H

#include <cstdint>
#include <nlohmann/detail/macro_scope.hpp>
#include <qqmusic/result.h>
#include <string>

namespace qqmusic::utils {

/**
 * @brief 凭据信息类
 * */
class Credential {
public:
    /**
     * @brief 默认空初始化
     * */
    Credential() = default;

    /**
     * @brief 从login api获取的返回结果构造Credential
     *
     * @param cookie Json字符串
     * */
    explicit Credential(std::string_view cookie);

    /**
     * @brief 从login api返回的结果构造Credential
     *
     * @param cookie Json对象
     * */
    explicit Credential(nlohmann::json& cookie);

    /**
     * @brief 从序列化出的cache生成Credential
     *
     * @param cache 由`Credential::to_string()`和`Credential::to_json()`
     *              生成的序列化结果导入Credential
     *
     * @return `true`, 如果处理过程出现问题
     *         `false`, 如果正常处理完毕
     * */
    bool from_cache(std::string_view cache);

    /**
     * @brief 从序列化出的cache生成Credential
     *
     * @param cache 由`Credential::to_string()`和`Credential::to_json()`
     *              生成的序列化结果导入Credential
     *
     * @return `true`, 如果处理过程出现问题
     *         `false`, 如果正常处理完毕
     * */
    bool from_cache(nlohmann::json& cache);

    /**
     * @brief 判断Credential是否合法, 即有没有`musicid`和`musickey`字段
     *
     * @return `true`, 合法
     *         `false`, 不合法
     * */
    bool is_valid() const;

    /**
     * @brief 判断Credential是否过期
     *
     * @return 包含正确JSON格式结果的`Task<Result<bool>>`.
     *         如果产生错误, 返回值包含错误结果和错误类型枚举
     *         返回`true`说明未过期, `false`说明过期, 需要刷新
     *
     * @note 异步函数, 返回`Task`
     * */
    qqmusic::Task<qqmusic::Result<bool>> is_expired();

    /**
     * @brief 刷新Credential
     *
     * @return 包含正确JSON格式结果的`Task<Result<void>>`.
     *         如果产生错误, 返回值包含错误结果和错误类型枚举
     *
     * @note 异步函数, 返回`Task`
     * */
    qqmusic::Task<qqmusic::Result<void>> refresh();

    /**
     * @brief 将Credential序列化为Json字符串
     *
     * @return 包含结果或错误的`Result<std::string>`
     * */
    qqmusic::Result<std::string> to_string();

    /**
     * @brief 将Credential序列化为Json对象
     *
     * @return 包含结果或错误的`Result<nlohmann::json>`
     * */
    qqmusic::Result<nlohmann::json> to_json();

    /**
     * @brief OpenID
     * */
    std::string openid;

    /**
     * @brief RefreshToken
     * */
    std::string refresh_token;

    /**
     * @brief AccessToken
     * */
    std::string access_token;

    /**
     * @brief 到期时间, 秒级时间戳
     * */
    uint64_t expired_at = 0;

    /**
     * @brief 账号MusicID, 一般是QQ号
     *
     * @note 常用字段
     * */
    uint64_t musicid = 0;

    /**
     * @brief 账号MusicKey
     *
     * @note 常用
     * */
    std::string musickey;

    /**
     * @brief UnionID
     * */
    std::string unionid;

    /**
     * @brief std::string格式的账号MusicID, 一般是QQ号
     *
     * @note 常用字段
     * */
    std::string str_musicid;

    /**
     * @brief RefreshKey
     * */
    std::string refresh_key;

    /**
     * @brief 加密的euin, euin和账号ID一一对应
     * */
    std::string encryptUin;

    /**
     * @brief 登录模式
     *        `1`: 微信登陆
     *        `2`: QQ登陆
     * */
    int loginType = 2;

    /**
     * @brief 没有收录在上面的字段
     * */
    nlohmann::json extra_fields;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Credential,
                                   openid,
                                   refresh_token,
                                   access_token,
                                   expired_at,
                                   musicid,
                                   unionid,
                                   str_musicid,
                                   musickey,
                                   refresh_key,
                                   encryptUin,
                                   loginType);
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_CREDENTIAL_H
