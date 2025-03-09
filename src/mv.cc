#include "qqmusic/details/result.h"
#include <boost/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/mv.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/session.h>
#include <string>
#include <utility>

namespace qqmusic {

Task<Result<nlohmann::json>> get_mv_detail(std::span<std::string> vids) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "video.VideoDataServer", "get_video_info_batch");
    nlohmann::json params = {
        {"vidlist", vids},
        {"required",
         {
             "vid",
             "type",
             "sid",
             "cover_pic",
             "duration",
             "singers",
             "video_switch",
             "msg",
             "name",
             "desc",
             "playcnt",
             "pubdate",
             "isfav",
             "gmid",
             "uploader_headurl",
             "uploader_nick",
             "uploader_encuin",
             "uploader_uin",
             "uploader_hasfollow",
             "uploader_follower_num",
             "uploader_hasfollow",
             "related_songs",
         }},
    };

    auto req_res = co_await api.prepare_request(params);
    if (req_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_res.unwrapErr().get_error_enum()),
            std::format("[get_mv_detail] -- Error occurred when preparing request: `{}`",
                        req_res.unwrapErr().what())));
    }

    auto url = req_res.unwrap().url;
    auto req = req_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_mv_detail] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto data_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (data_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_mv_detail] -- Error occurred when parsing reponse: `{}`",
                        data_res.unwrapErr().what())));
    }

    co_return Ok(data_res.unwrap());
}

Task<Result<std::vector<MvUrl>>> get_mv_urls(std::span<std::string> vids) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.stream.MvUrlProxy", "GetMvUrls");
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    nlohmann::json params = {
        {"vids", vids},
        {"request_type", 10003},
        {"guid", boost::uuids::to_string(uuid)},
        {"videoformat", 1},
        {"format", 265},
        {"dolby", 1},
        {"use_new_domain", 1},
        {"use_ipv6", 1},
    };

    auto req_res = co_await api.prepare_request(params);
    if (req_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_res.unwrapErr().get_error_enum()),
            std::format("[get_mv_urls] -- Error occurred when preparing request: `{}`",
                        req_res.unwrapErr().what())));
    }

    auto url = req_res.unwrap().url;
    auto req = req_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_mv_urls] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto data_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (data_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_mv_urls] -- Error occurred when parsing reponse: `{}`",
                        data_res.unwrapErr().what())));
    }

    std::vector<MvUrl> res;
    auto get_play_urls = [](nlohmann::json& data) {
        std::map<std::string, std::string> res;
        for (auto& url_info : data) {
            if (url_info.contains("freeflow_url") && url_info["freeflow_url"].size()
                && url_info.contains("filetype")) {
                std::string type = std::to_string(url_info["filetype"].get<uint64_t>());
                res[type] = url_info["freeflow_url"][0].get<std::string>();
            }
        }
        return std::move(res);
    };
    auto data = data_res.unwrap();
    for (auto& i : data.items()) {
        if (!i.value().contains("mp4") || !i.value().contains("hls")) {
            co_return Err(utils::Exception(
                utils::Exception::DataDestroy,
                "[get_mv_urls] -- Assert data contains fields `mp4` and `hls` failed."));
        }

        MvUrl item;
        item.vid = i.key();
        item.mp4 = std::move(get_play_urls(i.value()["mp4"]));
        item.hls = std::move(get_play_urls(i.value()["hls"]));
        res.push_back(item);
    }
    co_return Ok(res);
}

} // namespace qqmusic
