#include <qmtest/test-login.h>
#include <qqmusic/search.h>
#include <qqmusic/song.h>
#include <qqmusic/song_lists.h>
#include <qqmusic/utils/async-executor.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/cookie.h>
#include <qqmusic/utils/credential.h>
#include <qqmusic/utils/paths.h>

int main(int argc, char** argv) {
    /* test example:
     * "name": "嘘つきは恋のはじまり"
     * "vid": "0144lJK0051IdY", "010t16G52AiN8Y", "011xz2ZN2qYxMs"
     * "id" : 213836590,
     * "mid": "001CJxVG1yppB0"
     *
     * 洛天依
     * singer mid: 003ktdcg3E4kaG
     *
     * album:
     * "id": 4031756,
     * "mid": "002n5zzy3VVGau",
     */
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " [qq|wx|ph]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "qq") {
        return qqmusic::utils::sync_exec(qqmusic::test::test_qr_login(qqmusic::QRLoginType::QQ));
    } else if (mode == "wx") {
        return qqmusic::utils::sync_exec(qqmusic::test::test_qr_login(qqmusic::QRLoginType::WX));
    } else if (mode == "ph") {
        return qqmusic::utils::sync_exec(qqmusic::test::test_phone_login());
    } else {
        std::cout << "Not an avalible mode" << std::endl;
        return 1;
    }
}

// int main() {
//     auto cache_path = qqmusic::utils::PathManager::get_instance().get_cache_path()
//                       / std::filesystem::path("credential.json");
// 
//     std::fstream fs(cache_path.c_str());
//     std::ostringstream oss;
//     if (fs.good()) {
//         /*file exist, load cache*/
//         std::string line;
//         while (std::getline(fs, line)) {
//             oss << line << '\n';
//         }
//         std::string s = oss.str();
//     } else {
//         std::cout << "load credential failed" << std::endl;
//         return 1;
//     }
//     auto credential = qqmusic::utils::Credential(oss.str());
//     std::cout << oss.str() << std::endl;
//     std::cout << credential.to_string().unwrap() << std::endl;
// 
//     // std::vector<uint64_t> songids = {
//     //     102176361, 104797698, 333975089, 230393569, 557104539, 201118980, 235176425, 293676135,
//     //     105503807, 105302677, 104701120, 201431302, 105392046, 468977148, 202791044, 368406201,
//     //     551086959, 238837519, 557104494, 556784880, 488446536, 235208691, 433114352, 553433944,
//     //     468977147, 545778586, 403884461, 405443755, 326274773
//     // };
//     // auto res = qqmusic::utils::sync_exec(qqmusic::songlist_add_songs(2, songids, credential));
//     // if (res.isErr()) {
//     //     std::cout << "Error: " << res.unwrapErr().what() << std::endl;
//     //     return 1;
//     // }
//     // std::cout << res.unwrap() << std::endl;
//     return 0;
// }
