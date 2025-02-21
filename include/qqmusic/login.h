/*----------------------------qqmusic/login.h-----------------------------------
 * Login utils
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_LOGIN_H
#define QQMUSIC_LOGIN_H

#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic {

/*Login type*/
enum class QRLoginType { QQ, WX };

/*QR Code info*/
struct QRCode {
    QRLoginType qr_type;         /*qr code type*/
    std::string identifier;      /*qr code identifier*/
    std::string mimie_type;      /*qr code image type*/
    qqmusic::utils::buffer data; /*qr code image binary data*/
};

qqmusic::Task<qqmusic::Result<QRCode>> get_qrcode(QRLoginType login_type);

} // namespace qqmusic

#endif // !QQMUSIC_LOGIN_H
