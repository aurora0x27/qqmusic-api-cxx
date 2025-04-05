/**
 * @file cipher_tea.h
 * @author kingpoem
 * @brief TEA算法解密实现
 * @version 0.1
 * @date 2025-03-22
 * 实现QQ音乐加密格式使用的TEA变种算法，支持32轮解密，每次解密8字节
 * @copyright Copyright (c) 2025
 * @see https://pkg.go.dev/golang.org/x/crypto/tea
 * @code{.cc}
 * for (size_t offset = 0; offset < buf.size(); offset += 8) {
 *     qqmusic::crypto::TeaCipher::decrypt(buf, offset, key);
 * }
 * @endcode
 * @note 该接口未测试，暂不可用
 */
#ifndef QQ_MUSIC_CIPHER_TEA_H
#define QQ_MUSIC_CIPHER_TEA_H

#include <botan/secmem.h>
#include <cstdint>
#include <qqmusic/utils/buffer.h>

namespace qqmusic::crypto::TeaCipher {

/**
 * @brief tea解密，每次解密8字节
 * @param buf 待解密的密钥
 * @param offset 偏移量
 * @param key 解密需要用到的密钥，在 key_derive.cc 中生成
 */
void decrypt(qqmusic::utils::buffer& buf, size_t offset, const std::vector<uint8_t>& key);

} // namespace qqmusic::crypto::TeaCipher

#endif // QQ_MUSIC_CIPHER_TEA_H
