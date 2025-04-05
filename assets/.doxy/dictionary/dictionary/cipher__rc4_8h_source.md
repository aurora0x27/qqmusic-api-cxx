

# File cipher\_rc4.h

[**File List**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**cipher\_rc4.h**](cipher__rc4_8h.md)

[Go to the documentation of this file](cipher__rc4_8h.md)


```C++

#ifndef QQMUSIC_CRYPTO_CIPHER_RC4_H
#define QQMUSIC_CRYPTO_CIPHER_RC4_H

#include <cstdint>
#include <qqmusic/crypto/cipher.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::crypto {

class RC4Cipher : public Cipher {
public:
    explicit RC4Cipher(const std::vector<uint8_t>& key);

    void decrypt(qqmusic::utils::buffer& buf, size_t offset) override;

private:
    std::vector<uint8_t> key;
};

} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_CIPHER_RC4_H
```


