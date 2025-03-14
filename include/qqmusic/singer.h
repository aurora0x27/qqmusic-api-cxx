/**------------------------------qqmusic/singer.h-------------------------------
 * @file include/qqmusic/singer.h
 *
 * @brief 歌手相关API
 *
 * @details 按区域, 风格, 性别和索引以及Tab来筛选歌手, 获取歌单, 专辑, 歌曲等信息
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_SINGER_H
#define QQMUSIC_SINGER_H

#include <qqmusic/result.h>
#include <stdexcept>
#include <unordered_map>

namespace qqmusic {

/**
 * @brief 区域类型枚举
 */
enum class AreaType : int {
    ALL = -100,
    CHINA = 200,
    TAIWAN = 2,
    AMERICA = 5,
    JAPAN = 4,
    KOREA = 3
};

/**
 * @brief 风格类型枚举
 */
enum class GenreType : int {
    ALL = -100,
    POP = 7,
    RAP = 3,
    CHINESE_STYLE = 19,
    ROCK = 4,
    ELECTRONIC = 2,
    FOLK = 8,
    R_AND_B = 11,
    ETHNIC = 37,
    LIGHT_MUSIC = 93,
    JAZZ = 14,
    CLASSICAL = 33,
    COUNTRY = 13,
    BLUES = 10
};

/**
 * @brief 性别类型枚举
 * */
enum class GenderType : int {
    ALL = -100,
    MALE = 0,
    FEMALE = 1,
    GROUP = 2,
};

/**
 * @brief 索引类型枚举, 按首字母筛选时需要
 * */
enum class IndexType : int {
    A = 1,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    HASH,
    ALL = -100
};

/**
 * @class TabType
 *
 * @brief 标签类
 */
class TabType {
public:
    /**
     * @brief 标签类型枚举
     */
    enum Type {
        WIKI,     ///< wiki
        ALBUM,    ///< 专辑
        COMPOSER, ///< 编曲
        LYRICIST, ///< 作词
        PRODUCER, ///< 制作
        ARRANGER,
        MUSICIAN, ///< 演唱
        SONG,     ///< 歌曲
        VIDEO     ///< 视频
    };

    /**
     * @brief 使用枚举值初始化标签类
     *
     * @param type 标签类型枚举
     *
     * @throws 若提供的枚举值不合法, 则抛出`std::runtime_error`
     */
    TabType() = delete;
    TabType(Type type) {
        auto it = tab_data.find(type);
        if (it == tab_data.end()) {
            throw std::runtime_error("Unexpected TabType enum");
        }
        this->inner = it->second;
    }

    /**
     * @brief 获取tab id
     *
     * @return std::string格式tab id
     */
    std::string get_id() const { return inner.tab_id; }

    /**
     * @brief 获取tab名称
     *
     * @return std::string格式tab name
     */
    std::string get_name() const { return inner.tab_name; }

private:
    /**
     * @brief 标签信息结构
     */
    struct Tab {
        std::string tab_id;   ///< tab ID
        std::string tab_name; ///< tab 名称
    } inner;

    /**
     * @brief 枚举值和标签内容的映射表初始化
     */
    inline static const std::unordered_map<TabType::Type, TabType::Tab> tab_data = {
        {TabType::WIKI, {"wiki", "IntroductionTab"}},
        {TabType::ALBUM, {"album", "AlbumTab"}},
        {TabType::COMPOSER, {"song_composing", "SongTab"}},
        {TabType::LYRICIST, {"song_lyric", "SongTab"}},
        {TabType::PRODUCER, {"producer", "SongTab"}},
        {TabType::ARRANGER, {"arranger", "SongTab"}},
        {TabType::MUSICIAN, {"musician", "SongTab"}},
        {TabType::SONG, {"song_sing", "SongTab"}},
        {TabType::VIDEO, {"video", "VideoTab"}},
    };
};

