#include "qqmusic/singer.h"
#include <qmtest/test-login.h>
#include <qqmusic/search.h>
#include <qqmusic/utils/async-executor.h>
#include <qqmusic/utils/common.h>
#include <qqmusic/utils/cookie.h>

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
