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
 * std::vector<uint8_t> encrypted = GetEncryptedKey();
 * auto tea = qqmusic::crypto::TeaCipher(deriveV2Key1, 32);
 * auto stage1 = tea.decrypt(encrypted);
 * @endcode
 */
#ifndef QQ_MUSIC_CIPHER_TEA_H
#define QQ_MUSIC_CIPHER_TEA_H

#include <algorithm>
#include <botan/secmem.h>
#include <cstdint>
#include <qqmusic/result.h>
#include <ranges>
#include <stdexcept>
#include <vector>
using SecureByteVector = Botan::secure_vector<uint8_t>;

namespace qqmusic::crypto {

/**
 * @brief TEA解密核心实现类
 * 
 * 封装符合QQ音乐规范的TEA解密操作，支持自定义解密轮次。解密过程修改输入缓冲区内容
 */
class TeaCipher {
public:
    /**
     * @brief 初始化TEA解密器
     * @param key 16字节解密密钥
     * @param rounds 解密轮次，默认32轮
     * @throws std::invalid_argument 密钥长度错误时抛出
     */
    explicit TeaCipher(const SecureByteVector& key, int rounds = 32)
        : m_rounds(rounds) {
        if (key.size() != 16) {
            throw std::invalid_argument("TEA key must be 16 bytes");
        }
        std::ranges::copy(key, m_key.begin());
    }

    /**
     * @brief 执行块解密操作
     * @param inout 输入输出缓冲区
     * @return qqmusic::Result<void> 解密结果
     */
    qqmusic::Task<qqmusic::Result<void>> decrypt(const uint8_t* src, uint8_t* dest) const {
        uint32_t v0 = load_u32(dest);
        uint32_t v1 = load_u32(dest + 4);
        uint32_t sum = 0x9E3779B9 * m_rounds;

        for (int i = 0; i < m_rounds; i++) {
            v1 -= ((v0 << 4) + m_key[8 + (sum >> 11 & 3)]) ^ (v0 + sum)
                  ^ ((v0 >> 5) + m_key[12 + (sum >> 11 & 3)]);
            v0 -= ((v1 << 4) + m_key[0 + (sum & 3)]) ^ (v1 + sum)
                  ^ ((v1 >> 5) + m_key[4 + (sum & 3)]);
            sum -= 0x9E3779B9;
        }

        store_u32(dest, v0);
        store_u32(dest + 4, v1);
        co_return Ok();
    }

private:
    std::array<uint8_t, 16> m_key; ///< 解密密钥存储
    int m_rounds;                  ///< 解密轮次计数器

    static uint32_t load_u32(const uint8_t* p) {
        return (static_cast<uint32_t>(p[3]) << 24) | (static_cast<uint32_t>(p[2]) << 16)
               | (static_cast<uint32_t>(p[1]) << 8) | (static_cast<uint32_t>(p[0]));
    }

    static void store_u32(uint8_t* p, uint32_t v) {
        p[0] = v & 0xFF;
        p[1] = (v >> 8) & 0xFF;
        p[2] = (v >> 16) & 0xFF;
        p[3] = (v >> 24) & 0xFF;
    }
};

} // namespace qqmusic::crypto

#endif // !QQ_MUSIC_CIPHER_TEA_H
