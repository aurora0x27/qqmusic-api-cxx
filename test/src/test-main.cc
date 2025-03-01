#include <cstdlib>
#include <fstream>
#include <nlohmann/json.hpp>
#include <qqmusic/album.h>
#include <qqmusic/details/api.h>
#include <qqmusic/login.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/async-executor.h>
#include <qqmusic/utils/credential.h>
#include <qqmusic/utils/device.h>
#include <qqmusic/utils/qimei.h>

int main(int argc, char** argv) {

    auto login_res = qqmusic::utils::sync_exec(qqmusic::get_qrcode(qqmusic::QRLoginType::WX));
    if (login_res.isErr()) {
        std::cout << login_res.unwrapErr().what() << std::endl;
        return 1;
    }

    auto qr = login_res.unwrap();

    auto check_res = qqmusic::utils::sync_exec(qqmusic::check_wx_qr(qr));
    if (check_res.isErr()) {
        std::cout << check_res.unwrapErr().what() << std::endl;
        return 1;
    }

    return 0;
}
