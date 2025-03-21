---
title: Device
---

设备信息

## ***struct*** OSVersion

操作系统信息

```cpp
struct OSVersion {
    OSVersion();

    std::string incremental;
    std::string release;
    std::string codename;
    int sdk;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OSVersion, incremental, release, codename, sdk);
};
```

## ***struct*** Device

设备信息

```cpp
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
```

## ***fn*** get_device_info

获取设备信息, 优先从缓存中读取, 如果没有缓存就生成随机设备信息并缓存

```cpp
qqmusic::Result<Device> get_device_info();
```

## ***fn*** cache_device

缓存设备信息

```cpp
qqmusic::Result<void> cache_device(const Device& device);
```