/**
 * @brief 获取符合筛选条件的歌手信息表单
 *
 * 获取JSON格式的歌手信息, 可以使用area, gender和genre筛选
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
Task<Result<nlohmann::json>> get_singer_list(AreaType area = AreaType::ALL,
                                             GenderType gender = GenderType::ALL,
                                             GenreType genre = GenreType::ALL);

/**
 * @brief 筛选获得歌曲歌单原始数据
 *
 * @param area 区域信息筛选, 默认`AreaType::ALL`.
 * @param gender 性别信息筛选, 默认`GenderType::ALL`.
 * @param genre 风格信息筛选, 默认`GenreType::ALL`.
 * @param sin 跳过的条目数量, 默认`0`
 * @param cur_page 当前页面的索引, 默认`1`
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @see AreaType, GenderType, GenreType, IndexType
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_singer_list_index(AreaType area = AreaType::ALL,
                                                   GenderType gender = GenderType::ALL,
                                                   GenreType genre = GenreType::ALL,
                                                   IndexType index = IndexType::ALL,
                                                   unsigned sin = 0,
                                                   unsigned cur_page = 1);

/**
 * @brief 获取所有符合条件的歌单原始数据
 *
 * @param area 区域信息筛选, 默认`AreaType::ALL`.
 * @param gender 性别信息筛选, 默认`GenderType::ALL`.
 * @param genre 风格信息筛选, 默认`GenreType::ALL`.
 * @param index 索引信息筛选, 默认`IndexType::ALL`.
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @see AreaType, GenderType, GenreType, IndexType
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_singer_list_index_all(AreaType area = AreaType::ALL,
                                                       GenderType gender = GenderType::ALL,
                                                       GenreType genre = GenreType::ALL,
                                                       IndexType index = IndexType::ALL);

/**
 * @brief 获取歌手基本信息
 *
 * @param 歌手mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_singer_info(std::string_view mid);

/**
 * @brief 返回标签详细信息
 *
 * @param mid 歌手mid
 * @param type 标签信息
 * @param page 页码
 * @param num 每页返回数量
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @see TabType
 * @note 异步函数, 返回`Task`
 */
Task<Result<nlohmann::json>> get_tab_detail(std::string_view mid,
                                            TabType type,
                                            unsigned page = 1,
                                            unsigned num = 10);

/**
 * @brief 获取多个歌手的简介
 *
 * @param mids 歌手mid列表
 * @param number 相似歌手数量, 默认为`0`
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @see TabType
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_singer_desc(std::span<std::string> mids, unsigned number = 10);

/**
 * @brief 获取相似歌手信息
 *
 * @param mids 歌手mid
 * @param number 相似歌手数量, 默认为`0`
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @see TabType
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_similar_singers(std::string_view mid, unsigned number = 10);

/**
 * @brief 获取歌手歌曲原始数据
 *
 * @param mid 歌手mid
 * @param num 每次获取的最大数量, 默认`10`, 最大`30`
 * @param begin 从第几个开始
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_songs_list(std::string_view mid,
                                            unsigned num = 10,
                                            unsigned begin = 0);

/**
 * @brief 获取歌手的所有歌曲
 *
 * @param mid 歌手mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_songs_list_all(std::string_view mid);

/**
 * @brief 获取歌手专辑列表
 *
 * @param mid 歌手mid
 * @param number 单次获取数据量, 默认`10`
 * @param begin 从第几个开始
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_album_list(std::string_view mid,
                                            unsigned number = 10,
                                            unsigned begin = 0);

/**
 * @brief 获取歌手所有专辑
 *
 * @param mid 歌手mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_album_list_all(std::string_view mid);

/**
 * @brief 获取歌手MV列表
 *
 * @param mid 歌手mid
 * @param number 单次获取数据量, 默认`10`
 * @param begin 从第几个开始
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_mv_list(std::string_view mid,
                                         unsigned number = 10,
                                         unsigned begin = 0);

/**
 * @brief 获取歌手所有MV
 *
 * @param mid 歌手mid
 *
 * @return 包含正确JSON格式结果的`Task<Result<nlohmann::json>>`.
 *         如果产生错误, 返回值包含错误结果和错误类型枚举
 *
 * @note 异步函数, 返回`Task`
 * */
Task<Result<nlohmann::json>> get_mv_list_all(std::string_view mid);

} // namespace qqmusic

#endif // !QQMUSIC_SINGER_H
