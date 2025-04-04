/**------------------------------------qqmusic/songlist.h-------------------------------------------
 * @file include/qqmusic/songlists.h
 *
 * @brief 歌单相关API
 *
 * @details 根据dirid对歌单进行增删
 *
 * @warn 歌单名称不应该包含表情包等特殊字符, 否则会导致实际操作完成, 
 * 但是json解析返回值错误
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_SONG_LIST_H
#define QQMUSIC_SONG_LIST_H

#include <cstdint>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic {

/**
 * @brief 获取歌单信息
 *
 * @param dissid 歌单ID
 * @param dirid 歌单drid, 默认为`0`
 * @param num 返回songlist元素数量, 默认为`10`
 * @param page 第几页, 默认为`1`
 * @param only_song 是否仅返回歌曲信息, 默认`false`
 * @param tag 是否返回歌单标签信息, 默认`true`
 * @param user_info 是否返回歌单创建者的用户信息, 默认`true`
 *
 * @return Json格式的歌单信息
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_songlist_detail(uint64_t dissid,
                                                 unsigned dirid = 0,
                                                 unsigned num = 10,
                                                 unsigned page = 1,
                                                 bool only_song = false,
                                                 bool tag = true,
                                                 bool user_info = true);

/**
 * @brief 获取歌单列表
 *
 * @param dissid 歌单ID
 * @param dirid 歌单dirid, 默认为0
 *
 * @return Json格式歌单列表
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_songlist(uint64_t dissid, unsigned dirid = 0);

/**
 * @brief 创建歌单
 *
 * @param dirname 新的歌单名称
 * @param credential 凭据信息
 *
 * @return 创建歌单的基本信息
 *
 * @note 异步函数, 返回`Task`
 *
 * @warning 歌单名称不应该含有除了正常字符之外的东西, 例如emoji, 
 *          这将导致歌单虽然被正确创建, 但是返回时json解析异常报错
 * */
Task<Result<nlohmann::json>> songlist_create(
    std::string_view dirname, std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 删除歌单
 *
 * @param dirid 歌单ID
 * @param credential 凭据信息
 *
 * @return 是否删除成功, 如果不存在这个歌单则返回`false`
 * */
Task<Result<bool>> songlist_delete(uint64_t dirid,
                                   std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 向歌单内添加歌曲
 *
 * @param dirid 歌单ID
 * @param song_ids 待添加歌单列表
 * @param credential 凭据信息
 *
 * @return 是否添加成功, 歌曲已存在则返回`false`
 * */
Task<Result<bool>> songlist_add_songs(uint64_t dirid,
                                      std::span<uint64_t> song_ids,
                                      std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 删除歌单内指定歌曲
 *
 * @param dirid 歌单ID
 * @param song_ids 待删除歌单列表
 * @param credential 凭据信息
 *
 * @return 是否删除成功, 歌曲不存在则返回`false`
 * */
Task<Result<bool>> songlist_delete_songs(uint64_t dirid,
                                         std::span<uint64_t> song_ids,
                                         std::optional<utils::Credential> credential = std::nullopt);
} // namespace qqmusic

#endif // !QQMUSIC_SONG_LIST_H
