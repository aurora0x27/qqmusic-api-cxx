/**
 * @file cipher_rc4.h
 * @author kingpoem
 * @brief cipher_rc4
 * @version 0.1
 * @date 2025-03-16
 * @copyright Copyright (c) 2025
 * @see https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/cipher_rc4.go
 * @code{.cc}
 * qqmusic::crypto::RC4Cipher::decrypt(buf, 0, key);
 * @endcode
 */
#ifndef QQMUSIC_CRYPTO_CIPHER_RC4_H
#define QQMUSIC_CRYPTO_CIPHER_RC4_H

#include <cstdint>
#include <numeric>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::crypto::RC4Cipher {

inline void decrypt(qqmusic::utils::buffer& buf, size_t offset, const std::vector<uint8_t>& key) {
    // 初始化S盒
    std::vector<uint8_t> box(key.size());
    std::iota(box.begin(), box.end(), 0);

    // KSA密钥调度算法
    size_t j = 0;
    for (size_t i = 0; i < key.size(); ++i) {
        j = (j + box[i] + key[i % key.size()]) % key.size();
        std::swap(box[i], box[j]);
    }

    // 哈希值计算
    uint32_t hash_base = 1;
    for (auto k : key) {
        if (k == 0)
            continue;
        const auto next_hash = hash_base * k;
        if (next_hash == 0 || next_hash <= hash_base)
            break;
        hash_base = next_hash;
    }

    // 解密主逻辑
    constexpr size_t SegmentSize = 5120;
    constexpr size_t FirstSegmentSize = 128;

    auto process_segment = [&](size_t block_size, auto enc_func) {
        const size_t chunk = std::min(block_size, buf.size());
        if (chunk == 0)
            return true;

        enc_func(buf, chunk, offset);
        offset += chunk;
        buf.erase(buf.begin(), buf.begin() + static_cast<long>(chunk));
        return buf.empty();
    };

    // 处理第一段
    if (offset < FirstSegmentSize) {
        auto enc_first = [&](auto& buf, auto len, auto off) {
            for (size_t i = 0; i < len; ++i) {
                const auto seg_id = (off + i) / SegmentSize;
                const auto seed = key[seg_id % key.size()];
                const auto idx = static_cast<double>(hash_base) / ((seg_id + 1) * seed) * 100.0;
                const auto skip = static_cast<size_t>(idx) % key.size();
                buf[i] ^= key[skip];
            }
        };
        if (process_segment(FirstSegmentSize - offset, enc_first))
            return;
    }

    // 处理常规段
    auto enc_regular = [&](auto& buf, auto len, auto off) {
        auto local_box = box;
        int j = 0, k = 0;
        const int seg_num = off / SegmentSize;
        const int seed = key[seg_num % key.size()];
        const int idx = static_cast<double>(hash_base) / ((seg_num + 1) * seed) * 100.0;
        const int skip_len = (off % SegmentSize) + (static_cast<size_t>(idx) % key.size());

        for (int i = -static_cast<int>(skip_len); i < static_cast<int>(len); ++i) {
            j = (j + i) % static_cast<int>(key.size());
            k = (local_box[j] + k) % static_cast<int>(key.size());
            std::swap(local_box[j], local_box[k]);
            if (i >= 0) {
                buf[i] ^= local_box[(local_box[j] + local_box[k]) % key.size()];
            }
        }
    };

    while (!process_segment(SegmentSize, enc_regular)) {
    }
}

} // namespace qqmusic::crypto::RC4Cipher

#endif // !QQMUSIC_CRYPTO_CIPHER_RC4_H
