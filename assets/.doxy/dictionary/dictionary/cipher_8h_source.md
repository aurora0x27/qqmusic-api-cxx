

# File cipher.h

[**File List**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**cipher.h**](cipher_8h.md)

[Go to the documentation of this file](cipher_8h.md)


```C++

#ifndef QQMUSIC_CRYPTO_CIPHER_H
#define QQMUSIC_CRYPTO_CIPHER_H

#include <qqmusic/utils/buffer.h>

namespace qqmusic::crypto {
class Cipher {
public:
    virtual ~Cipher() = default;
    virtual void decrypt(qqmusic::utils::buffer& buf, size_t offset) = 0;
};

} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_CIPHER_H
```


