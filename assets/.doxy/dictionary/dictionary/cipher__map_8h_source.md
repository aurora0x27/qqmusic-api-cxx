

# File cipher\_map.h

[**File List**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**cipher\_map.h**](cipher__map_8h.md)

[Go to the documentation of this file](cipher__map_8h.md)


```C++

#ifndef QQ_MUSIC_CIPHER_MAP_H
#define QQ_MUSIC_CIPHER_MAP_H

#include <cstdint>
#include <qqmusic/crypto/cipher.h>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::crypto {

class MapCipher : public Cipher {
public:
    explicit MapCipher(const std::vector<uint8_t>& key);

    void decrypt(qqmusic::utils::buffer& buf, size_t offset) override;

private:
    std::vector<uint8_t> key;
};

} // namespace qqmusic::crypto

#endif // !QQ_MUSIC_CIPHER_MAP_H
```


