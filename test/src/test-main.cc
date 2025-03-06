#include <qqmusic/utils/common.h>
#include <iostream>
#include <qmtest/test-login.h>
#include <qqmusic/utils/async-executor.h>
#include <qqmusic/utils/cookie.h>

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

// int main() {
//     std::vector<std::string> tests
//         = {"pt2gguin=;Expires=Thu, 01 Jan 1970 00:00:00 GMT;Path=/;Domain=qq.com;",
//            "pt2gguin=o3991239532;Expires=Tue, 19 Jan 2038 03:14:07 "
//            "GMT;Path=/;Domain=ptlogin2.qq.com;Secure;",
//            "ETK=;Path=/;Domain=ptlogin2.qq.com;Secure;",
//            "superuin=o3991239532;Path=/;Domain=ptlogin2.qq.com;Secure;",
//            "supertoken=2348467763;Path=/;Domain=ptlogin2.qq.com;Secure;",
//            "superkey=V6wjA6H7SkF1kXrXTYKRfMjH6Ko*617t5VIJ2nCC9VA_;Path=/"
//            ";Domain=ptlogin2.qq.com;Secure;HttpOnly;",
//            "pt_recent_uins="
//            "b2c2ee4f91799263a8bf0859f2b1a7458165865ed9f943290afce509b209ceb3aef28c8d4f9614efe1c4c48"
//            "6f958321adb2ea1015928438e;Expires=Tue, 01 Apr 2025 14:58:57 "
//            "GMT;Path=/;Domain=ptlogin2.qq.com;Secure;HttpOnly;",
//            "RK=Q9XFdiAr//;Expires=Tue, 19 Jan 2038 03:14:07 GMT;Path=/;Domain=qq.com;",
//            "ptnick_3991239532=e6b6bce5aeaee7a78be69c88e381aee99aa8e6b3a2e98090e6b581;Path=/"
//            ";Domain=ptlogin2.qq.com;Secure;",
//            "ptcz=94f739affe791da6ba7df0684f850a078b926486a8a88c6d049017130f86b3cb;Expires=Tue, 19 "
//            "Jan 2038 03:14:07 GMT;Path=/;Domain=qq.com;",
//            "ptcz=;Expires=Thu, 01 Jan 1970 00:00:00 GMT;Path=/;Domain=ptlogin2.qq.com;Secure;",
//            "airkey=;Expires=Thu, 01 Jan 1970 00:00:00 GMT;Path=/;Domain=qq.com;",
//            "pt2gguin=;Expires=Thu, 01 Jan 1970 00:00:00 GMT;Path=/;Domain=qq.com;",
//            "pt2gguin=o3991239532;Expires=Tue, 19 Jan 2038 03:14:07 "
//            "GMT;Path=/;Domain=ptlogin2.qq.com;Secure;",
//            "p_uin=o3991239532;Path=/;Domain=graph.qq.com;",
//            "pt4_token=2L1S6dgqvoW9f7AJBYdw0y5nOdz8IMztMxlFWvxQV1k_;Path=/;Domain=graph.qq.com;",
//            "p_skey_forbid=;Expires=Thu, 01 Jan 1970 00:00:00 GMT;Path=/;Domain=graph.qq.com;",
//            "p_skey=Fv6cN01VzY9PUCVpxXu1zTMKhsDCB-bkdrY0cM9jjtQ_;Path=/;Domain=graph.qq.com;Secure;",
//            "pt_oauth_token=82ZEUN*J-*hEU3DmgsBKrG*HPYPDLmANyb2DRZUgFKIUPC8hi94hTAxFAMsBwXXE43qPMlt*"
//            "8io_;Path=/;Domain=graph.qq.com;Secure;HttpOnly;",
//            "p_uin=;Expires=Thu, 01 Jan 1970 00:00:00 GMT;Path=/;Domain=qq.com;",
//            "p_skey=;Expires=Thu, 01 Jan 1970 00:00:00 GMT;Path=/;Domain=qq.com;",
//            "pt4_token=;Expires=Thu, 01 Jan 1970 00:00:00 GMT;Path=/;Domain=qq.com;",
//            "pt_login_type=3;Path=/;Domain=graph.qq.com;Secure;HttpOnly;SameSite=None;"};
//     qqmusic::utils::CookieJar cookies;
//     for (auto& i : tests) {
//         std::cout << qqmusic::utils::hash33(i, 10) << std::endl;;
//     }
//     return 0;
// }
