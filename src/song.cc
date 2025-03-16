#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/uuid/uuid.hpp>
#include <botan/auto_rng.h>
#include <qqmusic/details/api.h>
#include <qqmusic/result.h>
#include <qqmusic/song.h>
#include <qqmusic/utils/async-executor.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/session.h>
#include <utility>
#include <vector>

namespace qqmusic {

Task<Result<nlohmann::json>> query_song(std::span<std::string> mids) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.trackInfo.UniformRuleCtrl", "CgiGetTrackInfo");

    auto placeholder = std::vector(mids.size(), 0);
    nlohmann::json params = {{"types", placeholder},
                             {"modify_stamp", placeholder},
                             {"ctx", 0},
                             {"client", 1},
                             {"mids", mids}};

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[query_song] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[query_song] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[query_song] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["tracks"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[query_song] -- Cannot get `tracks` from data"));
    }
}

Task<Result<nlohmann::json>> query_song(std::span<uint64_t> ids) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.trackInfo.UniformRuleCtrl", "CgiGetTrackInfo");

    auto placeholder = std::vector(ids.size(), 0);
    nlohmann::json params = {{"types", placeholder},
                             {"modify_stamp", placeholder},
                             {"ctx", 0},
                             {"client", 1},
                             {"mids", ids}};

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[query_song] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[query_song] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[query_song] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["tracks"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[query_song] -- Cannot get `tracks` from data"));
    }
    co_return Err(utils::Exception(utils::Exception::UnknownError, "Not implemented"));
}

Task<Result<std::vector<SongInfo>>> get_song_urls(std::span<std::string> mids,
                                                  std::unique_ptr<BaseMediaFileType> file_type,
                                                  std::optional<utils::Credential> credential) {
    if (file_type == nullptr) {
        file_type = std::make_unique<SongFileType>();
    }
    bool encrypted = file_type->encrypted();
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            encrypted ? "music.vkey.GetEVkey" : "music.vkey.GetVkey",
                            encrypted ? "CgiGetEVkey" : "UrlGetVkey",
                            credential.has_value() ? credential.value() : utils::Credential{});
    auto rng = Botan::AutoSeeded_RNG();

    auto gen_hex_strings = [&rng](unsigned len) {
        const char table[] = "abcdef1234567890";
        uint64_t randi = 0;
        std::vector<uint8_t> buffer(sizeof(randi));
        rng.randomize(buffer);
        memcpy(&randi, buffer.data(), sizeof(randi));
        std::vector<char> res(len);
        for (auto& i : res) {
            rng.randomize(buffer);
            memcpy(&randi, buffer.data(), sizeof(randi));
            i = table[randi % 16];
        }
        return std::string(res.data(), res.size());
    };

    std::vector<Task<Result<details::RequestParam>>> request_group;
    unsigned full_group_num = mids.size() / 100;
    /* slice mids into groups. the max request id amount is 100 */
    for (unsigned i = 0; i < full_group_num; ++i) {
        auto it = mids.begin() + i * 100;
        auto group = std::span<std::string>{it, it + 100};
        std::vector<std::string> name_list;
        for (auto& mid : group) {
            std::string file_name;
            file_name += file_type->prefix();
            file_name += mid;
            file_name += mid;
            file_name += file_type->expandation();
            name_list.push_back(file_name);
        }
        std::vector<int> placeholder{100, 0};
        nlohmann::json params = {
            {"filename", name_list},
            {"guid", gen_hex_strings(32)},
            {"songmid", group},
            {"songtype", placeholder},
        };
        request_group.push_back(api.prepare_request(params));
    }

    /* handle the left mids */
    auto group_rest = std::span<std::string>{mids.begin() + full_group_num * 100, mids.end()};
    std::vector<std::string> name_list;
    for (auto& mid : group_rest) {
        std::string file_name;
        file_name += file_type->prefix();
        file_name += mid;
        file_name += mid;
        file_name += file_type->expandation();
        name_list.push_back(file_name);
    }
    std::vector<int> placeholder{100, 0};
    nlohmann::json params = {
        {"filename", name_list},
        {"guid", gen_hex_strings(32)},
        {"songmid", group_rest},
        {"songtype", placeholder},
    };
    request_group.push_back(api.prepare_request(params));

    /* wrap the request and the post-process to task */
    auto task = [&session,
                 &api,
                 encrypted](boost::url url,
                            boost::beast::http::request<boost::beast::http::string_body> req)
        -> Task<Result<std::vector<SongInfo>>> {
        static const char domain[] = "https://isure.stream.qqmusic.qq.com/";
        auto res = co_await session.perform_request(url, req);
        if (res.isErr()) {
            co_return Err(res.unwrapErr());
        }

        auto resp = api.parse_response(utils::resp2buf(res.unwrap()));
        if (resp.isErr()) {
            co_return Err(utils::Exception(
                utils::Exception::DataDestroy,
                std::format("[get_song_urls] -- Error occurred when parsing reponse: `{}`",
                            resp.unwrapErr().what())));
        }

        std::vector<SongInfo> list{};

        try {
            auto data = resp.unwrap()["midurlinfo"];
            for (auto& info : data) {
                SongInfo item;
                item.mid = info["songmid"];
                auto url = info["wifiurl"].get<std::string>();
                if (url.size()) {
                    item.url = domain + url;
                }
                if (encrypted) {
                    item.ekey = info["ekey"];
                }
                list.push_back(item);
            }
            co_return Ok(list);
        } catch (const std::exception& e) {
            co_return Err(utils::Exception(utils::Exception::JsonError,
                                           "[get_song_urls] -- Cannot process the result"));
        }
    };

    auto& executor = utils::AsyncExecutor::get_instance();
    auto requests = co_await executor.when_all(std::move(request_group));
    std::vector<Task<Result<std::vector<SongInfo>>>> tasks;
    for (auto& req_params : requests) {
        if (req_params.isErr()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           "[get_song_urls] -- Prepare request error"));
        }
        tasks.push_back(task(req_params.unwrap().url, req_params.unwrap().req));
    }

    std::vector<SongInfo> song_list;
    auto all_results = co_await executor.when_all(std::move(tasks));
    for (auto& res : all_results) {
        if (res.isErr()) {
            co_return Err(res.unwrapErr());
        }
        auto next = res.unwrap();
        song_list.insert(song_list.end(), next.begin(), next.end());
    }

    co_return Ok(song_list);
}

