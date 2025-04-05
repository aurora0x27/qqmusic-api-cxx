/**
 * @file key_derive.h
 * @author kingpoem
 * @brief QQ音乐密钥解密算法实现
 * @version 0.1
 * @date 2025-03-16
 * 该文件实现QQ音乐加密格式的密钥派生体系，通过解析Base64编码的输入密钥，区分V1/V2两种加密版本，
 * 使用TEA算法进行多层解密（V2需两次TEA+Base64嵌套解密），最终生成用于RC4/mapCipher等音频解密算法的有效密钥。
 * 内部包含数学变换密钥生成、块解密时盐值处理（2字节随机盐+7字节零校验）、以及跨平台兼容的字节序处理等安全机制。
 * 
 * 传入的是从ekey提取的Base64编码的密钥字符串
 * 输出的密钥直接用于初始化音频解码器
 *
 * 原始输入ekey → rawKey (base64编码)
 *      ↓ 解码
 * 中间数据 → rawKeyDec (可能含V2头)
 *      ↓ 去头处理（如果是V2）
 * 核心密文 → raw (V2解密输入)
 *      ↓ 双重TEA解密
 * 最终数据 → 再次base64解码 → 最终密钥
 * @copyright Copyright (c) 2025
 * @see https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/key_derive.go
 * @code{.cc}
 * std::vector<uint8_t> encrypted_key = {...}; // 原始加密密钥
 * qqmusic::crypto::Decoder Decoder(ekey); // 用ekey初始化解码器，ekey存取为解码器的公有变量
 * auto key = qqmusic::crypto::KeyDrive::derive(Decoder.ekey);
 * @endcode
 * @note 该接口未测试，暂不可用
 */
#ifndef QQMUSIC_CRYPTO_KEY_DERIVE_H
#define QQMUSIC_CRYPTO_KEY_DERIVE_H

#include <botan/secmem.h>
#include <cstdint>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>

namespace qqmusic::crypto {

class KeyDerive {
public:
    /**
    * @brief 主要的密钥派生函数，处理派生逻辑
    * 当检测到V2前缀时，先进行V2解密，然后进行V1解密
    * 如果没有V2前缀，则直接进行V1解密
    * @param raw_key 原始密钥
    * @return qqmusic::utils::buffer 解密后的密钥
    */
    static qqmusic::utils::buffer derive(qqmusic::utils::buffer&& raw_key);

private:
    /**
    * @brief 执行QQ音乐V1版密钥派生算法
    * 
    * 处理流程：
    * 1. 校验密钥长度 ≥16 字节
    * 2. 生成106号简单密钥（8字节）
    * 3. 构建16字节TEA密钥（交替填充简单密钥和原始密钥头）
    * 4. 使用TEA算法解密核心数据块（原始密钥8字节之后的数据）
    * 5. 拼接原始密钥头(8字节) + 解密数据（可变长度） → 最终音频密钥 raw_key_dec + decrypt_res
    * 
    * @param raw_key_dec 经过Base64解码的中间密钥数据，包含：
    *                    - V2流程处理后的解密数据
    *                    - 直接输入的V1密钥数据
    *                    @b 最小长度：16字节
    */
    static qqmusic::utils::buffer derive_V1(qqmusic::utils::buffer&& raw_key_dec);

    /**
    * @brief V2密钥处理
    */
    static qqmusic::utils::buffer derive_V2(qqmusic::utils::buffer raw);

    /**
    * @brief 腾讯TEA算法核心解密函数
    * 实现标准TEA解密算法的变种，支持不同阶段的解密需求：
    * 处理流程：
    * 1. 输入校验：块大小8字节对齐，最小长度16字节
    * 2. 初始化TEA算法（32轮迭代）
    * 3. 解密首块获取填充长度
    * 4. 处理2字节盐值（随机干扰数据）
    * 5. 循环解密数据主体
    * 6. 校验7字节零值区
    * @param in_buf 加密数据流，包含：
    *               - V2阶段：Base64解码后的原始数据，但并未解密
    *               - V1阶段：去除8字节校验头的核心密文
    *               要求：长度必须为8的倍数且≥16字节
    * @param key 解密密钥，包含：
    *           - V2阶段：固定密钥 DeriveV2Key1/DeriveV2Key2
    *           - V1阶段：动态生成的16字节混合密钥
    *           - 固定密钥：deriveV2Key1/deriveV2Key2（V2阶段） 动态密钥：基于simpleKey生成的teaKey（V1阶段）
    *              长度：必须为16字节
    */
    static qqmusic::utils::buffer decrypt_tencent_tea(qqmusic::utils::buffer&& in_buf,
                                                      const std::vector<uint8_t>& key);

    inline static std::vector<uint8_t> makeSimpleKey(uint8_t salt, size_t len) {
        std::vector<uint8_t> key(len);
        std::ranges::generate(key, [=, i = 0]() mutable {
            const double tmp = std::tan(salt + i++ * 0.1);
            return static_cast<uint8_t>(std::abs(tmp) * 100.0);
        });
        return key;
    }

    inline static const std::vector<uint8_t> DeriveV2Key1 = {{0x33,
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

    inline static const std::vector<uint8_t> DeriveV2Key2 = {{0x2A,
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
