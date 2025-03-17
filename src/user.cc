#include <boost/beast/http.hpp>
#include <boost/url/urls.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/user.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/session.h>
#include <string>

namespace qqmusic {

Task<Result<std::string>> get_euin(uint64_t musicid) {
    namespace http = boost::beast::http;
    auto session = utils::SessionManager::get_instance().get_session();

    boost::url url{"https://c6.y.qq.com/rsc/fcgi-bin/fcg_get_profile_homepage.fcg"};
    url.set_params({{"ct", "20"},
                    {"cv", "4747474"},
                    {"cid", "205360838"},
                    {"userid", std::to_string(musicid)}});
    http::request<http::string_body> req{http::verb::get, url, 11};
    req.set(http::field::host, url.host());
    req.set(http::field::accept, "*/*");
    /*use raw buffer instead of compressed buffer when debuging*/
    req.set(http::field::accept_encoding, "gzip, deflate");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::user_agent,
            "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54");
    req.set(http::field::referer, "y.qq.com");

    /*send request*/
    auto res = co_await session.perform_request(url, req);
    if (res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_euin] -- Error occurred when performing https request: `{}`",
                        res.unwrapErr().what())));
    }

    try {
        auto data = nlohmann::json::parse(utils::resp2buf(res.unwrap()))["data"];
        std::string euin = data["creator"]["encrypt_uin"];
        co_return Ok(euin);
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::JsonError,
                             std::format("[get_euin] -- Cannot filter euin from json: `{}`",
                                         e.what())));
    }
}

Task<Result<uint64_t>> get_musicid(std::string_view euin) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.srfDissInfo.DissInfo", "CgiGetDiss");

    nlohmann::json params = {{"disstid", 0},
                             {"dirid", 201},
                             {"song_num", 1},
                             {"enc_host_uin", euin},
                             {"onlysonglist", 1}};

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_musicid] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_musicid] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_musicid] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(response_json["dirinfo"]["creator"]["musicid"].get<uint64_t>());
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            std::format("[get_musicid] -- Cannot filter musicid from response: `{}`", e.what())));
    }
}

Task<Result<nlohmann::json>> get_homepage(std::string_view euin,
                                          std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.UnifiedHomepage.UnifiedHomepageSrv",
                            "GetHomepageHeader",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"uin", euin}, {"IsQueryTabDetail", 1}};

    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_homepage] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_homepage] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_homepage] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}

Task<Result<nlohmann::json>> get_vip_info(std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "VipLogin.VipLoginInter",
                            "vip_login_base",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    auto req_param_res = co_await api.prepare_request(nlohmann::json::object(), true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_vip_info] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_vip_info] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_vip_info] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    co_return Ok(response_json_res.unwrap());
}

Task<Result<nlohmann::json>> get_follow_singers(std::string_view euin,
                                                unsigned page,
                                                unsigned num,
                                                std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.concern.RelationList",
                            "GetFollowSingerList",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"HostUin", euin}, {"From", (page - 1) * num}, {"Size", num}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_follow_singers] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_follow_singers] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_follow_singers] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(
            nlohmann::json{{"total", response_json["Total"]}, {"list", response_json["List"]}});
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            std::format("[get_follow_singers] -- Cannot get result in response: `{}`", e.what())));
    }
}

Task<Result<nlohmann::json>> get_fans(std::string_view euin,
                                      unsigned page,
                                      unsigned num,
                                      std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.concern.RelationList",
                            "GetFansList",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"HostUin", euin}, {"From", (page - 1) * num}, {"Size", num}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_fans] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_fans] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(
            utils::Exception(utils::Exception::DataDestroy,
                             std::format("[get_fans] -- Error occurred when parsing reponse: `{}`",
                                         response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(
            nlohmann::json{{"total", response_json["Total"]}, {"list", response_json["List"]}});
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::JsonError,
                             std::format("[get_fans] -- Cannot get result in response: `{}`",
                                         e.what())));
    }
}

