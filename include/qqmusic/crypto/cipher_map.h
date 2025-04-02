/**
 * @file cipher_map.h
 * @author kingpoem
 * @brief cipher_map
 * @version 0.1
 * @date 2025-03-15
 * 解密算法中的一种，适用于QQ音乐的早期加密
 * @copyright Copyright (c) 2025
 * @see https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/cipher_map.go
 * @code{.cc}
 * qqmusic::crypto::MapCipher cipher(key);
 * cipher.decrypt(buf, 0);
 * @endcode
 */
#ifndef QQ_MUSIC_CIPHER_MAP_H
#define QQ_MUSIC_CIPHER_MAP_H

#include <cstdint>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::crypto {

class MapCipher {
public:
    explicit MapCipher(const std::vector<uint8_t>& key_)
        : key(key_) {}

    void decrypt(qqmusic::utils::buffer& buf, size_t offset) {
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

private:
    std::vector<uint8_t> key;
};

} // namespace qqmusic::crypto

#endif // !QQ_MUSIC_CIPHER_MAP_H
