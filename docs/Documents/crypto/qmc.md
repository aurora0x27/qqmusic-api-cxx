---
title: qmc
---

::: doxy.dictionary.namespace.function
    namespace: qqmusic::crypto
    name: Decoder

::: doxy.dictionary.namespace.function
    namespace: qqmusic::crypto
    name: decrypt

::: doxy.dictionary.namespace.function
    namespace: qqmusic::crypto
    name: read2buf

调用示例：
```cpp
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
