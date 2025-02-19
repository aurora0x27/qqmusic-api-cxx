#include "qqmusic/details/cookie.h"
#include <qqmusic/album.h>
#include <qqmusic/login.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/async-executor.h>

const char credential_path[]
    = "/home/aurora/Desktop/projects/qqmusic-cli-project/reference/QQMusicApi/credential.json";

const char cookie[]
    = "buvid3=01D3CD48-FE86-80DE-5706-CA860591B9D805168infoc; b_nut=1731210805; "
      "_uuid=A15F1697-CE9E-10573-53D4-22C482AAE59905861infoc; enable_web_push=DISABLE; "
      "buvid4=B670D1E4-190B-0A0C-6E3D-35074972527F06140-024111003-ba1d0oh5R0Q47E2dVDisZg%3D%3D;"
      " DedeUserID=1015677505; DedeUserID__ckMd5=1dc3581108d47792; header_theme_version=CLOSE; "
      "rpdid=|(u~RklkYm)u0J'u~Juku|mRk; LIVE_BUVID=AUTO5817312114888751; hit-dyn-v2=1; "
      "buvid_fp_plain=undefined; CURRENT_QUALITY=80; "
      "SESSDATA=74f723c6%2C1751381727%2C8e73f%2A11CjD-laRYPg4Le9kIu7YJa0-uuhnnD-"
      "pNqVDTZOBTXnZ88r1Qy9zKDaW_"
      "KQvCroMY5mYSVjBobDJMa1FZUXZ6SGpaQVFUNVI5Y2VFZG41Wm1CMG96RndwZkV2MndKUDU2WU9uZDBYSzhZZ2NG"
      "ZlVNSlV1SlhsbjNKa1ZjNWxVY091d05IemZaVVNnIIEC; "
      "bili_jct=46ed0a1229deb2743107c21332647bd8; sid=4iwz5o99; PVID=2; "
      "enable_feed_channel=DISABLE; theme_style=dark; "
      "bili_ticket=eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9."
      "eyJleHAiOjE3NDAwMjA5MjcsImlhdCI6MTczOTc2MTY2NywicGx0IjotMX0.cm8QUIkgn2VobL8QTjLNDSQEQRY-"
      "Ed5iNeOT6kqHuOQ; bili_ticket_expires=1740020867; home_feed_column=5; bmg_af_switch=1; "
      "bmg_src_def_domain=i0.hdslb.com; browser_resolution=2048-1031; "
      "fingerprint=b1e9946bec168e981bcdf00fc4ba1f25; "
      "buvid_fp=a742a828dcded593137108e9559435b5; b_lsid=58E24E76_1951E33186D; "
      "CURRENT_FNVAL=4048";

int main(int argc, char** argv) {
    const char album_mid[] = "001JUznp0TEqnM";
    const uint64_t album_id = 27143050;

    qqmusic::details::CookieJar cookies(cookie, "bilibili");

    auto dump_res = cookies.serialize("bilibili");

    if (dump_res.isErr()) {
        std::cout << dump_res.unwrapErr().what() << std::endl;
        return 1;
    }

    std::cout << dump_res.unwrap() << std::endl;

    // auto res = qqmusic::utils::sync_exec(qqmusic::get_qrcode(qqmusic::QRLoginType::QQ));

    // auto res = qqmusic::utils::sync_exec(qqmusic::get_album_songs(album_mid, 1, 1));
    // if (res.isErr()) {
    //     std::cout << "Error occurred -- " << res.unwrapErr().what() << std::endl;
    // } else {
    //     std::cout << res.unwrap() << std::endl;
    // }
    return 0;
}
