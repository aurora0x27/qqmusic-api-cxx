#include "qqmusic/utils/async-executor.h"
#include <iostream>
#include <qmtest/test-login.h>

int main(int argc, char** argv) {
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
