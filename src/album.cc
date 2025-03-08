#include <format>
#include <qqmusic/album.h>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/common.h>
#include <string>

namespace qqmusic {

std::string get_album_cover_url(std::string_view mid, cover_size size) {
    return std::format("https://y.gtimg.cn/music/photo_new/T002R{}x{}M000{}.jpg",
                       (int) size,
                       (int) size,
                       mid);
}

Task<Result<nlohmann::json>> get_album_detail(std::string_view album_mid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallAlbum.AlbumInfoServer", "GetAlbumDetail");
    nlohmann::json params = {{"albumMId", album_mid}};

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_album_detail] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_detail] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_album_detail] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> get_album_detail(uint64_t album_id) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallAlbum.AlbumInfoServer", "GetAlbumDetail");

    nlohmann::json params = {{"albumId", album_id}};

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_album_detail] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_detail] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_album_detail] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> get_album_songs(std::string_view album_mid,
                                             unsigned int num,
                                             unsigned int page) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallAlbum.AlbumSongList", "GetAlbumSongList");

    nlohmann::json params = {
        {"begin", num * (page - 1)},
        {"num", num},
        {"albumMid", album_mid},
    };

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_album_songs] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_songs] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(response_res.unwrapErr()));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json["songList"]);
}

Task<Result<nlohmann::json>> get_album_songs(uint64_t album_id,
                                             unsigned int num,
                                             unsigned int page) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallAlbum.AlbumSongList", "GetAlbumSongList");

    nlohmann::json params = {
        {"begin", num * (page - 1)},
        {"num", num},
        {"albumId", album_id},
    };

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_album_songs] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_songs] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(response_res.unwrapErr()));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json["songList"]);
}

} // namespace qqmusic
