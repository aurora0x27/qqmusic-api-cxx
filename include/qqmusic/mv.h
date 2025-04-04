/**-----------------------------------------qqmusic/mv.h--------------------------------------------
 *
 * @file include/qqmusic/mv.h
 *
 * @brief 关于MV的Api
 *
 * @date 2025-4-2
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_MV_H
#define QQMUSIC_MV_H

#include <qqmusic/result.h>
#include <qqmusic/utils/async-executor.h>

namespace qqmusic {

/**
 * @struct MvUrl
 *
 * @brief Mv url封裝
 * */
struct MvUrl {
    std::string vid;                        ///< Mv的vid
    std::map<std::string, std::string> mp4; ///< mp4格式url
    std::map<std::string, std::string> hls; ///< hls格式url
};

/**
 * @brief 获取一组MV的信息
 *
 * @param vids 待查询的MV vid列表
 *
 * @return MV信息json
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_mv_detail(std::span<std::string> vids);

/**
 * @brief 获取mv url
 *
 * @param vids 待查询的MV vid列表
 *
 * @return MvUrl列表
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<std::vector<MvUrl>>> get_mv_urls(std::span<std::string> vids);

} // namespace qqmusic

#endif // !QQMUSIC_MV_H
