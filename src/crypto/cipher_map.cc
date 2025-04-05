#include <iostream>
#include <qqmusic/crypto/cipher_map.h>

namespace qqmusic::crypto {

MapCipher::MapCipher(const std::vector<uint8_t>& key)
    : key(key) {
    if (key.size() == 0) {
        std::cout << "[MapCipher constructor failed] key size is 0" << std::endl;
    }
}

void MapCipher::decrypt(qqmusic::utils::buffer& buf, size_t offset) {
    const auto rotate = [](uint8_t value, uint8_t bits) {
        const uint8_t r = (bits + 4) % 8;
        return (value << r) | (value >> (8 - r));
    };

    for (size_t i = 0; i < buf.size(); ++i) {
        const size_t cur_offset = offset + i;
        size_t final_offset = cur_offset;
        if (cur_offset > 0x7FFF) {
            final_offset %= 0x7FFF;
        }
        const auto idx = (final_offset * final_offset + 71214) % key.size();
        const uint8_t mask = rotate(key[idx], static_cast<uint8_t>(idx) & 0x07);
        buf[i] ^= mask;
    }
}

} // namespace qqmusic::crypto
