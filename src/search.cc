#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/http.hpp>
#include <boost/url/urls.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/search.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/session.h>

namespace qqmusic {

Task<Result<nlohmann::json>> hotkey() {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musicsearch.HotkeyService", "GetHotkeyForQQMusicMobile");

    nlohmann::json params = {{"search_id", utils::get_search_id()}};

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(
            utils::Exception(utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
                             std::format("[hotkey] -- Error occurred when preparing request: `{}`",
                                         req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(
            utils::Exception(utils::Exception::NetworkError,
                             std::format("[hotkey] -- Error occurred when performing request: `{}`",
                                         response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(
            utils::Exception(utils::Exception::DataDestroy,
                             std::format("[hotkey] -- Error occurred when parsing reponse: `{}`",
                                         response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> keyword_complete(std::string_view keyword) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.smartboxCgi.SmartBoxCgi", "GetSmartBoxResult");

    nlohmann::json params = {{"search_id", utils::get_search_id()},
                             {"query", keyword},
                             {"num_per_page", 0},
                             {"page_idx", 0}};

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[keyword_complete] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[keyword_complete] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[keyword_complete] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> quick_search(std::string_view keyword) {
    namespace http = boost::beast::http;
    auto session = utils::SessionManager::get_instance().get_session();
    boost::url url{"https://c.y.qq.com/splcloud/fcgi-bin/smartbox_new.fcg"};
    url.set_params({{"key", keyword}});
    http::request<http::string_body> req{http::verb::get, url, 11};
    req.set(http::field::host, url.host());
    req.set(http::field::accept, "*/*");
    /*use raw buffer instead of compressed buffer when debuging*/
    req.set(http::field::accept_encoding, "identity");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::user_agent,
            "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");

    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[quick_search] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    try {
        auto resp_json = nlohmann::json::parse(
            boost::beast::buffers_to_string(response_res.unwrap().body().data()));
        int64_t rc = resp_json["code"].get<int64_t>();
        switch (rc) {
        case 0:
            co_return Ok(resp_json["data"]);
        case 1000:
            co_return Err(
                qqmusic::utils::Exception(qqmusic::utils::Exception::CredendialExpiredError,
                                          "[quick_search] -- Credential is expired"));
        case 2000:
            co_return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::SignInvalidError,
                                                    "[quick_search] -- Sign is invalid"));
        default:
            co_return Err(qqmusic::utils::Exception(
                qqmusic::utils::Exception::ResponseCodeError,
                std::format("[quick_search] -- Unexpected response code: {}", rc)));
        }

    } catch (const std::exception& e) {
        co_return Err(utils::Exception(utils::Exception::JsonError,
                             std::format("[quick_search] -- Cannot parse response: `{}`",
                                         e.what())));
    }
}

Task<Result<nlohmann::json>> general_search(std::string_view keyword,
                                            unsigned num,
                                            unsigned page,
                                            bool highlight) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.adaptor.SearchAdaptor", "do_search_v2");

    nlohmann::json params = {
        {"searchid", utils::get_search_id()},
        {"search_type", 100},
        {"page_num", num},
        {"query", keyword},
        {"page_id", page},
        {"highlight", (unsigned) highlight},
        {"grp", 1},
    };

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[general_search] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[general_search] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[general_search] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> search_by_type(std::string_view keyword,
                                            SearchType type,
                                            unsigned num_per_page,
                                            unsigned page,
                                            bool highlight) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.search.SearchCgiService", "DoSearchForQQMusicMobile");

    nlohmann::json params = {
        {"searchid", utils::get_search_id()},
        {"search_type", (unsigned) type},
        {"page_num", page},
        {"query", keyword},
        {"num_per_page", num_per_page},
        {"highlight", (unsigned) highlight},
        {"grp", 1},
    };

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[search_by_type] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[search_by_type] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[search_by_type] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    std::string key;
    switch ((unsigned) type) {
    case (unsigned) SearchType::SONG:
    case (unsigned) SearchType::LYRIC:
    case (unsigned) SearchType::AUDIO:
        key = "item_song";
        break;
    case (unsigned) SearchType::SINGER:
        key = "singer";
        break;
    case (unsigned) SearchType::ALBUM:
        key = "item_album";
        break;
    case (unsigned) SearchType::SONG_LIST:
        key = "item_songlist";
        break;
    case (unsigned) SearchType::MV:
        key = "item_mv";
        break;
    case (unsigned) SearchType::USER:
        key = "item_user";
        break;
    case (unsigned) SearchType::AUDIO_ALBUM:
        key = "item_audio";
        break;
    default:
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       "[search_by_type] -- Error enum value of search type"));
    }

    try {
        auto res = response_json["body"][key];
        co_return Ok(res);
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::DataDestroy,
                             std::format("[search_by_type] -- Cannot get search result: `{}`",
                                         e.what())));
    }
}

} // namespace qqmusic
