

# File key\_derive.h



[**FileList**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**key\_derive.h**](key__derive_8h.md)

[Go to the source code of this file](key__derive_8h_source.md)

_QQ音乐密钥解密算法实现_ [More...](#detailed-description)

* `#include <botan/secmem.h>`
* `#include <cstdint>`
* `#include <qqmusic/result.h>`
* `#include <qqmusic/utils/buffer.h>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**qqmusic**](namespaceqqmusic.md) <br> |
| namespace | [**crypto**](namespaceqqmusic_1_1crypto.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**KeyDerive**](classqqmusic_1_1crypto_1_1KeyDerive.md) <br> |


















































## Detailed Description




**Author:**

kingpoem 




**Version:**

0.1 




**Date:**

2025-03-16 该文件实现QQ音乐加密格式的密钥派生体系，通过解析Base64编码的输入密钥，区分V1/V2两种加密版本， 使用TEA算法进行多层解密（V2需两次TEA+Base64嵌套解密），最终生成用于RC4/mapCipher等音频解密算法的有效密钥。 内部包含数学变换密钥生成、块解密时盐值处理（2字节随机盐+7字节零校验）、以及跨平台兼容的字节序处理等安全机制。


传入的是从ekey提取的Base64编码的密钥字符串 输出的密钥直接用于初始化音频解码器


原始输入ekey → rawKey (base64编码) ↓ 解码 中间数据 → rawKeyDec (可能含V2头) ↓ 去头处理（如果是V2） 核心密文 → raw (V2解密输入) ↓ 双重TEA解密 最终数据 → 再次base64解码 → 最终密钥 

**Copyright:**

Copyright (c) 2025 




**See also:** [https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/key\_derive.go](https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/key_derive.go) 
```C++
std::vector<uint8_t> encrypted_key = {...}; // 原始加密密钥
qqmusic::crypto::Decoder Decoder(ekey); // 用ekey初始化解码器，ekey存取为解码器的公有变量
auto key = qqmusic::crypto::KeyDrive::derive(Decoder.ekey);
```
 


**Note:**

该接口未测试，暂不可用 





    

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/crypto/key_derive.h`

