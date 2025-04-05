

# File login.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**login.h**](login_8h.md)

[Go to the documentation of this file](login_8h.md)


```C++

#ifndef QQMUSIC_LOGIN_H
#define QQMUSIC_LOGIN_H

#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/credential.h>

namespace qqmusic {

enum class QRLoginType { QQ, WX };

enum class PhoneLoginEvent : int { 
    SEND = 0,           
    CAPTCHA = 20276,    
    FREQUENCY = 100001, 
    OTHER = -1          
};

struct PhoneLoginResult {
    PhoneLoginEvent event;
    std::string msg;
};

struct QRCode {
    QRLoginType qr_type;         
    std::string identifier;      
    std::string mimie_type;      
    qqmusic::utils::buffer data; 
};

class QRCodeLoginEvent {
public:

    enum class Status { 
        DONE,    
        SCAN,    
        CONF,    
        REFUSE,  
        TIMEOUT, 
        OTHER    
    };

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

qqmusic::Task<qqmusic::Result<QRCodeLoginResult>> check_qq_qr(QRCode& qrc);

qqmusic::Task<qqmusic::Result<QRCodeLoginResult>> check_wx_qr(QRCode& qrc);

qqmusic::Task<qqmusic::Result<PhoneLoginResult>> send_authcode(
    std::string_view phone, std::string_view country_code = "86");

qqmusic::Task<qqmusic::Result<utils::Credential>> phone_authorize(
    std::string_view phone, std::string_view auth_code, std::string_view country_code = "86");

} // namespace qqmusic

#endif // !QQMUSIC_LOGIN_H
```


