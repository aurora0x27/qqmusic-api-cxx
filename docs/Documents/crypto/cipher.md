---
title: Cipher
---

解密密钥

::: doxy.dictionary.namespace.function
    namespace: qqmusic::crypto
    name: decrypt

!!! tip "decrypt"

    有 RC4Cipher 和 MapCipher 两种实现

::: doxy.dictionary.namespace.function
    namespace: qqmusic::crypto::TeaCipher
    name: decrypt

密钥先进行 tea 解密，再根据长度进行算法路由选择 rc4 或者 map 解密

具体过程见 qmc.cc 中的 decrypt
