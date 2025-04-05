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
 * @note 该接口未测试，暂不可用
 */
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

    /**
     * @brief map解密算法
     */
    void decrypt(qqmusic::utils::buffer& buf, size_t offset) override;

private:
    std::vector<uint8_t> key;
};

} // namespace qqmusic::crypto

#endif // !QQ_MUSIC_CIPHER_MAP_H
