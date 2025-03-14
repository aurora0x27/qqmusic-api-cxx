/*------------------------------qqmusic/song.h----------------------------------
 * @file include/qqmusic/song.h
 *
 * @brief 歌曲相关API
 *
 * @details 根据mid, id获取歌曲信息和下载链接
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_SONG_H
#define QQMUSIC_SONG_H

#include <map>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>
#include <stdexcept>

namespace qqmusic {

/**
 * @class BaseMediaFileType
 *
 * @brief 媒体文件类型基类
 * */
class BaseMediaFileType {
public:
    /**
     * @brief 获取基类ID
     * */
    virtual std::string_view prefix() = 0;

    /**
     * @brief 获取扩展名
     * */
    virtual std::string_view expandation() = 0;

    /**
     * @brief 是否加密
     * */
    virtual bool encrypted() = 0;

    virtual ~BaseMediaFileType() = default;
};

/**
 * @class SongFileType
 *
 * @brief 未加密的歌曲文件类枚举
 *
 * @see BaseMediaFileType
 * */
class SongFileType : public BaseMediaFileType {
public:
    /**
     * @brief 未加密歌曲类型枚举
     * */
    enum Type {
        MASTER,   ///< 臻品母带2.0,24Bit 192kHz,size_new[0]
        ATMOS_2,  ///< 臻品全景声2.0,16Bit 44.1kHz,size_new[1]
        ATMOS_51, ///< 臻品音质2.0,16Bit 44.1kHz,size_new[2]
        FLAC,     ///< flac 格式,16Bit 44.1kHz~24Bit 48kHz,size_flac
        OGG_640,  ///< ogg 格式,640kbps,size_new[5]
        OGG_320,  ///< ogg 格式,320kbps,size_new[3]
        OGG_192,  ///< ogg 格式,192kbps,size_192ogg
        OGG_96,   ///< ogg 格式,96kbps,size_96ogg
        MP3_320,  ///< mp3 格式,320kbps,size_320mp3
        MP3_128,  ///< mp3 格式,128kbps,size_128mp3
        ACC_192,  ///< m4a 格式,192kbps,size_192aac
        ACC_96,   ///< m4a 格式,96kbps,size_96aac
        ACC_48,   ///< m4a 格式,48kbps,size_48aac
    };

    /**
     * @brief 通过类型枚举初始化
     *
     * @param type 类型枚举
     *
     * @see SongFileType::Type
     *
     * @throws 若提供的枚举不合法, 就抛出`std::runtime_error`
     * */
    SongFileType(Type type) {
        if (!table.contains(type)) {
            throw std::runtime_error("Unexpected SongFileType enum");
        }
        inner = table.at(type);
    };

    /**
     * @brief 默认初始化函数, 默认格式: MP3_128格式
     *
     * @see SongFileType::Type
     * */
    SongFileType()
        : qqmusic::SongFileType(MP3_128) {};

    ~SongFileType() override = default;

    /**
     * @brief 获取歌曲类ID前缀
     *
     * @return 返回`std::string`格式的类型前缀
     * */
    std::string_view prefix() override { return inner.start_id; };

    /**
     * @brief 获取歌曲类ID扩展
     *
     * @return 返回`std::string`格式的类型扩展名
     * */
    std::string_view expandation() override { return inner.extention; };

    /**
     * @brief 该格式是否加密
     *
     * @return 总是返回`false`
     * */
    bool encrypted() override { return false; };

private:
    struct CoreData {
        std::string start_id;
        std::string extention;
    } inner;

