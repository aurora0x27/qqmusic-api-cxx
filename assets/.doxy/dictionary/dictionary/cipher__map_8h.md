

# File cipher\_map.h



[**FileList**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**cipher\_map.h**](cipher__map_8h.md)

[Go to the source code of this file](cipher__map_8h_source.md)

_cipher\_map_ [More...](#detailed-description)

* `#include <cstdint>`
* `#include <qqmusic/crypto/cipher.h>`
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
| class | [**MapCipher**](classqqmusic_1_1crypto_1_1MapCipher.md) <br> |


















































## Detailed Description




**Author:**

kingpoem 




**Version:**

0.1 




**Date:**

2025-03-15 解密算法中的一种，适用于QQ音乐的早期加密 




**Copyright:**

Copyright (c) 2025 




**See also:** [https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/cipher\_map.go](https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/cipher_map.go) 
```C++
qqmusic::crypto::MapCipher cipher(key);
cipher.decrypt(buf, 0);
```
 


**Note:**

该接口未测试，暂不可用 





    

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/crypto/cipher_map.h`

