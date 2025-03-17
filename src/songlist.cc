#include <format>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <qqmusic/songlist.h>
#include <qqmusic/utils/async-executor.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/session.h>

namespace qqmusic {

Task<Result<nlohmann::json>> get_songlist_detail(uint64_t songlist_id,
                                                 unsigned dirid,
                                                 unsigned num,
                                                 unsigned page,
                                                 bool only_song,
                                                 bool tag,
                                                 bool user_info) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.srfDissInfo.DissInfo", "CgiGetDiss");

    nlohmann::json params = {{"disstid", songlist_id},
                             {"dirid", dirid},
                             {"tag", tag},
                             {"song_begin", num * (page - 1)},
                             {"song_num", num},
                             {"userinfo", (unsigned) user_info},
                             {"orderlist", 1},
                             {"onlysonglist", (unsigned) only_song}};

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_songlist_detail] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_songlist_detail] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_songlist_detail] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }

    auto data = resp_data.unwrap();
    try {
        co_return Ok(nlohmann::json{{"dirinfo", data["dirinfo"]},
                                    {"total_song_num", data["total_song_num"]},
                                    {"songlist_size", data["songlist_size"]},
                                    {"songlist", data["songlist"]},
                                    {"songtag", data["songtag"]},
                                    {"orderlist", data["orderlist"]}});
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            std::format("[get_songlist_detail] -- Cannot filter data from response: `{}`",
                        e.what())));
    }
    co_return Err(utils::Exception(utils::Exception::UnknownError, "Not implemented"));
}

Task<Result<nlohmann::json>> get_songlist(uint64_t songlist_id, unsigned dirid) {
    auto response = co_await get_songlist_detail(songlist_id, dirid, 100, 1, true);
    if (response.isErr()) {
        co_return Err(utils::Exception(response.unwrapErr()));
    }
    auto data = response.unwrap();
    uint64_t total = data["total_song_num"].get<uint64_t>();
    nlohmann::json songlist = data["songlist"];

    if (total <= 100) {
        co_return Ok(songlist);
    }

    std::vector<Task<Result<nlohmann::json>>> tasks;
    for (unsigned i = 100; i < total; i += 100) {
        tasks.push_back(get_songlist_detail(songlist_id, dirid, 100, i, true));
    }

    auto& async_executor = utils::AsyncExecutor::get_instance();
    auto all_results = co_await async_executor.when_all(std::move(tasks));
    for (auto& res : all_results) {
        if (res.isErr()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           std::format("[get_songlist] -- Error occurred when "
                                                       "unwrapping results of left items: `{}`",
                                                       res.unwrapErr().what())));
        }
        songlist.insert(songlist.end(), res.unwrap()["songlist"]);
    }
    co_return Ok(songlist);
}

Task<Result<nlohmann::json>> songlist_create(std::string_view dirname,
                                             std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    /* if provided with credential, use it. otherwise use the credential in current session */
    auto api = details::Api(session,
                            "music.musicasset.PlaylistBaseWrite",
                            "AddPlaylist",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);
    nlohmann::json params = {{"dirName", dirname}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[songlist_create] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[songlist_create] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[songlist_create] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(response_json["result"]);
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::DataDestroy,
                             "[songlist_create] -- Cannot find field `result` in response"));
    }
}

Task<Result<bool>> songlist_delete(uint64_t dirid, std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.musicasset.PlaylistBaseWrite",
                            "DelPlaylist",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"dirId", dirid}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[songlist_delete] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[songlist_delete] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[songlist_delete] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(response_json["result"]["dirid"].get<uint64_t>() == dirid);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       "[songlist_delete] -- Cannot get result in response"));
    }
}

Task<Result<bool>> songlist_add_songs(uint64_t dirid,
                                      std::span<uint64_t> song_ids,
                                      std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.musicasset.PlaylistDetailWrite",
                            "AddSonglist",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);
    nlohmann::json content;
    for (auto& song : song_ids) {
        content.push_back({{"songType", 0}, {"songId", song}});
    }
    nlohmann::json params = {{"dirId", dirid}, {"v_songInfo", content}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[songlist_add_songs] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    std::cout << std::endl << req << std::endl;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[songlist_add_songs] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    std::cout << response_res.unwrap() << std::endl;
    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[songlist_add_songs] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(!(response_json["result"]["updateTime"].get<uint64_t>() == 0));
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       "[songlist_add_songs] -- Cannot get result in response"));
    }
}

Task<Result<bool>> songlist_delete_songs(uint64_t dirid,
                                         std::span<uint64_t> song_ids,
                                         std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.musicasset.PlaylistDetailWrite",
                            "DelSonglist",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);
    nlohmann::json content;
    for (auto& song : song_ids) {
        content.push_back({{"songType", 0}, {"songId", song}});
    }
    nlohmann::json params = {{"dirId", dirid}, {"v_songInfo", content}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[songlist_delete_songs] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[songlist_delete_songs] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[songlist_delete_songs] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(!(response_json["result"]["updateTime"].get<uint64_t>() == 0));
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       "[songlist_delete_songs] -- Cannot get result in response"));
    }
}

} // namespace qqmusic
