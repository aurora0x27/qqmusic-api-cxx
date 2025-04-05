

# File key\_derive.cc

[**File List**](files.md) **>** [**crypto**](dir_4261af1259721e3e39e0d2dd7354b511.md) **>** [**key\_derive.cc**](key__derive_8cc.md)

[Go to the documentation of this file](key__derive_8cc.md)


```C++
#include <algorithm>
#include <botan/base64.h>
#include <botan/secmem.h>
#include <cmath>
#include <format>
#include <iostream>
#include <qqmusic/crypto/cipher_tea.h>
#include <qqmusic/crypto/key_derive.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::crypto {

qqmusic::utils::buffer KeyDerive::derive(qqmusic::utils::buffer&& raw_key) {
    auto base64_raw = Botan::base64_decode(std::string(raw_key.begin(), raw_key.end()));
    std::vector<uint8_t> raw_key_dec(base64_raw.begin(), base64_raw.end());

    // V2 前缀处理
    static constexpr std::string_view V2_prefix = "QQMusic EncV2,Key:";
    if (std::string_view(reinterpret_cast<const char*>(raw_key_dec.data()), raw_key_dec.size())
            .starts_with(V2_prefix)) {
        auto V2_res = derive_V2(
            std::vector<uint8_t>({raw_key_dec.begin() + V2_prefix.size(), raw_key_dec.end()}));
        raw_key_dec = V2_res;
    }

    return derive_V1(std::move(raw_key_dec));
}

qqmusic::utils::buffer KeyDerive::derive_V1(qqmusic::utils::buffer&& raw_key_dec) {
    // Check key length (minimum 16 bytes)
    if (raw_key_dec.size() < 16) {
        std::cout << std::format("[KeyDerive::derive_V1] -- raw_key_dec size {} < 16",
                                 raw_key_dec.size())
                  << std::endl;
        return {};
    }

    const auto simple_key = makeSimpleKey(106, 8);

    // Build TEA key
    std::vector<uint8_t> tea_key(16);
    for (size_t i = 0; i < 8; ++i) {
        tea_key.push_back(simple_key[i]);
        tea_key.push_back(raw_key_dec[i]);
    }

    // Decrypt core data
    auto raw_key_dec_headless = qqmusic::utils::buffer(
        std::vector<uint8_t>(raw_key_dec.begin() + 8, raw_key_dec.end()));
    const auto decrypt_res = decrypt_tencent_tea(std::move(raw_key_dec_headless), tea_key);

    // Concatenate final result
    std::vector<uint8_t> res;
    res.reserve(8 + decrypt_res.size());
    res.insert(res.end(), raw_key_dec.begin(), raw_key_dec.begin() + 8);
    res.insert(res.end(), decrypt_res.begin(), decrypt_res.end());

    return res;
}

qqmusic::utils::buffer KeyDerive::derive_V2(qqmusic::utils::buffer raw) {
    auto first_buf = decrypt_tencent_tea(std::move(raw), DeriveV2Key1);
    auto second_buf = decrypt_tencent_tea(std::move(first_buf), DeriveV2Key2);

    // Base64
    auto decode_buf = second_buf;
    const std::string encoded(decode_buf.begin(), decode_buf.end());
    auto base64_res = Botan::base64_decode(encoded);
    std::vector<uint8_t> res(base64_res.begin(), base64_res.end());
    return res;
}

qqmusic::utils::buffer KeyDerive::decrypt_tencent_tea(qqmusic::utils::buffer&& in_buf,
                                                      const std::vector<uint8_t>& key) {
    constexpr size_t salt_len = 2;
    constexpr size_t zero_len = 7;

    // Block validation
    if (in_buf.size() % 8 != 0 || in_buf.size() < 16) {
        std::cout << std::format("[KeyDerive::decrypt_tencent_tea] -- in_buf len {} < 16",
                                 in_buf.size())
                  << std::endl;
        return {};
    }

    // Decrypt first block
    qqmusic::utils::buffer dest_buf(8);
    for (size_t offset = 0; offset < dest_buf.size(); offset += 8) {
        qqmusic::crypto::TeaCipher::decrypt(in_buf, offset, key);
    }
    const uint8_t pad_len = dest_buf[0] & 0x7;
    const size_t out_len = in_buf.size() - 1 - pad_len - salt_len - zero_len;

    auto xor8Bytes = [](auto& dst, const auto& a, const auto& b) {
        for (int i = 0; i < 8; ++i) {
            dst[i] = a[i] ^ b[i];
        }
    };

    std::vector<uint8_t> out(out_len);
    std::array<uint8_t, 8> iv_prev{}, ivCur{};
    std::copy_n(in_buf.begin(), 8, ivCur.begin());

    // Decrypt main blocks
    size_t in_buf_pos = 8;
    size_t dest_idx = 1 + pad_len;

    auto crypto_block = [&] {
        iv_prev = ivCur;
        std::copy_n(in_buf.begin() + static_cast<long>(in_buf_pos), 8, ivCur.begin());

        xor8Bytes(dest_buf, dest_buf, in_buf.data() + in_buf_pos);
        for (size_t offset = 0; offset < dest_buf.size(); offset += 8) {
            qqmusic::crypto::TeaCipher::decrypt(dest_buf, offset, key);
        }

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
            std::cout << "[KeyDerive::decrypt_tencent_tea] -- zero check failed" << std::endl;
            return {};
        }
        ++dest_idx;
    }

    return out;
}

} // namespace qqmusic::crypto
```


