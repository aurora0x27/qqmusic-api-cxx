/**----------------------------qqmusic/songlist.h-------------------------------
 * @file include/qqmusic/songlists.h
 *
 * @brief 歌单相关API
 *
 * @details 根据dirid对歌单进行增删
 *
 * @warn 歌单名称不应该包含表情包等特殊字符, 否则会导致实际操作完成, 
 * 但是json解析返回值错误
 *----------------------------------------------------------------------------*/
#ifndef QQMUSIC_SONG_LIST_H
#define QQMUSIC_SONG_LIST_H

#include <cstdint>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic {

Task<Result<nlohmann::json>> get_songlist_detail(uint64_t songlist_id,
                                                 unsigned dirid = 0,
                                                 unsigned num = 10,
                                                 unsigned page = 1,
                                                 bool only_song = false,
                                                 bool tag = true,
                                                 bool user_info = true);

Task<Result<nlohmann::json>> get_songlist(uint64_t songlist_id, unsigned dirid);

Task<Result<nlohmann::json>> songlist_create(
    std::string_view dirname, std::optional<utils::Credential> credential = std::nullopt);

Task<Result<bool>> songlist_delete(uint64_t dirid,
                                   std::optional<utils::Credential> credential = std::nullopt);

Task<Result<bool>> songlist_add_songs(uint64_t dirid,
                                      std::span<uint64_t> song_ids,
                                      std::optional<utils::Credential> credential = std::nullopt);

Task<Result<bool>> songlist_delete_songs(uint64_t dirid,
                                         std::span<uint64_t> song_ids,
                                         std::optional<utils::Credential> credential = std::nullopt);
} // namespace qqmusic

#endif // !QQMUSIC_SONG_LIST_H
