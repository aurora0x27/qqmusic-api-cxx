#include <boost/asio/this_coro.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <qqmusic/singer.h>
#include <qqmusic/utils/async-executor.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/session.h>
#include <utility>

namespace qqmusic {

Task<Result<nlohmann::json>> get_singer_list(AreaType area, GenderType gender, GenreType genre) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallSinger.SingerList", "GetSingerList");
    nlohmann::json params = {{"hastag", 0},
                             {"area", (int) area},
                             {"sex", (int) gender},
                             {"genre", (int) genre}};
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_singer_list] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_singer_list] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_singer_list] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(response_json["hotlist"]);
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::DataDestroy,
                             std::format(
                                 "[get_singer_list] -- Cannot find field `hotlist` in response")));
    }
}

Task<Result<nlohmann::json>> get_singer_list_index(AreaType area,
                                                   GenderType gender,
                                                   GenreType genre,
                                                   IndexType index,
                                                   unsigned sin,
                                                   unsigned cur_page) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallSinger.SingerList", "GetSingerListIndex");
    nlohmann::json params = {{"hastag", 0},
                             {"area", (int) area},
                             {"sex", (int) gender},
                             {"genre", (int) genre},
                             {"index", index},
                             {"sin", sin},
                             {"cur_page", cur_page}};
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_singer_list_index] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_singer_list_index] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_singer_list_index] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> get_singer_list_index_all(AreaType area,
                                                       GenderType gender,
                                                       GenreType genre,
                                                       IndexType index) {
    // TODO: I didn't implement a `RequestGroup` as the python did.
    // so how can I optimize the code to make the requests execute parallel?
    auto data_res = co_await get_singer_list_index(area, gender, genre, index, 0, 1);
    if (data_res.isErr()) {
        co_return Err(data_res.unwrapErr());
    }

    auto data = data_res.unwrap();
    nlohmann::json singer_list;
    unsigned int total;
    try {
        total = data["total"];
        singer_list = std::move(data["singerlist"]);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            std::format("[get_singer_list_index_all] -- Cannot get singer list basic info: `{}`",
                        e.what())));
    }

    if (total <= 80) {
        co_return Ok(singer_list);
    }

    unsigned pages = total / 80;
    std::vector<Task<Result<nlohmann::json>>> tasks;
    for (unsigned i = 2, sin = 80; i < pages + 2; ++i, sin += 80) {
        tasks.push_back(get_singer_list_index(area, gender, genre, index, sin, i));
    }

    auto& async_executor = utils::AsyncExecutor::get_instance();
    auto all_results = co_await async_executor.when_all(std::move(tasks));
    for (auto& res : all_results) {
        if (res.isErr()) {
            co_return Err(res.unwrapErr());
        }
        auto group_new = res.unwrap();
        if (!group_new.contains("singerlist")) {
            co_return Err(utils::Exception(
                utils::Exception::DataDestroy,
                "[get_singer_list_index_all] -- Cannot find field `singerlist` in response"));
        }
        singer_list.insert(singer_list.end(), group_new["singerlist"]);
    }

    co_return Ok(singer_list);
}

Task<Result<nlohmann::json>> get_singer_info(std::string_view mid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.UnifiedHomepage.UnifiedHomepageSrv",
                            "GetHomepageHeader");
    nlohmann::json params = {{"SingerMid", mid}};
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_singer_info] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_singer_info] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_singer_info] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> get_tab_detail(std::string_view mid,
                                            TabType type,
                                            unsigned page,
                                            unsigned num) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.UnifiedHomepage.UnifiedHomepageSrv",
                            "GetHomepageTabDetail");
    nlohmann::json params = {{"SingerMid", mid},
                             {"IsQueryTabDetail", 1},
                             {"TabID", type.get_id()},
                             {"PageNum", page - 1},
                             {"PageSize", num},
                             {"Order", 0}};
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_singer_info] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_singer_info] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_singer_info] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }

    try {
        auto data = response_json_res.unwrap();
        auto result = data.value("List", data.value("VideoList", data.value("AlbumList", data)));
        co_return Ok(result);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            std::format("[get_singer_info] -- Cannot filter result from response: `{}`", e.what())));
    }
}

Task<Result<nlohmann::json>> get_singer_desc(std::span<std::string> mids, unsigned number) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallSinger.SingerInfoInter", "GetSingerDetail");
    nlohmann::json params = {{"singer_mids", mids}, {"groups", 1}, {"wikis", 1}};
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_singer_desc] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_singer_desc] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_singer_desc] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }

    auto data = response_json_res.unwrap();
    if (!data.contains("singerlist")) {
        co_return Err(
            utils::Exception(utils::Exception::DataDestroy,
                             "[get_singer_desc] -- Cannot find field `singerlist` in response"));
    }
    co_return Ok(data["singerlist"]);
}

Task<Result<nlohmann::json>> get_similar_singers(std::string_view mid, unsigned number) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.SimilarSingerSvr", "GetSimilarSingerList");
    nlohmann::json params = {{"singerMid", mid}, {"number", number}};
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_similar_singers] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_similar_singers] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_similar_singers] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(response_json["singerlist"]);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_similar_singers] -- Cannot find field `singerlist` in response")));
    }
}

