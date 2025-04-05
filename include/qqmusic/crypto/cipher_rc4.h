/**
 * @file cipher_rc4.h
 * @author kingpoem
 * @brief cipher_rc4
 * @version 0.1
 * @date 2025-03-16
 * @copyright Copyright (c) 2025
 * @see https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/cipher_rc4.go
 * @code{.cc}
 * qqmusic::crypto::RC4Cipher cipher(key);
 * cipher.decrypt(buf, 0);
 * @endcode
 */
#ifndef QQMUSIC_CRYPTO_CIPHER_RC4_H
#define QQMUSIC_CRYPTO_CIPHER_RC4_H

#include <cstdint>
#include <qqmusic/crypto/cipher.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::crypto {

class RC4Cipher : public Cipher {
public:
    explicit RC4Cipher(const std::vector<uint8_t>& key)
        : key(key) {}

    void decrypt(qqmusic::utils::buffer& buf, size_t offset) override;

private:
    std::vector<uint8_t> key;
};

} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_CIPHER_RC4_H
