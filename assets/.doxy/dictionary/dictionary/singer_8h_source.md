

# File singer.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**singer.h**](singer_8h.md)

[Go to the documentation of this file](singer_8h.md)


```C++

#ifndef QQMUSIC_SINGER_H
#define QQMUSIC_SINGER_H

#include <qqmusic/result.h>
#include <stdexcept>
#include <unordered_map>

namespace qqmusic {

enum class AreaType : int {
    ALL = -100,
    CHINA = 200,
    TAIWAN = 2,
    AMERICA = 5,
    JAPAN = 4,
    KOREA = 3
};

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

enum class GenderType : int {
    ALL = -100,
    MALE = 0,
    FEMALE = 1,
    GROUP = 2,
};

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

class TabType {
public:
    enum Type {
        WIKI,     
        ALBUM,    
        COMPOSER, 
        LYRICIST, 
        PRODUCER, 
        ARRANGER,
        MUSICIAN, 
        SONG,     
        VIDEO     
    };

    TabType() = delete;
    TabType(Type type) {
        auto it = tab_data.find(type);
        if (it == tab_data.end()) {
            throw std::runtime_error("Unexpected TabType enum");
        }
        this->inner = it->second;
    }

    std::string get_id() const { return inner.tab_id; }

    std::string get_name() const { return inner.tab_name; }

private:
    struct Tab {
        std::string tab_id;   
        std::string tab_name; 
    } inner;

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

Task<Result<nlohmann::json>> get_singer_list(AreaType area = AreaType::ALL,
                                             GenderType gender = GenderType::ALL,
                                             GenreType genre = GenreType::ALL);

Task<Result<nlohmann::json>> get_singer_list_index(AreaType area = AreaType::ALL,
                                                   GenderType gender = GenderType::ALL,
                                                   GenreType genre = GenreType::ALL,
                                                   IndexType index = IndexType::ALL,
                                                   unsigned sin = 0,
                                                   unsigned cur_page = 1);

Task<Result<nlohmann::json>> get_singer_list_index_all(AreaType area = AreaType::ALL,
                                                       GenderType gender = GenderType::ALL,
                                                       GenreType genre = GenreType::ALL,
                                                       IndexType index = IndexType::ALL);

Task<Result<nlohmann::json>> get_singer_info(std::string_view mid);

Task<Result<nlohmann::json>> get_tab_detail(std::string_view mid,
                                            TabType type,
                                            unsigned page = 1,
                                            unsigned num = 10);

Task<Result<nlohmann::json>> get_singer_desc(std::span<std::string> mids, unsigned number = 10);

Task<Result<nlohmann::json>> get_similar_singers(std::string_view mid, unsigned number = 10);

Task<Result<nlohmann::json>> get_songs_list(std::string_view mid,
                                            unsigned num = 10,
                                            unsigned begin = 0);

Task<Result<nlohmann::json>> get_songs_list_all(std::string_view mid);

Task<Result<nlohmann::json>> get_album_list(std::string_view mid,
                                            unsigned number = 10,
                                            unsigned begin = 0);

Task<Result<nlohmann::json>> get_album_list_all(std::string_view mid);

Task<Result<nlohmann::json>> get_mv_list(std::string_view mid,
                                         unsigned number = 10,
                                         unsigned begin = 0);

Task<Result<nlohmann::json>> get_mv_list_all(std::string_view mid);

} // namespace qqmusic

#endif // !QQMUSIC_SINGER_H
```