    inline static const std::map<Type, CoreData> table = {{MASTER, {"AI00", ".flac"}},
                                                          {ATMOS_2, {"Q000", ".flac"}},
                                                          {ATMOS_51, {"Q001", ".flac"}},
                                                          {FLAC, {"F000", ".flac"}},
                                                          {OGG_640, {"O801", ".ogg"}},
                                                          {OGG_320, {"O800", ".ogg"}},
                                                          {OGG_192, {"O600", ".ogg"}},
                                                          {OGG_96, {"O400", ".ogg"}},
                                                          {MP3_320, {"M800", ".mp3"}},
                                                          {MP3_128, {"M500", ".mp3"}},
                                                          {ACC_192, {"C600", ".m4a"}},
                                                          {ACC_96, {"C400", ".m4a"}},
                                                          {ACC_48, {"C200", ".m4a"}}};
};

/**
 * @class EncryptedSongFileType
 *
 * @brief 加密歌曲文件类型
 *
 * @see BaseMediaFileType
 * */
class EncryptedSongFileType : public BaseMediaFileType {
public:
    /**
     * @brief 加密歌曲类型枚举
     * */
    enum Type {
        MASTER,   //< MASTER: 臻品母带2.0,24Bit 192kHz,size_new[0]
        ATMOS_2,  //< ATMOS_2: 臻品全景声2.0,16Bit 44.1kHz,size_new[1]
        ATMOS_51, //< ATMOS_51: 臻品音质2.0,16Bit 44.1kHz,size_new[2]
        FLAC,     //< FLAC: mflac 格式,16Bit 44.1kHz~24Bit 48kHz,size_flac
        OGG_640,  //< OGG_640: mgg 格式,640kbps,size_new[5]
        OGG_320,  //< OGG_320: mgg 格式,320kbps,size_new[3]
        OGG_192,  //< OGG_192: mgg 格式,192kbps,size_192ogg
        OGG_96,   //< OGG_96: mgg 格式,96kbps,size_96ogg
    };

    /**
     * @brief 通过类型枚举初始化
     *
     * @param type 类型枚举
     *
     * @see EncryptedSongFileType::Type
     *
     * @throws 若提供的枚举不合法, 就抛出`std::runtime_error`
     * */
    EncryptedSongFileType(Type type) {
        if (!table.contains(type)) {
            throw std::runtime_error("SongFileType -- Ilegal enum type");
        }
        inner = table.at(type);
    };

    /**
     * @brief 默认初始化函数, 默认格式: FLAC格式
     *
     * @see SongFileType::Type
     * */
    EncryptedSongFileType()
        : EncryptedSongFileType(FLAC) {};

    ~EncryptedSongFileType() override = default;

    /**
     * @brief 获取歌曲类ID前缀
     *
     * @return 返回`std::string`格式的类型前缀
     * */
    std::string_view prefix() override { return inner.start_id; };

    /**
     * @brief 获取歌曲类ID扩展
     *
     * @return 返回`std::string`格式的类型扩展名
     * */
    std::string_view expandation() override { return inner.extention; };

    /**
     * @brief 该格式是否加密
     *
     * @return 总是返回`true`
     * */
    bool encrypted() override { return true; };

private:
    struct CoreData {
        std::string start_id;
        std::string extention;
    } inner;

    inline static const std::map<Type, CoreData> table = {
        {MASTER, {"AIM0", ".mflac"}},
        {ATMOS_2, {"Q0M0", ".mflac"}},
        {ATMOS_51, {"Q0M1", ".mflac"}},
        {FLAC, {"F0M0", ".mflac"}},
        {OGG_640, {"O801", ".mgg"}},
        {OGG_320, {"O800", ".mgg"}},
        {OGG_192, {"O6M0", ".mgg"}},
        {OGG_96, {"O4M0", ".mgg"}},
    };
};

/**
 * @brief 批量查询歌曲信息
 *
 * @param mids 待查询歌曲mid列表
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> query_song(std::span<std::string> mids);

/**
 * @brief 批量查询歌曲信息
 *
 * @param mids 待查询歌曲id列表
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> query_song(std::span<uint64_t> ids);

/**
 * @struct SongInfo
 *
 * @var SongInfo::mid 歌曲mid
 * @var SongInfo url 歌曲文件地址, 如果没有则是`std::nullopt`
 * @var SongInfo ekey 歌曲密钥, 如果没有则是`std::nullopt`
 * */
