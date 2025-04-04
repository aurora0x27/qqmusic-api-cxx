/**--------------------------------------qqmusic/album.h--------------------------------------------
 *
 * @file include/qqmusic/album.h
 *
 * @brief 专辑相关Api
 *
 * @date 2025-4-2
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_ALBUM_H
#define QQMUSIC_ALBUM_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <string>
#include <string_view>

namespace qqmusic {

/**
 * @brief 封面类型枚举
 * */
enum class CoverSize : int { SMALL = 150, DEFAULT = 300, MIDIUM = 500, LARGE = 800 };

/**
 * @brief 获取专辑封面url
 *
 * @param mid 歌曲mid
 * @param size 歌曲封面尺寸
 * 
 * @return std::string格式歌曲封面的url
 *
 * */
std::string get_album_cover_url(std::string_view mid, CoverSize size);

/**
 * @brief 从mid获取专辑信息
 *
 * @param album_mid 专辑mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_detail(std::string_view album_mid);

/**
 * @brief 从id获取专辑信息
 *
 * @param album_id 专辑id
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_detail(uint64_t album_id);

/**
 * @brief 从mid获取专辑歌曲列表
 *
 * @param album_mid 专辑mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_songs(std::string_view album_mid,
                                                               unsigned int num = 10,
                                                               unsigned int page = 1);

/**
 * @brief 从id获取专辑歌曲列表
 *
 * @param album_id 专辑id
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
qqmusic::Task<qqmusic::Result<nlohmann::json>> get_album_songs(uint64_t album_id,
                                                               unsigned int num = 10,
                                                               unsigned int page = 1);

} // namespace qqmusic

#endif // !QQMUSIC_ALBUM_H
