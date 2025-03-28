

# Struct qqmusic::utils::Device



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**Device**](structqqmusic_1_1utils_1_1Device.md)



_设备信息_ 

* `#include <device.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::string | [**android\_id**](#variable-android_id)  <br> |
|  std::string | [**apn**](#variable-apn)  <br> |
|  std::string | [**base\_band**](#variable-base_band)  <br> |
|  std::string | [**board**](#variable-board)  <br> |
|  std::string | [**boot\_id**](#variable-boot_id)  <br> |
|  std::string | [**bootloader**](#variable-bootloader)  <br> |
|  std::string | [**brand**](#variable-brand)  <br> |
|  std::string | [**device**](#variable-device)  <br> |
|  std::string | [**display**](#variable-display)  <br> |
|  std::string | [**fingerprint**](#variable-fingerprint)  <br> |
|  std::string | [**imei**](#variable-imei)  <br> |
|  std::vector&lt; int &gt; | [**imsi\_md5**](#variable-imsi_md5)  <br> |
|  std::vector&lt; int &gt; | [**ip\_address**](#variable-ip_address)  <br> |
|  std::string | [**mac\_address**](#variable-mac_address)  <br> |
|  std::string | [**model**](#variable-model)  <br> |
|  std::string | [**os\_type**](#variable-os_type)  <br> |
|  std::string | [**proc\_version**](#variable-proc_version)  <br> |
|  std::string | [**product**](#variable-product)  <br> |
|  std::string | [**qimei**](#variable-qimei)  <br> |
|  std::string | [**sim\_info**](#variable-sim_info)  <br> |
|  std::string | [**vendor\_name**](#variable-vendor_name)  <br> |
|  std::string | [**vendor\_os\_name**](#variable-vendor_os_name)  <br> |
|  struct [**OSVersion**](structqqmusic_1_1utils_1_1OSVersion.md) | [**version**](#variable-version)  <br> |
|  std::string | [**wifi\_bssid**](#variable-wifi_bssid)  <br> |
|  std::string | [**wifi\_ssid**](#variable-wifi_ssid)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Device**](#function-device) () <br> |
|   | [**NLOHMANN\_DEFINE\_TYPE\_INTRUSIVE**](#function-nlohmann_define_type_intrusive) ([**Device**](structqqmusic_1_1utils_1_1Device.md), display, product, device, board, model, fingerprint, boot\_id, proc\_version, imei, brand, bootloader, base\_band, version, sim\_info, os\_type, mac\_address, wifi\_bssid, wifi\_ssid, imsi\_md5, android\_id, apn, vendor\_name, vendor\_os\_name, qimei) <br> |




























## Public Attributes Documentation




### variable android\_id 

```C++
std::string qqmusic::utils::Device::android_id;
```




<hr>



### variable apn 

```C++
std::string qqmusic::utils::Device::apn;
```




<hr>



### variable base\_band 

```C++
std::string qqmusic::utils::Device::base_band;
```




<hr>



### variable board 

```C++
std::string qqmusic::utils::Device::board;
```




<hr>



### variable boot\_id 

```C++
std::string qqmusic::utils::Device::boot_id;
```




<hr>



### variable bootloader 

```C++
std::string qqmusic::utils::Device::bootloader;
```




<hr>



### variable brand 

```C++
std::string qqmusic::utils::Device::brand;
```




<hr>



### variable device 

```C++
std::string qqmusic::utils::Device::device;
```




<hr>



### variable display 

```C++
std::string qqmusic::utils::Device::display;
```




<hr>



### variable fingerprint 

```C++
std::string qqmusic::utils::Device::fingerprint;
```




<hr>



### variable imei 

```C++
std::string qqmusic::utils::Device::imei;
```




<hr>



### variable imsi\_md5 

```C++
std::vector<int> qqmusic::utils::Device::imsi_md5;
```




<hr>



### variable ip\_address 

```C++
std::vector<int> qqmusic::utils::Device::ip_address;
```




<hr>



### variable mac\_address 

```C++
std::string qqmusic::utils::Device::mac_address;
```




<hr>



### variable model 

```C++
std::string qqmusic::utils::Device::model;
```




<hr>



### variable os\_type 

```C++
std::string qqmusic::utils::Device::os_type;
```




<hr>



### variable proc\_version 

```C++
std::string qqmusic::utils::Device::proc_version;
```




<hr>



### variable product 

```C++
std::string qqmusic::utils::Device::product;
```




<hr>



### variable qimei 

```C++
std::string qqmusic::utils::Device::qimei;
```




<hr>



### variable sim\_info 

```C++
std::string qqmusic::utils::Device::sim_info;
```




<hr>



### variable vendor\_name 

```C++
std::string qqmusic::utils::Device::vendor_name;
```




<hr>



### variable vendor\_os\_name 

```C++
std::string qqmusic::utils::Device::vendor_os_name;
```




<hr>



### variable version 

```C++
struct OSVersion qqmusic::utils::Device::version;
```




<hr>



### variable wifi\_bssid 

```C++
std::string qqmusic::utils::Device::wifi_bssid;
```




<hr>



### variable wifi\_ssid 

```C++
std::string qqmusic::utils::Device::wifi_ssid;
```




<hr>
## Public Functions Documentation




### function Device 

```C++
qqmusic::utils::Device::Device () 
```




<hr>



### function NLOHMANN\_DEFINE\_TYPE\_INTRUSIVE 

```C++
qqmusic::utils::Device::NLOHMANN_DEFINE_TYPE_INTRUSIVE (
    Device,
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
    qimei
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/device.h`

