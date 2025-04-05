

# File device.cc



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**utils**](dir_313caf1132e152dd9b58bea13a4052ca.md) **>** [**device.cc**](device_8cc.md)

[Go to the source code of this file](device_8cc_source.md)



* `#include <boost/uuid.hpp>`
* `#include <boost/uuid/detail/md5.hpp>`
* `#include <botan/auto_rng.h>`
* `#include <botan/hex.h>`
* `#include <exception>`
* `#include <filesystem>`
* `#include <format>`
* `#include <fstream>`
* `#include <nlohmann/detail/conversions/from_json.hpp>`
* `#include <nlohmann/json.hpp>`
* `#include <qqmusic/utils/buffer.h>`
* `#include <qqmusic/utils/device.h>`
* `#include <qqmusic/utils/paths.h>`
* `#include <string>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**qqmusic**](namespaceqqmusic.md) <br> |
| namespace | [**utils**](namespaceqqmusic_1_1utils.md) <br>_简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖_ `nlohmann::json` _实现 目前不支持路径嵌套和_`Expires` _和_`HttpOnly` _这样的特性_ |





















































------------------------------
The documentation for this class was generated from the following file `src/utils/device.cc`

