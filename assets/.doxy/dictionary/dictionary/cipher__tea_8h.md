

# File cipher\_tea.h



[**FileList**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**cipher\_tea.h**](cipher__tea_8h.md)

[Go to the source code of this file](cipher__tea_8h_source.md)

_TEA算法解密实现_ [More...](#detailed-description)

* `#include <botan/secmem.h>`
* `#include <cstdint>`
* `#include <qqmusic/utils/buffer.h>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**qqmusic**](namespaceqqmusic.md) <br> |
| namespace | [**crypto**](namespaceqqmusic_1_1crypto.md) <br> |
| namespace | [**TeaCipher**](namespaceqqmusic_1_1crypto_1_1TeaCipher.md) <br> |




















































## Detailed Description




**Author:**

kingpoem 




**Version:**

0.1 




**Date:**

2025-03-22 实现QQ音乐加密格式使用的TEA变种算法，支持32轮解密，每次解密8字节 




**Copyright:**

Copyright (c) 2025 




**See also:** [https://pkg.go.dev/golang.org/x/crypto/tea](https://pkg.go.dev/golang.org/x/crypto/tea) 
```C++
for (size_t offset = 0; offset < buf.size(); offset += 8) {
    qqmusic::crypto::TeaCipher::decrypt(buf, offset, key);
}
```
 


**Note:**

该接口未测试，暂不可用 





    

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/crypto/cipher_tea.h`

