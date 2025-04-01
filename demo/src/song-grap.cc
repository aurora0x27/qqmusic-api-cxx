#include <qqmusic/result.h>
#include <qqmusic/search.h>
#include <qqmusic/song.h>
#include <qqmusic/utils/async-executor.h>

// [search_by_type] -- Error occurred when preparing request: `[Api::prepare_request] --
// UnknownError occurred: `[json.exception.type_error.316] invalid UTF-8 byte at index 0: 0x80``
int main(int argc, char** argv) {
    boost::asio::io_context ioc;

    if (argc == 1) {
        std::cout << "Usage: " << argv[0] << " <song_name1> <song_name2> ..." << std::endl;
        return 1;
    }

    std::vector<qqmusic::Task<qqmusic::Result<nlohmann::json>>> tasks;
    for (int i = 1; i < argc; ++i) {
        tasks.push_back(qqmusic::search_by_type(std::string(argv[i])));
    }

    std::vector<qqmusic::Task<qqmusic::Result<std::vector<qqmusic::SongInfo>>>> get_songs;
    auto& executor = qqmusic::utils::AsyncExecutor::get_instance();
    auto search_results = qqmusic::utils::sync_exec(ioc, executor.when_all(std::move(tasks)));
    std::cout << "get songs mids" << std::endl;
    std::vector<std::vector<std::string>> mids_groups;
    for (auto& result : search_results) {
        std::vector<std::string> mids;
        if (result.isErr()) {
            std::cout << result.unwrapErr().what() << std::endl;
            continue;
        }
        auto data = result.unwrap();
        for (auto& item : data) {
            mids.push_back(item["mid"]);
        }
        mids_groups.push_back(mids);
        get_songs.push_back(qqmusic::get_song_urls(*(mids_groups.end() - 1)));
    }

    ioc.restart();

    std::cout << "begin to get songs" << std::endl;
    auto songs = qqmusic::utils::sync_exec(ioc, executor.when_all(std::move(get_songs)));
    std::cout << "get songs end" << std::endl;
    for (auto& song : songs) {
        if (song.isErr()) {
            std::cout << song.unwrapErr().what() << std::endl;
            continue;
        }
        auto list = song.unwrap();
        for (auto& item : list) {
            std::cout << "Mid:  " << item.mid << std::endl;
            std::cout << "URL:  " << (item.url.has_value() ? item.url.value() : "No avaliable url")
                      << std::endl;
            std::cout << "Ekey: "
                      << (item.ekey.has_value() ? item.ekey.value() : "No avaliable ekey")
                      << std::endl;
        }
    }

    return 0;
}
