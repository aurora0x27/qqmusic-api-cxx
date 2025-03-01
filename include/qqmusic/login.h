/*----------------------------qqmusic/login.h-----------------------------------
 * Login utils -- include tree methods 
 * 1. QQ QRCode Login
 * 2. Wechat QRCode Login 
 * 3. and phone number login
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_LOGIN_H
#define QQMUSIC_LOGIN_H

#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic {

/*Login type*/
enum class QRLoginType { QQ, WX };

enum class PhoneLoginEvent : int { SEND = 0, CAPTCHA = 20276, FREQUENCY = 100001, OTHER = -1 };

struct PhoneLoginResult {
    PhoneLoginEvent event;
    std::string msg;
};

/*QR Code info*/
struct QRCode {
    QRLoginType qr_type;         /*qr code type*/
    std::string identifier;      /*qr code identifier*/
    std::string mimie_type;      /*qr code image type*/
    qqmusic::utils::buffer data; /*qr code image binary data*/
};

class QRCodeLoginEvent {
public:
    enum class Status { DONE, SCAN, CONF, REFUSE, TIMEOUT, OTHER };

    QRCodeLoginEvent(Status stat) { status = stat; }

    QRCodeLoginEvent(unsigned int code) {
        switch (code) {
        case 0:
        case 405:
            status = Status::DONE;
            break;
        case 66:
        case 408:
            status = Status::SCAN;
            break;
        case 67:
        case 404:
            status = Status::CONF;
            break;
        case 65:
            status = Status::TIMEOUT;
            break;
        case 68:
        case 403:
            status = Status::REFUSE;
            break;
        default:
            status = Status::OTHER;
        }
    };

    inline bool operator==(const QRCodeLoginEvent& e) { return this->status == e.status; }

    inline bool operator!=(const QRCodeLoginEvent& e) { return this->status != e.status; }

    inline bool operator==(Status stat) { return status == stat; }

    inline bool operator!=(Status stat) { return status != stat; }

    inline std::string_view name() const { return name_map.at(status); }

private:
    Status status;
    inline static const std::map<Status, std::string> name_map = {
        {Status::DONE, "DONE"},
        {Status::SCAN, "SCAN"},
        {Status::CONF, "CONF"},
        {Status::TIMEOUT, "TIMEOUT"},
        {Status::REFUSE, "REFUSE"},
        {Status::OTHER, "OTHER"},
    };
};

struct QRCodeLoginResult {
    QRCodeLoginEvent status{QRCodeLoginEvent::Status::OTHER};
    std::optional<utils::Credential> credential;
};

qqmusic::Task<qqmusic::Result<QRCode>> get_qrcode(QRLoginType login_type);

/*check qq qrcode login status*/
qqmusic::Task<qqmusic::Result<QRCodeLoginResult>> check_qq_qr(QRCode& qrc);

/*check wechat qrcode login status*/
qqmusic::Task<qqmusic::Result<QRCodeLoginResult>> check_wx_qr(QRCode& qrc);

qqmusic::Task<qqmusic::Result<PhoneLoginResult>> send_authcode(
    std::string_view phone, std::string_view country_code = "86");

qqmusic::Task<qqmusic::Result<utils::Credential>> phone_authorize(
    std::string_view phone, std::string_view auth_code, std::string_view country_code = "86");

} // namespace qqmusic

#endif // !QQMUSIC_LOGIN_H
