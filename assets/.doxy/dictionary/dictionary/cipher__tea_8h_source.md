

# File cipher\_tea.h

[**File List**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**cipher\_tea.h**](cipher__tea_8h.md)

[Go to the documentation of this file](cipher__tea_8h.md)


```C++

#ifndef QQ_MUSIC_CIPHER_TEA_H
#define QQ_MUSIC_CIPHER_TEA_H

#include <botan/secmem.h>
#include <cstdint>
#include <qqmusic/utils/buffer.h>

namespace qqmusic::crypto::TeaCipher {

void decrypt(qqmusic::utils::buffer& buf, size_t offset, const std::vector<uint8_t>& key);

} // namespace qqmusic::crypto::TeaCipher

#endif // QQ_MUSIC_CIPHER_TEA_H
```


