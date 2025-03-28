

# File song.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**song.h**](song_8h.md)

[Go to the documentation of this file](song_8h.md)


```C++

#ifndef QQMUSIC_SONG_H
#define QQMUSIC_SONG_H

#include <map>
#include <qqmusic/result.h>
#include <qqmusic/utils/credential.h>
#include <stdexcept>

namespace qqmusic {

class BaseMediaFileType {
public:
    virtual std::string_view prefix() = 0;

    virtual std::string_view expandation() = 0;

    virtual bool encrypted() = 0;

    virtual ~BaseMediaFileType() = default;
};

class SongFileType : public BaseMediaFileType {
public:
    enum Type {
        MASTER,   
        ATMOS_2,  
        ATMOS_51, 
        FLAC,     
        OGG_640,  
        OGG_320,  
        OGG_192,  
        OGG_96,   
        MP3_320,  
        MP3_128,  
        ACC_192,  
        ACC_96,   
        ACC_48,   
    };

    SongFileType(Type type) {
        if (!table.contains(type)) {
            throw std::runtime_error("Unexpected SongFileType enum");
        }
        inner = table.at(type);
    };

    SongFileType()
        : qqmusic::SongFileType(MP3_128) {};

    ~SongFileType() override = default;

    std::string_view prefix() override { return inner.start_id; };

    std::string_view expandation() override { return inner.extention; };

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

class EncryptedSongFileType : public BaseMediaFileType {
public:
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

    EncryptedSongFileType(Type type) {
        if (!table.contains(type)) {
            throw std::runtime_error("SongFileType -- Ilegal enum type");
        }
        inner = table.at(type);
    };

    EncryptedSongFileType()
        : EncryptedSongFileType(FLAC) {};

    ~EncryptedSongFileType() override = default;

    std::string_view prefix() override { return inner.start_id; };

    std::string_view expandation() override { return inner.extention; };

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

Task<Result<nlohmann::json>> query_song(std::span<std::string> mids);

Task<Result<nlohmann::json>> query_song(std::span<uint64_t> ids);

struct SongInfo {
    std::string mid;
    std::optional<std::string> url = std::nullopt;
    std::optional<std::string> ekey = std::nullopt;
};

Task<Result<std::vector<SongInfo>>> get_song_urls(
    std::span<std::string> mids,
    std::unique_ptr<BaseMediaFileType> file_type = std::make_unique<SongFileType>(),
    std::optional<utils::Credential> credential = std::nullopt);

Task<Result<std::string>> get_try_url(std::string_view mid, std::string_view vs);

Task<Result<nlohmann::json>> get_song_detail(std::string_view mid);

Task<Result<nlohmann::json>> get_song_detail(uint64_t id);

Task<Result<nlohmann::json>> get_similar_songs(uint64_t songid);

Task<Result<nlohmann::json>> get_labels(uint64_t songid);

Task<Result<nlohmann::json>> get_related_songlist(uint64_t songid);

Task<Result<nlohmann::json>> get_related_mv(uint64_t songid,
                                            std::optional<std::string> last_mvid = std::nullopt);

Task<Result<nlohmann::json>> get_other_version(std::string_view mid);

Task<Result<nlohmann::json>> get_other_version(uint64_t id);

Task<Result<nlohmann::json>> get_producer_info(std::string_view mid);

Task<Result<nlohmann::json>> get_producer_info(uint64_t id);

Task<Result<nlohmann::json>> get_sheet(std::string_view mid);

Task<Result<nlohmann::json>> get_fav_num(std::span<uint64_t> id_list, bool readable = true);

} // namespace qqmusic

#endif // !QQMUSIC_SONG_H
```


