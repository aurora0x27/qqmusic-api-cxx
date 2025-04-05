

# File cipher\_tea.cc

[**File List**](files.md) **>** [**crypto**](dir_4261af1259721e3e39e0d2dd7354b511.md) **>** [**cipher\_tea.cc**](cipher__tea_8cc.md)

[Go to the documentation of this file](cipher__tea_8cc.md)


```C++
#include <qqmusic/crypto/cipher_tea.h>

namespace qqmusic::crypto {

void TeaCipher::decrypt(qqmusic::utils::buffer& buf,
                        size_t offset,
                        const std::vector<uint8_t>& key) {
    constexpr int rounds = 32;

    auto load_u32 = [](const uint8_t* p) {
        return (static_cast<uint32_t>(p[3]) << 24) | (static_cast<uint32_t>(p[2]) << 16)
               | (static_cast<uint32_t>(p[1]) << 8) | static_cast<uint32_t>(p[0]);
    };

    auto store_u32 = [](uint8_t* p, uint32_t v) {
        p[0] = v & 0xFF;
        p[1] = (v >> 8) & 0xFF;
        p[2] = (v >> 16) & 0xFF;
        p[3] = (v >> 24) & 0xFF;
    };

    uint8_t* block = buf.data() + offset;
    uint32_t v0 = load_u32(block);
    uint32_t v1 = load_u32(block + 4);
    uint32_t sum = 0x9E3779B9 * rounds;

    for (int i = 0; i < rounds; i++) {
        v1 -= ((v0 << 4) + key[8 + (sum >> 11 & 3)]) ^ (v0 + sum)
              ^ ((v0 >> 5) + key[12 + (sum >> 11 & 3)]);
        v0 -= ((v1 << 4) + key[0 + (sum & 3)]) ^ (v1 + sum) ^ ((v1 >> 5) + key[4 + (sum & 3)]);
        sum -= 0x9E3779B9;
    }

    store_u32(block, v0);
    store_u32(block + 4, v1);
}

} // namespace qqmusic::crypto
```


