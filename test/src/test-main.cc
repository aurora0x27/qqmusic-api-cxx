#include <qqmusic/utils/async-executor.h>
#include <qqmusic/result.h>
#include <qqmusic/album.h>

const char credential_path[]
    = "/home/aurora/Desktop/projects/qqmusic-cli-project/reference/QQMusicApi/credential.json";

int main(int argc, char** argv) {
    const char album_mid[] = "001JUznp0TEqnM";
    const uint64_t album_id = 27143050;
    auto res = qqmusic::utils::sync_exec(qqmusic::get_album_songs(album_mid, 1, 1));
    if (res.isErr()) {
        std::cout << "Error occurred -- " << res.unwrapErr().what() << std::endl;
    } else {
        std::cout << res.unwrap() << std::endl;
    }
    return 0;
}
