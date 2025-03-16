#include <qqmusic/details/api.h>
#include <qqmusic/toplist.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/session.h>

namespace qqmusic {

Task<Result<nlohmann::json>> get_toplist_category() {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musicToplist.Toplist", "GetAll");
    auto req_params = co_await api.prepare_request(nlohmann::json::object());
    if (req_params.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_params.unwrapErr().get_error_enum()),
            std::format("[get_toplist_category] -- Error occurred when preparing request: `{}`",
                        req_params.unwrapErr().what())));
    }

    auto url = req_params.unwrap().url;
    auto req = req_params.unwrap().req;
    auto resp_res = co_await session.perform_request(url, req);
    if (resp_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_toplist_category] -- Error occurred when performing request: `{}`",
                        resp_res.unwrapErr().what())));
    }

    auto resp_data = api.parse_response(utils::resp2buf(resp_res.unwrap()));
    if (resp_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_toplist_category] -- Error occurred when parsing reponse: `{}`",
                        resp_data.unwrapErr().what())));
    }
    try {
        auto data = resp_data.unwrap()["group"];
        co_return Ok(data);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                                       "[get_toplist_category] -- Cannot get `group` from data"));
    }
}

Task<Result<nlohmann::json>> get_toplist_item_detail(unsigned top_id,
                                                     unsigned num,
                                                     unsigned page,
                                                     bool tag) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musicToplist.Toplist", "GetDetail");
    nlohmann::json params = {{"topId", top_id},
                             {"offset", num * (page - 1)},
                             {"num", num},
                             {"withTags", tag}};

    auto req_data = co_await api.prepare_request(params);
    if (req_data.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_data.unwrapErr().get_error_enum()),
            std::format("[get_toplist_item_detail] -- Error occurred when preparing request: `{}`",
                        req_data.unwrapErr().what())));
    }

    auto url = req_data.unwrap().url;
    auto req = req_data.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_toplist_item_detail] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto resp = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (resp.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_toplist_item_detail] -- Error occurred when parsing reponse: `{}`",
                        resp.unwrapErr().what())));
    }
    auto data = resp.unwrap();
    co_return Ok(data);
}

} // namespace qqmusic
