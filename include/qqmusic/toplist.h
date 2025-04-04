/**-------------------------------------qqmusic/toplist.h-------------------------------------------
 *
 * @file include/qqmusic/top.h
 *
 * @brief 排行榜相关API
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_TOP_H
#define QQMUSIC_TOP_H

#include <qqmusic/result.h>

namespace qqmusic {

/**
 * @brief 获取所有排行榜
 * */
Task<Result<nlohmann::json>> get_toplist_category();

/**
 * @brief 获取排行榜条目信息
 *
 * @param top_id 排行榜ID
 * @param num 返回数量, 默认为`10`
 * @param page 页码, 默认为`0`
 * @param tag 是否返回歌曲标签, 默认为`true`
 *
 * @return 
 * */
Task<Result<nlohmann::json>> get_toplist_item_detail(unsigned top_id,
                                                     unsigned num = 10,
                                                     unsigned page = 1,
                                                     bool tag = true);

} // namespace qqmusic

#endif // !QQMUSIC_TOP_H
