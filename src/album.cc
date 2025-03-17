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

    auto req_data = co_await api.prepare_request(params);
    if (req_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_data.unwrapErr().get_error_enum()),
            std::format("[get_album_detail] -- Error occurred when preparing request: `{}`",
                        req_data.unwrapErr().what())));
    }

    auto url = req_data.unwrap().url;
    auto req = req_data.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_detail] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto resp = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (resp.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_album_detail] -- Error occurred when parsing reponse: `{}`",
                        resp.unwrapErr().what())));
    }
    auto data = resp.unwrap();
    co_return Ok(data);
}

Task<Result<nlohmann::json>> get_album_detail(uint64_t album_id) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallAlbum.AlbumInfoServer", "GetAlbumDetail");

    nlohmann::json params = {{"albumId", album_id}};

    auto req_data = co_await api.prepare_request(params);
    if (req_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_data.unwrapErr().get_error_enum()),
            std::format("[get_album_detail] -- Error occurred when preparing request: `{}`",
                        req_data.unwrapErr().what())));
    }

    auto url = req_data.unwrap().url;
    auto req = req_data.unwrap().req;
    auto response = co_await session.perform_request(url, req);
    if (response.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_detail] -- Error occurred when performing request: `{}`",
                        response.unwrapErr().what())));
    }

    auto resp = api.parse_response(utils::resp2buf(response.unwrap()));
    if (resp.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_album_detail] -- Error occurred when parsing reponse: `{}`",
                        resp.unwrapErr().what())));
    }
    auto data = resp.unwrap();
    co_return Ok(data);
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

    auto req_data = co_await api.prepare_request(params);
    if (req_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_data.unwrapErr().get_error_enum()),
            std::format("[get_album_songs] -- Error occurred when preparing request: `{}`",
                        req_data.unwrapErr().what())));
    }

    auto url = req_data.unwrap().url;
    auto req = req_data.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_songs] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto resp = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (resp.isErr()) {
        co_return Err(utils::Exception(response_res.unwrapErr()));
    }
    auto data = resp.unwrap();
    co_return Ok(data["songList"]);
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

    auto req_data = co_await api.prepare_request(params);
    if (req_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_data.unwrapErr().get_error_enum()),
            std::format("[get_album_songs] -- Error occurred when preparing request: `{}`",
                        req_data.unwrapErr().what())));
    }

    auto url = req_data.unwrap().url;
    auto req = req_data.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_songs] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto resp = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (resp.isErr()) {
        co_return Err(utils::Exception(response_res.unwrapErr()));
    }
    auto data = resp.unwrap();
    co_return Ok(data["songList"]);
}

} // namespace qqmusic
