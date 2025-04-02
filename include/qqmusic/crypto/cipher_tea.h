/**
 * @file cipher_tea.h
 * @author kingpoem
 * @brief TEA算法解密实现
 * @version 0.1
 * @date 2025-03-22
 * 
 * 实现QQ音乐加密格式使用的TEA变种算法，支持32轮解密操作。包含密钥派生体系的核心解密逻辑。
 * 与工程中现有的RC4/mapCipher解密器共享相同的内存管理策略和异常处理机制。
 * 
 * @copyright Copyright (c) 2025
 * @see https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/key_derive.go
 * @code{.cc}
 * qqmusic::crypto::TeaCipher::decrypt(buf, 0, deriveV2Key1);
 * @endcode
 */
#ifndef QQ_MUSIC_CIPHER_TEA_H
#define QQ_MUSIC_CIPHER_TEA_H

#include <botan/secmem.h>
#include <cstdint>
#include <format>
#include <iostream>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
using SecureByteVector = Botan::secure_vector<uint8_t>;

namespace qqmusic::crypto::TeaCipher {

inline void decrypt(qqmusic::utils::buffer& buf_in, size_t offset, const SecureByteVector& key) {
    constexpr int rounds = 32;

    if (key.size() != 16) {
        std::cout << std::format("Invalid key size: {}\n", key.size());
    }

    if (buf_in.size() < offset + 8) {
        std::cout << std::format("Buffer size is too small: {}\n", buf_in.size());
    }

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

    uint8_t* block = buf_in.data() + offset;
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

} // namespace qqmusic::crypto::TeaCipher

#endif // QQ_MUSIC_CIPHER_TEA_H
