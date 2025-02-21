#include <format>
#include <qqmusic/album.h>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <string>

namespace qqmusic {

std::string get_album_cover_url(std::string_view mid, cover_size size) {
    return std::format("https://y.gtimg.cn/music/photo_new/T002R{}x{}M000{}.jpg",
                       (int) size,
                       (int) size,
                       mid);
}

Task<Result<nlohmann::json>> get_album_detail(std::string_view album_mid) {
    // auto api = details::Api("music.musichallAlbum.AlbumInfoServer", "GetAlbumDetail");
    // nlohmann::json params = {{"albumMId", album_mid}};

    // auto response_res = co_await api.do_request(params);
    // if (response_res.isErr()) {
    //     co_return Err(utils::Exception(response_res.unwrapErr()));
    // }

    // auto response_raw = response_res.unwrap();
    // auto response_json_res = api.parse_response(response_raw);
    // if (response_json_res.isErr()) {
    //     co_return Err(utils::Exception(response_res.unwrapErr()));
    // }
    // auto response_json = response_json_res.unwrap();
    // co_return response_json;
}

Task<Result<nlohmann::json>> get_album_detail(uint64_t album_id) {
    // auto api = details::Api("music.musichallAlbum.AlbumInfoServer", "GetAlbumDetail");
    // nlohmann::json params = {{"albumId", album_id}};

    // auto response_res = co_await api.do_request(params);
    // if (response_res.isErr()) {
    //     co_return Err(utils::Exception(response_res.unwrapErr()));
    // }

    // auto response_raw = response_res.unwrap();
    // auto response_json_res = api.parse_response(response_raw);
    // if (response_json_res.isErr()) {
    //     co_return Err(utils::Exception(response_res.unwrapErr()));
    // }
    // auto response_json = response_json_res.unwrap();
    // co_return Ok(response_json);
    co_return Err(utils::Exception(utils::Exception::UnknownError, "Not implemented yet"));
}

Task<Result<nlohmann::json>> get_album_songs(std::string_view album_mid,
                                             unsigned int num,
                                             unsigned int page) {
    // nlohmann::json params = {
    //     {"begin", num * (page - 1)},
    //     {"num", num},
    //     {"albumMid", album_mid},
    // };
    // auto api = details::Api("music.musichallAlbum.AlbumSongList", "GetAlbumSongList");

    // auto response_res = co_await api.do_request(params);
    // if (response_res.isErr()) {
    //     co_return Err(utils::Exception(response_res.unwrapErr()));
    // }

    // auto response_raw = response_res.unwrap();
    // auto response_json_res = api.parse_response(response_raw);
    // if (response_json_res.isErr()) {
    //     co_return Err(utils::Exception(response_res.unwrapErr()));
    // }
    // auto response_json = response_json_res.unwrap();
    // co_return Ok(response_json["songList"]);
    co_return Err(utils::Exception(utils::Exception::UnknownError, "Not implemented yet"));
}

Task<Result<nlohmann::json>> get_album_songs(uint64_t album_id,
                                             unsigned int num,
                                             unsigned int page) {
    // nlohmann::json params = {
    //     {"begin", num * (page - 1)},
    //     {"num", num},
    //     {"albumId", album_id},
    // };
    // auto api = details::Api("music.musichallAlbum.AlbumSongList", "GetAlbumSongList");

    // auto response_res = co_await api.do_request(params);
    // if (response_res.isErr()) {
    //     co_return Err(utils::Exception(response_res.unwrapErr()));
    // }

    // auto response_raw = response_res.unwrap();
    // auto response_json_res = api.parse_response(response_raw);
    // if (response_json_res.isErr()) {
    //     co_return Err(utils::Exception(response_res.unwrapErr()));
    // }
    // auto response_json = response_json_res.unwrap();
    // co_return Ok(response_json["songList"]);
    co_return Err(utils::Exception(utils::Exception::UnknownError, "Not implemented yet"));
}

} // namespace qqmusic