struct SongInfo {
    std::string mid;
    std::optional<std::string> url = std::nullopt;
    std::optional<std::string> ekey = std::nullopt;
};

/**
 * @brief 批量获取歌曲播放链接
 *
 * @params mids 歌曲mid列表
 * @params file_type 歌曲文件类型, 默认MP3_128类型
 * @params credential 凭证信息
 *
 * 调用示例: 
 * @code{.cpp}
 * std::vector<std::string> mids{"002n5zzy3VVGau"};
 *
 * // 未加密类型
 * auto res = co_await get_song_urls(mids,
 *     std::make_unique<SongFileType>(SongFileType::ACC_192));
 *
 * // 加密文件类型 
 * auto res = co_await get_song_urls(mids,
 *     std::make_unique<EncryptedSongFileType>(EncryptedSongFileType::MASTER));
 * @endcode
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @see BaseMediaFileType, SongFileType, EncryptedSongFileType
 * @note 异步函数, 返回`Task`
 */
Task<Result<std::vector<SongInfo>>> get_song_urls(
    std::span<std::string> mids,
    std::unique_ptr<BaseMediaFileType> file_type = std::make_unique<SongFileType>(),
    std::optional<utils::Credential> credential = std::nullopt);

/**
 * @brief 获取试听链接
 *
 * @param area 区域信息筛选, 默认`AreaType::ALL`.
 * @param gender 性别信息筛选, 默认`GenderType::ALL`.
 * @param genre 风格信息筛选, 默认`GenreType::ALL`.
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @see AreaType, GenderType, GenreType
 * @note 异步函数, 返回`Task`
 */
Task<Result<std::string>> get_try_url(std::string_view mid, std::string_view vs);

/**
 * @brief 获取歌曲详细信息
 *
 * @param mid 歌曲mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_song_detail(std::string_view mid);

/**
 * @brief 获取歌曲详细信息
 *
 * @param id 歌曲id
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_song_detail(uint64_t id);

/**
 * @brief 获取相似歌曲列表
 *
 * @param id 歌曲id
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_similar_songs(uint64_t songid);

/**
 * @brief 获取歌曲标签信息
 *
 * @param id 歌曲id
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_labels(uint64_t songid);

/**
 * @brief 获取相关歌单列表
 *
 * @param id 歌曲id
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_related_songlist(uint64_t songid);

/**
 * @brief 获取相关MV
 *
 * @param id 歌曲id
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_related_mv(uint64_t songid,
                                            std::optional<std::string> last_mvid = std::nullopt);

/**
 * @brief 获取同名歌曲的其他版本
 *
 * @param mid 歌曲mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_other_version(std::string_view mid);

/**
 * @brief 获取同名歌曲的其他版本
 *
 * @param id 歌曲id
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_other_version(uint64_t id);

/**
 * @brief 获取歌曲制作者信息
 *
 * @param mid 歌曲mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 可能返回JSON的`null`, 即没有制作者信息
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_producer_info(std::string_view mid);

/**
 * @brief 获取歌曲制作者信息
 *
 * @param id 歌曲id
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 可能返回JSON的`null`, 即没有制作者信息
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_producer_info(uint64_t id);

/**
 * @brief 获取曲谱信息
 *
 * @param mid 歌曲mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_sheet(std::string_view mid);

/**
 * @brief 获取歌曲的收藏量
 *
 * @param id_list 歌曲id列表
 * @param readable 是否人类可读
 *
 * 示例:
 *
 * @code{.txt}
 * readable == false 返回: {'438910555': 1000001}
 * readable == true 返回:  {'438910555': '550w+'}
 * @endcode
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_fav_num(std::span<uint64_t> id_list, bool readable = true);

} // namespace qqmusic

#endif // !QQMUSIC_SONG_H
