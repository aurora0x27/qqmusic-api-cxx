#include <qqmusic/details/api.h>
#include <qqmusic/lyric.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/session.h>

namespace qqmusic {

Task<Result<Lyric>> get_lyric(std::string_view mid, bool qrc, bool trans, bool roma) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallSong.PlayLyricInfo", "GetPlayLyricInfo");
    nlohmann::json params = {{"crypt", 1},
                             {"ct", 11},
                             {"cv", 13020508},
                             {"lrc_t", 0},
                             {"qrc", qrc ? 1 : 0},
                             {"qrc_t", 0},
                             {"roma", roma ? 1 : 0},
                             {"roma_t", 0},
                             {"trans", trans ? 1 : 0},
                             {"trans_t", 0},
                             {"type", 1},
                             {"songMid", mid}};

    auto req_res = co_await api.prepare_request(params);
    if (req_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_res.unwrapErr().get_error_enum()),
            std::format("[get_lyric] -- Error occurred when preparing request: `{}`",
                        req_res.unwrapErr().what())));
    }

    auto url = req_res.unwrap().url;
    auto req = req_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_lyric] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto data_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (data_res.isErr()) {
        co_return Err(
            utils::Exception(utils::Exception::DataDestroy,
                             std::format("[get_lyric] -- Error occurred when parsing reponse: `{}`",
                                         data_res.unwrapErr().what())));
    }

    auto data = data_res.unwrap();
    if (!data.contains("lyric") || !data.contains("trans") || !data.contains("roma")) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            "[get_lyric] -- Assert data contains field `lyric`, `trans` and `roma` failed"));
    }

    Lyric res{};

    utils::buffer lyric_buf{std::move(utils::hex2buf(data["lyric"].get<std::string>()))};
    if (lyric_buf.size() == 0) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_lyric] -- Cannot find lyric, may be `{}` is not a song", mid)));
    }
    auto lyric_res = utils::qrc_decode(lyric_buf, utils::qrc_type::cloud);
    if (lyric_res.isErr()) {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       std::format("[get_lyric] -- Lyric decode error: `{}`",
                                                   lyric_res.unwrapErr().what())));
    }
    res.lyric = lyric_res.unwrap();

    utils::buffer trans_buf{std::move(utils::hex2buf(data["trans"].get<std::string>()))};
    if (trans && trans_buf.size()) {
        auto trans_res = utils::qrc_decode(trans_buf, utils::qrc_type::cloud);
        if (trans_res.isErr()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           std::format("[get_lyric] -- Lyric decode error: `{}`",
                                                       trans_res.unwrapErr().what())));
        }
        res.trans = trans_res.unwrap();
    }

    utils::buffer roma_buf{std::move(utils::hex2buf(data["roma"].get<std::string>()))};
    if (roma && roma_buf.size()) {
        auto roma_res = utils::qrc_decode(roma_buf, utils::qrc_type::cloud);
        if (roma_res.isErr()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           std::format("[get_lyric] -- Lyric decode error: `{}`",
                                                       roma_res.unwrapErr().what())));
        }
        res.roma = roma_res.unwrap();
    }

    co_return Ok(res);
}

Task<Result<Lyric>> get_lyric(uint64_t id, bool qrc, bool trans, bool roma) {
    auto session = utils::SessionManager::get_instance().get_session();
    auto api = details::Api(session, "music.musichallSong.PlayLyricInfo", "GetPlayLyricInfo");
    nlohmann::json params = {{"crypt", 1},
                             {"ct", 11},
                             {"cv", 13020508},
                             {"lrc_t", 0},
                             {"qrc", qrc ? 1 : 0},
                             {"qrc_t", 0},
                             {"roma", roma ? 1 : 0},
                             {"roma_t", 0},
                             {"trans", trans ? 1 : 0},
                             {"trans_t", 0},
                             {"type", 1},
                             {"songId", id}};

    auto req_res = co_await api.prepare_request(params);
    if (req_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_res.unwrapErr().get_error_enum()),
            std::format("[get_lyric] -- Error occurred when preparing request: `{}`",
                        req_res.unwrapErr().what())));
    }

    auto url = req_res.unwrap().url;
    auto req = req_res.unwrap().req;
    req.prepare_payload();
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_lyric] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    auto data_res = api.parse_response(utils::resp2buf(response_res.unwrap()));
    if (data_res.isErr()) {
        co_return Err(
            utils::Exception(utils::Exception::DataDestroy,
                             std::format("[get_lyric] -- Error occurred when parsing reponse: `{}`",
                                         data_res.unwrapErr().what())));
    }

    auto data = data_res.unwrap();
    if (!data.contains("lyric") || !data.contains("trans") || !data.contains("roma")) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            "[get_lyric] -- Assert data contains field `lyric`, `trans` and `roma` failed"));
    }

    Lyric res{};

    utils::buffer lyric_buf{std::move(utils::hex2buf(data["lyric"].get<std::string>()))};
    if (lyric_buf.size() == 0) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_lyric] -- Cannot find lyric, may be `{}` is not a song", id)));
    }
    auto lyric_res = utils::qrc_decode(lyric_buf, utils::qrc_type::cloud);
    if (lyric_res.isErr()) {
        co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                       std::format("[get_lyric] -- Lyric decode error: `{}`",
                                                   lyric_res.unwrapErr().what())));
    }
    res.lyric = lyric_res.unwrap();

    utils::buffer trans_buf{std::move(utils::hex2buf(data["trans"].get<std::string>()))};
    if (trans && trans_buf.size()) {
        auto trans_res = utils::qrc_decode(trans_buf, utils::qrc_type::cloud);
        if (trans_res.isErr()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           std::format("[get_lyric] -- Lyric decode error: `{}`",
                                                       trans_res.unwrapErr().what())));
        }
        res.trans = trans_res.unwrap();
    }

    utils::buffer roma_buf{std::move(utils::hex2buf(data["roma"].get<std::string>()))};
    if (roma && roma_buf.size()) {
        auto roma_res = utils::qrc_decode(roma_buf, utils::qrc_type::cloud);
        if (roma_res.isErr()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           std::format("[get_lyric] -- Lyric decode error: `{}`",
                                                       roma_res.unwrapErr().what())));
        }
        res.roma = roma_res.unwrap();
    }

    co_return Ok(res);
    co_return Err(utils::Exception(utils::Exception::UnknownError, "Not implemented yet"));
}

} // namespace qqmusic
