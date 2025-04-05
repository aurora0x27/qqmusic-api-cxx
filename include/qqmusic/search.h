/**--------------------------------------qqmusic/search.h-------------------------------------------
 *
 * @file include/qqmusic/search.h
 *
 * @brief 搜索相关Api
 *
 * @date 2025-4-4
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_SEARCH_H
#define QQMUSIC_SEARCH_H

#include <qqmusic/result.h>
#include <string_view>

namespace qqmusic {

/**
 * @brief 搜索类型枚举
 * */
enum class SearchType : unsigned {
    SONG = 0,         ///< 歌曲
    SINGER = 1,       ///< 歌手
    ALBUM = 2,        ///< 专辑
    SONG_LIST = 3,    ///< 歌单
    MV = 4,           ///< MV
    LYRIC = 7,        ///< 歌词
    USER = 8,         ///< 用户
    AUDIO_ALBUM = 15, ///< 节目专辑
    AUDIO = 18,       ///< 节目
};

/**
 * @brief 获取搜索热词
 *
 * @return Json格式的搜索热词
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> hotkey();

/**
 * @brief 搜索词联想补全
 *
 * @param keyword 当前搜索词
 *
 * @return 带有高亮的补全结果
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> keyword_complete(std::string_view keyword);

/**
 * @brief 快速搜索
 *
 * @param keyword 搜索词
 *
 * @return Json格式搜索结果
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> quick_search(std::string_view keyword);

/**
 * @brief 综合搜索
 *
 * @param keyword 搜索词
 * @param num 每页结果数, 默认`15`
 * @param page 页数, 默认`1`
 * @param highlight 是否高亮, 默认`true`
 *
 * @return Json格式搜索结果
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> general_search(std::string_view keyword,
                                            unsigned num = 15,
                                            unsigned page = 1,
                                            bool highlight = true);

/**
 * @brief 按类型搜索
 *
 * @param keybord 搜索词
 * @param type 搜索类型
 * @param num_per_page 每页结果数
 * @param page 页数
 *
 * @return Json格式搜索结果
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> search_by_type(std::string_view keyword,
                                            SearchType type = SearchType::SONG,
                                            unsigned num_per_page = 10,
                                            unsigned page = 1,
                                            bool highlight = true);

} // namespace qqmusic

#endif // !QQMUSIC_SEARCH_H
