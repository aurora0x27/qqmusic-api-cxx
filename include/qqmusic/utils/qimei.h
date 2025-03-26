/*-------------------------------------qqmusic/utils/qimei.h----------------------------------------
 * Provide interface to manage q-imei.
 *
 * Q-IMEI is IMEI used in qqmusic and other tencent services, it is required in
 * some interfaces which need device identification or have device limits
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_QIMEI_H
#define QQMUSIC_UTILS_QIMEI_H

#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/device.h>
#include <string>

namespace qqmusic::utils {

struct QimeiResult {
    std::string q16;
    std::string q36;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(QimeiResult, q16, q36);
};

qqmusic::Result<QimeiResult> get_qimei(qqmusic::utils::Device& device, std::string_view version);

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_QIMEI_H
