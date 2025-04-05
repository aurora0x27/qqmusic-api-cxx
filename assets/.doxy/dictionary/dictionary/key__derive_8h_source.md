

# File key\_derive.h

[**File List**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**key\_derive.h**](key__derive_8h.md)

[Go to the documentation of this file](key__derive_8h.md)


```C++

#ifndef QQMUSIC_CRYPTO_KEY_DERIVE_H
#define QQMUSIC_CRYPTO_KEY_DERIVE_H

#include <botan/secmem.h>
#include <cstdint>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>

namespace qqmusic::crypto {

class KeyDerive {
public:
    static qqmusic::utils::buffer derive(qqmusic::utils::buffer&& raw_key);

private:
    static qqmusic::utils::buffer derive_V1(qqmusic::utils::buffer&& raw_key_dec);

    static qqmusic::utils::buffer derive_V2(qqmusic::utils::buffer raw);

    static qqmusic::utils::buffer decrypt_tencent_tea(qqmusic::utils::buffer&& in_buf,
                                                      const std::vector<uint8_t>& key);

    inline static std::vector<uint8_t> makeSimpleKey(uint8_t salt, size_t len) {
        std::vector<uint8_t> key(len);
        std::ranges::generate(key, [=, i = 0]() mutable {
            const double tmp = std::tan(salt + i++ * 0.1);
            return static_cast<uint8_t>(std::abs(tmp) * 100.0);
        });
        return key;
    }

    inline static const std::vector<uint8_t> DeriveV2Key1 = {{0x33,
                                                              0x38,
                                                              0x36,
                                                              0x5A,
                                                              0x4A,
                                                              0x59,
                                                              0x21,
                                                              0x40,
                                                              0x23,
                                                              0x2A,
                                                              0x24,
                                                              0x25,
                                                              0x5E,
                                                              0x26,
                                                              0x29,
                                                              0x28}};

    inline static const std::vector<uint8_t> DeriveV2Key2 = {{0x2A,
                                                              0x2A,
                                                              0x23,
                                                              0x21,
                                                              0x28,
                                                              0x23,
                                                              0x24,
                                                              0x25,
                                                              0x26,
                                                              0x5E,
                                                              0x61,
                                                              0x31,
                                                              0x63,
                                                              0x5A,
                                                              0x2C,
                                                              0x54}};
};

} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_KEY_DERIVE_H
```