Task<Result<std::string>> get_try_url(std::string_view mid, std::string_view vs) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.vkey.GetVkey", "UrlGetVkey");

    auto rng = Botan::AutoSeeded_RNG();

    auto gen_hex_strings = [&rng](unsigned len) {
        const char table[] = "abcdef1234567890";
        uint64_t randi = 0;
        std::vector<uint8_t> buffer(sizeof(randi));
        rng.randomize(buffer);
        memcpy(&randi, buffer.data(), sizeof(randi));
        std::vector<char> res(len);
        for (auto& i : res) {
            rng.randomize(buffer);
            memcpy(&randi, buffer.data(), sizeof(randi));
            i = table[randi % 16];
        }
        return std::string(res.data(), res.size());
    };

    std::vector<std::string> list{std::string{mid}};
    nlohmann::json params = {
        {"filename", std::format("RS02{}.mp3", vs)},
        {"guid", gen_hex_strings(32)},
        {"songmid", list},
        {"songtype", std::vector<int>{1}},
    };

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_try_url] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_try_url] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_try_url] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["midurlinfo"][0];
        if (data["wifiurl"].size()) {
            co_return Ok(std::format("https://isure.stream.qqmusic.qq.com/{}",
                                     data["wifiurl"].get<std::string>()));
        } else {
            co_return Ok(std::string{});
        }
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_try_url] -- Cannot get `tracks` from data"));
    }
}

Task<Result<nlohmann::json>> get_song_detail(std::string_view mid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.pf_song_detail_svr", "get_song_detail_yqq");
    nlohmann::json params = {{"song_mid", mid}};

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_song_detail] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_song_detail] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_song_detail] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        co_return Ok(resp_data.unwrap());
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_song_detail] -- Cannot get `tracks` from data"));
    }
}

Task<Result<nlohmann::json>> get_song_detail(uint64_t id) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.pf_song_detail_svr", "get_song_detail_yqq");
    nlohmann::json params = {{"song_id", id}};

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_song_detail] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_song_detail] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_song_detail] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        co_return Ok(resp_data.unwrap());
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_song_detail] -- Cannot get `tracks` from data"));
    }
}

