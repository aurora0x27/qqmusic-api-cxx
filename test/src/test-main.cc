#include <cstdlib>
#include <fstream>
#include <nlohmann/json.hpp>
#include <qqmusic/details/api.h>
#include <qqmusic/details/device.h>
#include <qqmusic/details/qimei.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/async-executor.h>
#include <qqmusic/utils/credential.h>

int main(int argc, char** argv) {
    std::fstream fs;
    fs.open(
        "/home/aurora/Desktop/projects/qqmusic-cli-project/reference/QQMusicApi/credential.json",
        std::ios::in);
    /*file exist, load cache*/
    std::ostringstream oss;
    std::string line;
    while (std::getline(fs, line)) {
        oss << line << '\n';
    }
    std::string raw = oss.str();
    auto credential = qqmusic::utils::Credential(std::string_view(raw));

    std::cout << credential.to_string().unwrap() << std::endl;

    fs.close();

    fs.open(
        "/home/aurora/Desktop/projects/qqmusic-cli-project/reference/QQMusicApi/credential.json",
        std::ios::out);

    auto is_expired_res = qqmusic::utils::sync_exec(credential.is_expired());
    auto refresh_success = qqmusic::utils::sync_exec(credential.refresh());
    if (refresh_success.isErr()) {
        std::cout << refresh_success.unwrapErr().what() << std::endl;
    }

    std::cout << credential.to_string().unwrap() << std::endl;
    auto res = credential.to_string();
    if (res.isErr()) {
        std::cout << "cannot convert credential to string: " << res.unwrapErr().what() << std::endl;
    } else {
        fs.write(res.unwrap().c_str(), (long)res.unwrap().size());
    }

    fs.close();

    return 0;
}
