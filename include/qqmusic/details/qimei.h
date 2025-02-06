/*--------------------------qqmusic/details/qimei.h-----------------------------
 * Provide interface to manage q-imei.
 *
 * Q-IMEI is IMEI used in qqmusic and other tencent services, it is required in
 * some interfaces which need device identification or have device limits
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETAILS_QIMEI_H
#define QQMUSIC_DETAILS_QIMEI_H

#include <qqmusic/details/device.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <string>

namespace qqmusic::details {

struct QimeiResult {
    std::string q16;
    std::string q36;
};

/*always return ok*/
qqmusic::Result<QimeiResult> get_qimei(qqmusic::details::Device& device, std::string_view version);

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_QIMEI_H