Task<Result<nlohmann::json>> get_songs_list(std::string_view mid, unsigned num, unsigned begin) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "musichall.song_list_server", "GetSingerSongList");
    nlohmann::json params = {
        {"singerMid", mid},
        {"order", 1},
        {"number", num},
        {"begin", begin},
    };
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_songs_list] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_songs_list] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_songs_list] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> get_songs_list_all(std::string_view mid) {
    auto data_res = co_await get_songs_list(mid, 30, 0);
    if (data_res.isErr()) {
        co_return Err(data_res.unwrapErr());
    }

    auto data = data_res.unwrap();
    nlohmann::json song_list;
    unsigned int total;
    try {
        total = data["totalNum"];
        for (auto& song : data["songList"]) {
            song_list.push_back(song["songInfo"]);
        }
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            std::format("[get_singer_list_index_all] -- Cannot get singer list basic info: `{}`",
                        e.what())));
    }

    if (total <= 30) {
        co_return Ok(song_list);
    }

    std::vector<Task<Result<nlohmann::json>>> tasks;
    for (unsigned i = 30; i < total; i += 30) {
        tasks.push_back(get_songs_list(mid, 30, i));
    }

    auto& async_executor = utils::AsyncExecutor::get_instance();
    auto all_results = co_await async_executor.when_all(std::move(tasks));
    for (auto& res : all_results) {
        if (res.isErr()) {
            co_return Err(res.unwrapErr());
        }
        auto group_new = res.unwrap();
        if (!group_new.contains("songList")) {
            co_return Err(utils::Exception(
                utils::Exception::DataDestroy,
                "[get_singer_list_index_all] -- Cannot find field `songList` in response"));
        }

        for (auto& song : group_new["songList"]) {
            song_list.push_back(song["songInfo"]);
        }
    }

    co_return Ok(song_list);
}

Task<Result<nlohmann::json>> get_album_list(std::string_view mid, unsigned number, unsigned begin) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallAlbum.AlbumListServer", "GetAlbumList");
    nlohmann::json params = {
        {"singerMid", mid},
        {"order", 1},
        {"number", number},
        {"begin", begin},
    };
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_album_list] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_list] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_album_list] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> get_album_list_all(std::string_view mid) {
    auto data_res = co_await get_album_list(mid, 30, 0);
    if (data_res.isErr()) {
        co_return Err(data_res.unwrapErr());
    }

    auto data = data_res.unwrap();
    nlohmann::json album_list;
    unsigned int total;
    try {
        total = data["total"];
        album_list = data["albumlist"];
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            std::format("[get_album_list_all] -- Cannot get singer list basic info: `{}`",
                        e.what())));
    }

    if (total <= 30) {
        co_return Ok(album_list);
    }

    std::vector<Task<Result<nlohmann::json>>> tasks;
    for (unsigned i = 30; i < total; i += 30) {
        tasks.push_back(get_album_list(mid, 30, i));
    }

    auto& async_executor = utils::AsyncExecutor::get_instance();
    auto all_results = co_await async_executor.when_all(std::move(tasks));
    for (auto& res : all_results) {
        if (res.isErr()) {
            co_return Err(res.unwrapErr());
        }
        auto group_new = res.unwrap();
        if (!group_new.contains("albumList")) {
            co_return Err(utils::Exception(
                utils::Exception::DataDestroy,
                "[get_singer_list_index_all] -- Cannot find field `albumList` in response"));
        }

        album_list.push_back(group_new["albumList"]);
    }

    co_return Ok(album_list);
}

Task<Result<nlohmann::json>> get_mv_list(std::string_view mid, unsigned number, unsigned begin) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "MvService.MvInfoProServer", "GetSingerMvList");
    nlohmann::json params = {{"singermid", mid}, {"order", 1}, {"count", number}, {"start", begin}};
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_mv_list] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_mv_list] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_mv_list] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> get_mv_list_all(std::string_view mid) {
    auto data_res = co_await get_mv_list(mid, 100, 0);
    if (data_res.isErr()) {
        co_return Err(data_res.unwrapErr());
    }

    auto data = data_res.unwrap();
    nlohmann::json mv_list;
    unsigned int total;
    try {
        total = data["total"];
        mv_list = data["list"];
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            std::format("[get_mv_list_all] -- Cannot get singer list basic info: `{}`", e.what())));
    }

    if (total <= 100) {
        co_return Ok(mv_list);
    }

    std::vector<Task<Result<nlohmann::json>>> tasks;
    for (unsigned i = 100; i < total; i += 100) {
        tasks.push_back(get_mv_list(mid, 100, i));
    }

    auto& async_executor = utils::AsyncExecutor::get_instance();
    auto all_results = co_await async_executor.when_all(std::move(tasks));
    for (auto& res : all_results) {
        if (res.isErr()) {
            co_return Err(res.unwrapErr());
        }
        auto group_new = res.unwrap();
        if (!group_new.contains("list")) {
            co_return Err(utils::Exception(
                utils::Exception::DataDestroy,
                "[get_singer_list_index_all] -- Cannot find field `list` in response"));
        }

        mv_list.push_back(group_new["list"]);
    }

    co_return Ok(mv_list);
}

} // namespace qqmusic
