

# File device.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**utils**](dir_478616d8952f43e793f28d8ded6e3463.md) **>** [**device.h**](device_8h.md)

[Go to the documentation of this file](device_8h.md)


```C++

#ifndef QQMUSIC_UTILS_DEVICE_H
#define QQMUSIC_UTILS_DEVICE_H

#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/result.h>

namespace qqmusic::utils {

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
    std::string qimei;

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
                                   wifi_bssid,
                                   wifi_ssid,
                                   imsi_md5,
                                   android_id,
                                   apn,
                                   vendor_name,
                                   vendor_os_name,
                                   qimei);
};

qqmusic::Result<Device> get_device_info();

qqmusic::Result<void> cache_device(const Device& device);

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_DEVICE_H
```


