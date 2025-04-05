/**---------------------------------------qqmusic/user.h--------------------------------------------
 *
 * @file include/qqmusic/user.h 
 *
 * @brief 用户相关Api
 *
 * @date 2025-4-5
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_USER_H
#define QQMUSIC_USER_H

#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>
#include <string_view>

namespace qqmusic {

/**
 * @brief 由musicid获取euin
 *
 * @param musicid MusicID, 通常是QQ号 
 *
 * @return Euin
 * */
Task<Result<std::string>> get_euin(uint64_t musicid);

/**
 * @brief 由euin获取musicid
 *
 * @param euin encrypt_uin
 *
 * @return musicid
 * */
Task<Result<uint64_t>> get_musicid(std::string_view euin);

/**
 * @brief 获取用户主页信息
 *
 * @param euin encrypt_uin
 * @param credential 凭据信息
 *
 * @return 用户主页信息, 包含音乐基因, 歌单等
 * */
Task<Result<nlohmann::json>> get_homepage(
    std::string_view euin, std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取当前账号vip信息
 *
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_vip_info(std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取关注的歌手列表
 *
 * @param encrypt_uin
 * @param page 页码, 默认`1`
 * @param num 返回数量, 默认`10`
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_follow_singers(
    std::string_view euin,
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 粉丝数量
 *
 * @param encrypt_uin
 * @param page 页码, 默认`1`
 * @param num 返回数量, 默认`10`
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_fans(std::string_view euin,
                                      unsigned page = 1,
                                      unsigned num = 10,
                                      std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取好友列表
 *
 * @param page 页码, 默认`1`
 * @param num 返回数量, 默认`10`
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_friend_list(
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取关注用户列表
 *
 * @param encrypt_uin
 * @param page 页码, 默认`1`
 * @param num 返回数量, 默认`10`
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_follow_usr(
    std::string_view euin,
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取创建的歌单列表
 *
 * @param musicid, 通常是QQ号
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_created_songlist(
    uint64_t musicid, std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取收藏歌曲
 *
 * @param encrypt_uin
 * @param page 页码, 默认`1`
 * @param num 返回数量, 默认`10`
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_fav_song(std::string_view euin,
                                          unsigned page = 1,
                                          unsigned num = 10,
                                          std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取收藏歌单
 *
 * @param encrypt_uin
 * @param page 页码, 默认`1`
 * @param num 返回数量, 默认`10`
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_fav_songlist(
    std::string_view euin,
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取收藏专辑
 *
 * @param encrypt_uin
 * @param page 页码, 默认`1`
 * @param num 返回数量, 默认`10`
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_fav_album(
    std::string_view euin,
    unsigned page = 1,
    unsigned num = 10,
    std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取收藏MV
 *
 * @param encrypt_uin
 * @param page 页码, 默认`1`
 * @param num 返回数量, 默认`10`
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_fav_mv(std::string_view euin,
                                        unsigned page = 1,
                                        unsigned num = 10,
                                        std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取音乐基因
 *
 * @param euin encrypt_uin
 * @param credential 凭据信息
 * */
Task<Result<nlohmann::json>> get_music_gene(
    std::string_view euin, std::optional<utils::Credential> credential = std::nullopt);

} // namespace qqmusic

#endif // !QQMUSIC_USER_H