Task<Result<nlohmann::json>> get_similar_songs(uint64_t songid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.recommend.TrackRelationServer", "GetSimilarSongs");

    nlohmann::json params = {{"songid", songid}};

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_similar_songs] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_similar_songs] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_similar_songs] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["vecSong"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_similar_songs] -- Cannot get `vecSong` from data"));
    }
}

Task<Result<nlohmann::json>> get_labels(uint64_t songid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.recommend.TrackRelationServer", "GetSongLabels");

    nlohmann::json params = {{"songid", songid}};

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_labels] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_labels] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_labels] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["labels"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_labels] -- Cannot get `labels` from data"));
    }
}

Task<Result<nlohmann::json>> get_related_songlist(uint64_t songid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.recommend.TrackRelationServer", "GetRelatedPlaylist");

    nlohmann::json params = {{"songid", songid}};

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_related_songlist] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_related_songlist] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_related_songlist] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["vecPlaylist"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::JsonError,
                             "[get_related_songlist] -- Cannot get `vecPlaylist` from data"));
    }
}

Task<Result<nlohmann::json>> get_related_mv(uint64_t songid, std::optional<std::string> last_mvid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "MvService.MvInfoProServer", "GetSongRelatedMv");

    nlohmann::json params = {{"songid", songid}, {"songtype", 1}};
    if (last_mvid.has_value()) {
        params.push_back({"lastmvid", last_mvid.value()});
    }

    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_related_mv] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_related_mv] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_related_mv] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["list"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_related_mv] -- Cannot get `list` from data"));
    }
}

Task<Result<nlohmann::json>> get_other_version(std::string_view mid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.musichallSong.OtherVersionServer",
                            "GetOtherVersionSongs");

    nlohmann::json params = {{"songmid", mid}};
    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_other_version] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_other_version] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_other_version] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["versionList"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::JsonError,
                             "[get_other_version] -- Cannot get `versionList` from data"));
    }
}

Task<Result<nlohmann::json>> get_other_version(uint64_t id) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.musichallSong.OtherVersionServer",
                            "GetOtherVersionSongs");

    nlohmann::json params = {{"songid", id}};
    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_other_version] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_other_version] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_other_version] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["versionList"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::JsonError,
                             "[get_other_version] -- Cannot get `versionList` from data"));
    }
}

Task<Result<nlohmann::json>> get_producer_info(std::string_view mid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.sociality.KolWorksTag", "SongProducer");

    nlohmann::json params = {{"songmid", mid}};
    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_producer_info] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_producer_info] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_producer_info] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["Lst"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_producer_info] -- Cannot get `Lst` from data"));
    }
}

Task<Result<nlohmann::json>> get_producer_info(uint64_t id) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.sociality.KolWorksTag", "SongProducer");

    nlohmann::json params = {{"songid", id}};
    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_producer_info] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_producer_info] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_producer_info] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["Lst"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_producer_info] -- Cannot get `Lst` from data"));
    }
}

Task<Result<nlohmann::json>> get_sheet(std::string_view mid) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.mir.SheetMusicSvr", "GetMoreSheetMusic");

    nlohmann::json params = {{"songmid", mid}, {"scoreType", -1}};
    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_sheet] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_sheet] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(
            utils::Exception(utils::Exception::DataDestroy,
                             std::format("[get_sheet] -- Error occurred when parsing reponse: `{}`",
                                         resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["result"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_sheet] -- Cannot get `result` from data"));
    }
}

// readable: 是否人类可读
//
// demo:
// {'m_numbers': {'438910555': 1000001}, 'm_show': {'438910555': '550w+'}}
Task<Result<nlohmann::json>> get_fav_num(std::span<uint64_t> id_list, bool readable) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musicasset.SongFavRead", "GetSongFansNumberById");

    nlohmann::json params = {{"v_songId", id_list}};
    auto req_params = co_await api.prepare_request(params);
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_fav_num] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_fav_num] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_fav_num] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()[readable ? "m_show" : "m_numbers"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_fav_num] -- Cannot get statistics from data"));
    }
}

} // namespace qqmusic
