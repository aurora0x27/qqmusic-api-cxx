

# File qmc.h



[**FileList**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**qmc.h**](qmc_8h.md)

[Go to the source code of this file](qmc_8h_source.md)

_QQ音乐加密文件解码器核心模块_ [More...](#detailed-description)

* `#include <cstdint>`
* `#include <memory>`
* `#include <qqmusic/crypto/cipher_map.h>`
* `#include <qqmusic/crypto/cipher_rc4.h>`
* `#include <qqmusic/crypto/cipher_tea.h>`
* `#include <qqmusic/crypto/key_derive.h>`
* `#include <qqmusic/details/result.h>`
* `#include <qqmusic/utils/buffer.h>`
* `#include <vector>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**qqmusic**](namespaceqqmusic.md) <br> |
| namespace | [**crypto**](namespaceqqmusic_1_1crypto.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**Decoder**](classqqmusic_1_1crypto_1_1Decoder.md) <br> |


















































## Detailed Description




**Author:**

kingpoem 




**Version:**

0.1 




**Date:**

2025-03-26 将API请求得到的ekey存取到解码器的ekey中，根据长度选择解密算法（&gt;300字节为RC4,其他为MapCipher） 将解密后的密钥存取到key中，完成解密过程 需要解密的数据存取到buf\_in中，解密完成的数据存取到buf\_out中 




**Copyright:**

Copyright (c) 2025 




**See also:** [https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/qmc.go](https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/qmc.go) 
```C++
SongInfo song = ...;
if (song.ekey.has_value()) {
     qqmusic::crypto::Decoder decoder(*song.ekey);
     if (read2buf(buf)) {
         if (decoder.decrypt()) {
             std::cout << "decrypt successful" << std::endl;
         }
     }
} else {
     // throw error
}
// then can use `buf_out`
```
 


**Note:**

该接口未测试，暂不可用 





    

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/crypto/qmc.h`

