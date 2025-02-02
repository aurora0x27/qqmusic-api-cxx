/*-----------------------------details/device.h---------------------------------
 * Provide random device info generator
 *
 * Generate random IMEI number, OS version and device info
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETAILS_DEVICE_H
#define QQMUSIC_DETAILS_DEVICE_H

#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/result.h>

namespace qqmusic::details {

struct OSVersion {
    OSVersion();

    std::string incremental;
    std::string release;
    std::string codename;
    int sdk;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OSVersion, incremental, release, codename, sdk);
};

struct Device {
    Device();

    std::string display;
    std::string product;
    std::string device;
    std::string board;
    std::string model;
    std::string fingerprint;
    std::string boot_id;
    std::string proc_version;
    std::string imei;
    std::string brand;
    std::string bootloader;
    std::string base_band;
    struct OSVersion version;
    std::string sim_info;
    std::string os_type;
    std::string mac_address;
    std::vector<int> ip_address;
    std::string wifi_bssid;
    std::string wifi_ssid;
    std::vector<int> imsi_md5;
    std::string android_id;
    std::string apn;
    std::string vendor_name;
    std::string vendor_os_name;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Device,
                                   display,
                                   product,
                                   device,
                                   board,
                                   model,
                                   fingerprint,
                                   boot_id,
                                   proc_version,
                                   imei,
                                   brand,
                                   bootloader,
                                   base_band,
                                   version,
                                   sim_info,
                                   os_type,
                                   mac_address,
                                   ip_address,
                                   wifi_bssid,
                                   wifi_ssid,
                                   imsi_md5,
                                   android_id,
                                   apn,
                                   vendor_name,
                                   vendor_os_name);
};

qqmusic::result<Device> get_device_info();

qqmusic::result<void> cache_device_info(const Device& dev);

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_DEVICE_H
