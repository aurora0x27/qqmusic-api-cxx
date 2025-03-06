---
title: Qimei
---

Q-IMEI是用于QQ音乐和TX其他服务中用于标识设备的唯一识别码.

## ***struct*** QimeiResult

包含16位识别码和36位识别码

```cpp
struct QimeiResult {
    std::string q16;
    std::string q36;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(QimeiResult, q16, q36);
};
```

## ***fn*** get_qimei

从设备信息和版本信息获取Q-IMEI

```cpp
qqmusic::Result<QimeiResult> get_qimei(qqmusic::utils::Device& device,
                                       std::string_view version);
```
