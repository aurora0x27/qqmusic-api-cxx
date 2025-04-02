/**
 * @file cipher.h
 * @author kingpoem
 * @brief cipher 的基类
 * @version 0.1
 * @date 2025-04-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef QQMUSIC_CRYPTO_CIPHER_H
#define QQMUSIC_CRYPTO_CIPHER_H

#include <qqmusic/utils/buffer.h>

namespace qqmusic::crypto {
class Cipher {
public:
    virtual ~Cipher() = default;
    virtual void decrypt(qqmusic::utils::buffer& buf, size_t offset) = 0;
};

} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_CIPHER_H
