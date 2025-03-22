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
 * std::vector<uint8_t> key = {0xAA, 0xBB, 0xCC};
 * qqmusic::crypto::MapCipher cipher(key); // 初始化解码器
 * auto cipher_res = co_await cipher.decrypt(key, in_buf, 0); // 解密数据
 * if (cipher_res.isErr()) {
 *      co_return Err(qqmusic::utils::Exception(
 *          qqmusic::utils::Exception::DataDestroy,
 *          std::format("Error occurred when decrypting: `{}`",
 *          cipher_res.unwrapErr().what())));
 * }
 * auto data = cipher_res.unwrap()
 * @endcode
 */
#ifndef QQ_MUSIC_CIPHER_MAP_H
#define QQ_MUSIC_CIPHER_MAP_H

#include <cstdint>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <stdexcept>
#include <vector>

namespace qqmusic::crypto {

/**
 * @brief 解密类
 * 包括解码器的构造和解码操作
 */
class MapCipher {
public:
    /**
     * @brief Construct a new Map Cipher object
     * 传入密钥来构建解码器
     * @param key 
     */
    explicit MapCipher(const std::vector<uint8_t>& key)
        : key_(std::move(key))
        , box_(key.size())
        , size_(key.size()) {}

    /**
     * @brief 执行解密操作
     * 直接对缓冲区里的数据进行解密，解密出来的歌词结果和元数据存在一个结构体里
     * @param buf 输入/输出缓冲区
     * @param offset 数据在文件中的起始偏移量
     * @return qqmusic::Task<qqmusic::Result<void>>
     * @todo 返回值应该是一个包含解密数据和元数据的结构体
     */
    static qqmusic::Task<qqmusic::Result<void>> decrypt(qqmusic::utils::buffer& buf, size_t offset) {
        const auto rotate = [](uint8_t value, uint8_t bits) {
            const uint8_t r = (bits + 4) % 8;
            return (value << r) | (value >> (8 - r));
        };
        for (size_t i = 0; i < buf.size(); ++i) {
            const int cur_offset = offset + static_cast<int>(i);
            int final_offset = cur_offset;
            if (cur_offset > 0x7FFF) {
                final_offset %= 0x7FFF;
            }
            const auto idx = (final_offset * final_offset + 71214) % static_cast<int>(size_);
            const uint8_t mask = rotate(key_[idx], static_cast<uint8_t>(idx) & 0x07);
            buf[i] ^= mask;
        }
        co_return;
    }

private:
    std::vector<uint8_t> key_; // 存储原始解密密钥
    std::vector<uint8_t> box_; // 可能用于缓存预处理后的密钥，在该文件中仅初始化，长度为size_
    size_t size_; // key 的长度
};

} // namespace qqmusic::crypto

#endif // !QQ_MUSIC_CIPHER_MAP_H
