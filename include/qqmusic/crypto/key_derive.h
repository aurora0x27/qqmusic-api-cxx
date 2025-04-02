/**
 * @file key_derive.h
 * @author kingpoem
 * @brief 密钥生成模块
 * @version 0.1
 * @date 2025-03-16
 * 解码器初始化时需要一个key，该文件的作用是根据需要解码的数据判断并生成需要提供给相应解码器的密钥
 * 
 * 该文件实现QQ音乐加密格式的密钥派生体系，通过解析Base64编码的输入密钥，区分V1/V2两种加密版本，
 * 使用TEA算法进行多层解密（V2需两次TEA+Base64嵌套解密），最终生成用于RC4/mapCipher等音频解密算法的有效密钥。
 * 内部包含数学变换密钥生成、块解密时盐值处理（2字节随机盐+7字节零校验）、以及跨平台兼容的字节序处理等安全机制。
 * 
 * 传入的是从加密文件头部提取的Base64编码的密钥字符串
 * 输出的密钥直接用于初始化音频解码器
 * @copyright Copyright (c) 2025
 * @see https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/key_derive.go
 * @code{.cc}
 * SecureByteVector encrypted_key = {...}; // 原始加密密钥
 * auto real_key = qqmusic::crypto::KeyDrive::derive(encrypted_key);
 * @endcode
 */
#ifndef QQMUSIC_CRYPTO_KEY_DERIVE_H
#define QQMUSIC_CRYPTO_KEY_DERIVE_H

#include <botan/secmem.h>
#include <cstdint>
#include <qqmusic/result.h>
using SecureByteVector = Botan::secure_vector<uint8_t>;

namespace qqmusic::crypto {

class KeyDerive {
public:
    static SecureByteVector derive(const SecureByteVector& raw_key);

private:
    static SecureByteVector derive_V1(const SecureByteVector& raw_key_dec);
    static SecureByteVector derive_V2(SecureByteVector raw);

    static SecureByteVector decrypt_tencent_tea(const SecureByteVector& in_buf,
                                                const SecureByteVector& key);

    static SecureByteVector makeSimpleKey(uint8_t salt, size_t len) {
        SecureByteVector key(len); // 修改容器类型
        std::ranges::generate(key, [=, i = 0]() mutable {
            const double tmp = std::tan(salt + i++ * 0.1);
            return static_cast<uint8_t>(std::abs(tmp) * 100.0);
        });
        return key;
    }

    static inline const SecureByteVector DeriveV2Key1 = {{0x33,
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

    static inline const SecureByteVector DeriveV2Key2 = {{0x2A,
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