Task<Result<nlohmann::json>> get_friend_list(unsigned page,
                                             unsigned num,
                                             std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.homepage.Friendship",
                            "GetFriendList",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"PageSize", num}, {"Page", page - 1}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_friend_list] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_friend_list] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_friend_list] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    std::cout << response_json << std::endl;
    try {
        co_return Ok(response_json["Friends"]);
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::JsonError,
                             std::format("[get_friend_list] -- Cannot get result in response: `{}`",
                                         e.what())));
    }
}

Task<Result<nlohmann::json>> get_follow_usr(std::string_view euin,
                                            unsigned page,
                                            unsigned num,
                                            std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.concern.RelationList",
                            "GetFollowUserList",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"HostUin", euin}, {"From", (page - 1) * num}, {"Size", num}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_follow_usr] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_follow_usr] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_follow_usr] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(
            nlohmann::json{{"total", response_json["Total"]}, {"list", response_json["List"]}});
    } catch (const std::exception& e) {
        co_return Err(
            utils::Exception(utils::Exception::JsonError,
                             std::format("[get_follow_usr] -- Cannot get result in response: `{}`",
                                         e.what())));
    }
    co_return Err(utils::Exception(utils::Exception::UnknownError, "Not implemented"));
}

Task<Result<nlohmann::json>> get_created_songlist(uint64_t musicid,
                                                  std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musicasset.PlaylistBaseRead", "GetPlaylistByUin");

    nlohmann::json params = {{"uin", std::to_string(musicid)}};

    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_created_songlist] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_created_songlist] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_created_songlist] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    try {
        co_return Ok(response_json["v_playlist"]);
    } catch (const std::exception& e) {
        co_return Err(utils::Exception(
            utils::Exception::JsonError,
            std::format("[get_created_songlist] -- Cannot get result in response: `{}`", e.what())));
    }
}

Task<Result<nlohmann::json>> get_fav_song(std::string_view euin,
                                          unsigned page,
                                          unsigned num,
                                          std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.srfDissInfo.DissInfo",
                            "CgiGetDiss",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"disstid", 0},
                             {"dirid", 201},
                             {"tag", 1},
                             {"song_begin", num * (page - 1)},
                             {"song_num", num},
                             {"userinfo", 1},
                             {"orderlist", 1},
                             {"enc_host_uin", euin}};

    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_fav_song] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_fav_song] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_fav_song] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }

    co_return Ok(response_json_res.unwrap());
}

Task<Result<nlohmann::json>> get_fav_songlist(std::string_view euin,
                                              unsigned page,
                                              unsigned num,
                                              std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.musicasset.PlaylistFavRead",
                            "CgiGetPlaylistFavInfo",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"uin", euin}, {"offset", (page - 1) * num}, {"size", num}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_fav_songlist] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_fav_songlist] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_fav_songlist] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    co_return Ok(response_json_res.unwrap());
}

Task<Result<nlohmann::json>> get_fav_album(std::string_view euin,
                                           unsigned page,
                                           unsigned num,
                                           std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.musicasset.AlbumFavRead",
                            "CgiGetAlbumFavInfo",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"euin", euin}, {"offset", (page - 1) * num}, {"size", num}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_fav_album] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_fav_album] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_fav_album] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    co_return Ok(response_json_res.unwrap());
}

Task<Result<nlohmann::json>> get_fav_mv(std::string_view euin,
                                        unsigned page,
                                        unsigned num,
                                        std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.musicasset.MVFavRead",
                            "getMyFavMV_v2",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"encuin", euin}, {"num", page - 1}, {"pagesize", num}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_fav_mv] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_fav_mv] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_fav_mv] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    co_return Ok(response_json_res.unwrap());
}

Task<Result<nlohmann::json>> get_music_gene(std::string_view euin,
                                            std::optional<utils::Credential> credential) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session,
                            "music.recommend.UserProfileSettingSvr",
                            "GetProfileReport",
                            credential.has_value() ? credential.value()
                                                   : session.get_context_ref().credential);

    nlohmann::json params = {{"VisitAccount", euin}};
    auto req_param_res = co_await api.prepare_request(params, true);
    if (req_param_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_music_gene] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }

    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_music_gene] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto response_json_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_music_gene] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    co_return Ok(response_json_res.unwrap());
}

} // namespace qqmusic
