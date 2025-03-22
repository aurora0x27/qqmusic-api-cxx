/**
 * @file key_derive.cc
 * @author kingpoem
 * @brief QQ音乐密钥派生算法实现
 * @version 0.1
 * @date 2025-03-22
 * 包含V1/V2两种密钥派生方案，使用TEA算法进行数据解密
 *
 * 原始输入 → rawKey (base64编码)
 *      ↓ 解码
 * 中间数据 → rawKeyDec (可能含V2头)
 *      ↓ 去头处理（如果是V2）
 * 核心密文 → raw (V2解密输入)
 *      ↓ 双重TEA解密
 * 最终数据 → 再次base64解码 → 最终密钥
 * @copyright Copyright (c) 2025
 * @code{.cc}
 * auto key = co_await keyDerive::derive(encryptedKey);
 * @endcode
 */
#include <algorithm>
#include <botan/base64.h>
#include <botan/secmem.h>
#include <cmath>
#include <format>
#include <qqmusic/crypto/cipher_tea.h>
#include <qqmusic/crypto/key_derive.h>
#include <qqmusic/result.h>
#include <ranges>
#include <vector>

namespace qqmusic::crypto {

/**
 * @brief 主要的密钥派生函数，处理派生逻辑
 * 当检测到V2前缀时，先进行V2解密，然后进行V1解密
 * 如果没有V2前缀，则直接进行V1解密
 * @param rawKey 
 * @return qqmusic::Task<qqmusic::Result<std::vector<uint8_t>>> 
 */
qqmusic::Task<qqmusic::Result<SecureByteVector>> KeyDerive::derive(const SecureByteVector& raw_key) {
    auto raw_key_dec = Botan::base64_decode(std::string(raw_key.begin(), raw_key.end()));

    // V2 前缀处理
    static constexpr std::string_view V2_prefix = "QQMusic EncV2,Key:";
    if (std::string_view(reinterpret_cast<const char*>(raw_key_dec.data()), raw_key_dec.size())
            .starts_with(V2_prefix)) {
        auto V2_res = derive_V2({raw_key_dec.begin() + V2_prefix.size(), raw_key_dec.end()});
        if (V2_res.isErr()) {
            co_return Err(utils::Exception(utils::Exception::DataDestroy,
                                           std::format("[derive] V2 processing failed: {}",
                                                       V2_res.unwrapErr().what())));
        }
        raw_key_dec = V2_res.unwrap();
    }

    co_return derive_V1(raw_key_dec);
}

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
 * 
 * @return qqmusic::Result<SecureByteVector>
 */
qqmusic::Result<SecureByteVector> KeyDerive::derive_V1(const SecureByteVector& raw_key_dec) {
    // Check key length (minimum 16 bytes)
    if (raw_key_dec.size() < 16) {
        return Err(utils::Exception(utils::Exception::DataDestroy,
                                    std::format("[derive_V1] Key length {} too short",
                                                raw_key_dec.size())));
    }

    const auto simple_key = makeSimpleKey(106, 8);

    // Build TEA key
    SecureByteVector tea_key(16);
    for (size_t i = 0; i < 8; ++i) {
        tea_key.push_back(simple_key[i]);
        tea_key.push_back(raw_key_dec[i]);
    }

    // Decrypt core data
    const auto decrypt_res = decrypt_tencent_tea({raw_key_dec.begin() + 8, raw_key_dec.end()},
                                                 tea_key);

    if (decrypt_res.isErr()) {
        return Err(utils::Exception(utils::Exception::DataDestroy,
                                    std::format("[derive_key_V1] TEA decrypt failed: {}",
                                                decrypt_res.unwrapErr().what())));
    }

    // Concatenate final result
    SecureByteVector res;
    res.reserve(8 + decrypt_res.unwrap().size());
    res.insert(res.end(), raw_key_dec.begin(), raw_key_dec.begin() + 8);
    res.insert(res.end(), decrypt_res.unwrap().begin(), decrypt_res.unwrap().end());

    return Ok(res);
}

/**
 * @brief V2密钥处理
 * 
 * @param raw 
 * @return qqmusic::Result<std::vector<uint8_t>> 
 */
qqmusic::Result<SecureByteVector> KeyDerive::derive_V2(SecureByteVector raw) {
    auto first_buf = decrypt_tencent_tea(std::move(raw), DeriveV2Key1);
    auto second_buf = decrypt_tencent_tea(std::move(first_buf.unwrap()), DeriveV2Key2);

    // Base64
    auto decode_buf = second_buf.unwrap();
    const std::string encoded(decode_buf.begin(), decode_buf.end());
    auto res = Botan::base64_decode(encoded);
    return Ok(SecureByteVector(res.begin(), res.end()));
}

/**
 * @brief 腾讯TEA算法核心解密函数
 * 
 * 实现标准TEA解密算法的变种，支持不同阶段的解密需求：
 * 
 * 处理流程：
 * 1. 输入校验：块大小8字节对齐，最小长度16字节
 * 2. 初始化TEA算法（32轮迭代）
 * 3. 解密首块获取填充长度
 * 4. 处理2字节盐值（随机干扰数据）
 * 5. 循环解密数据主体
 * 6. 校验7字节零值区
 * 
 * @param in_buf 加密数据流，包含：
 *               - V2阶段：Base64解码后的原始数据，但并未解密
 *               - V1阶段：去除8字节校验头的核心密文
 *               @b 要求：长度必须为8的倍数且≥16字节
 * 
 * @param key 解密密钥，包含：
 *           - V2阶段：固定密钥 DeriveV2Key1/DeriveV2Key2
 *           - V1阶段：动态生成的16字节混合密钥
 *           - 固定密钥：deriveV2Key1/deriveV2Key2（V2阶段） 动态密钥：基于simpleKey生成的teaKey（V1阶段）
 *           @b 长度：必须为16字节
 * 
 * @return qqmusic::Result<SecureByteVector> 返回结果包含：
 *         - Ok状态：解密后的明文数据
 *         - Err状态：包含以下可能的错误类型：
 *           * DataDestroy::InvalidTEAInput 输入数据非法
 * 
 * @note 算法特性：
 * - 使用CBC模式变种，每块依赖前一块的IV
 * - 包含盐值和零值校验机制
 * - 输出长度 = 输入长度 -1(填充头) - pad_len(填充长度) - 2(盐值) -7(零值)
 */
qqmusic::Result<SecureByteVector> KeyDerive::decrypt_tencent_tea(const SecureByteVector& in_buf,
                                                                 const SecureByteVector& key) {
    constexpr size_t salt_len = 2;
    constexpr size_t zero_len = 7;

    // Block validation
    if (in_buf.size() % 8 != 0 || in_buf.size() < 16) {
        return Err(utils::Exception(utils::Exception::DataDestroy, "Invalid TEA input size"));
    }

    // Initialize TEA
    auto tea = qqmusic::crypto::TeaCipher(key, 32);

    // Decrypt first block
    SecureByteVector dest_buf(8);
    tea.decrypt(in_buf.data(), dest_buf.data());
    const uint8_t pad_len = dest_buf[0] & 0x7;
    const size_t out_len = in_buf.size() - 1 - pad_len - salt_len - zero_len;

    auto xor8Bytes = [](auto& dst, const auto& a, const auto& b) {
        for (int i = 0; i < 8; ++i) {
            dst[i] = a[i] ^ b[i];
        }
    };

    SecureByteVector out(out_len);
    std::array<uint8_t, 8> iv_prev{}, ivCur{};
    std::copy_n(in_buf.begin(), 8, ivCur.begin());

    // Decrypt main blocks
    size_t in_buf_pos = 8;
    size_t dest_idx = 1 + pad_len;

    auto crypto_block = [&] {
        iv_prev = ivCur;
        std::copy_n(in_buf.begin() + in_buf_pos, 8, ivCur.begin());

        xor8Bytes(dest_buf, dest_buf, in_buf.data() + in_buf_pos);
        auto res = tea.decrypt(dest_buf.data(), dest_buf.data());

        in_buf_pos += 8;
        dest_idx = 0;
    };

    // Salt process
    for (size_t i = 0; i < salt_len;) {
        if (dest_idx < 8)
            ++dest_idx, ++i;
        else if (dest_idx == 8)
            crypto_block();
    }

    // main decrypt
    size_t out_pos = 0;
    while (out_pos < out_len) {
        if (dest_idx < 8) {
            out[out_pos++] = dest_buf[dest_idx] ^ iv_prev[dest_idx];
            dest_idx++;
        } else if (dest_idx == 8) {
            crypto_block();
        }
    }

    // Zero check
    for (size_t i = 0; i < zero_len; ++i) {
        if (dest_buf[dest_idx] != iv_prev[dest_idx]) {
            return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                                 "[decrypt_tencent_tea] -- zero check failed"));
        }
        ++dest_idx;
    }

    return Ok(std::move(out));
}

} // namespace qqmusic::crypto
