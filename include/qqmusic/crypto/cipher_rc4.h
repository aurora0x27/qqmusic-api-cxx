/**
 * @file cipher_rc4.h
 * @author kingpoem
 * @brief cipher_rc4
 * @version 0.1
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2025
 * @see https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/cipher_rc4.go
 * @code{.cc}
 * std::vector<uint8_t> key = {0xAA, 0xBB, 0xCC};
 * qqmusic::crypto::RC4Cipher cipher(key); // 初始化解码器
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
#ifndef QQMUSIC_CRYPTO_CIPHER_RC4_H
#define QQMUSIC_CRYPTO_CIPHER_RC4_H

#include <cstdint>
#include <numeric>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <stdexcept>
#include <vector>

namespace qqmusic::crypto {

class RC4Cipher {
public:
    explicit RC4Cipher(const std::vector<uint8_t>& key)
        : key_(std::move(key))
        , size_(key.size()) {
        box.resize(size_);
        std::iota(box.begin(), box.end(), 0);

        // KSA key scheduling algorithm
        [this]() {
            size_t j = 0;
            for (size_t i = 0; i < size_; ++i) {
                j = (j + box[i] + key[i % size_] % size_);
                std::swap(box[i], box[j]);
            }
        }();

        // Calculate hash value
        hash_base = 1;
        [this]() {
            for (auto k : key) {
                if (k == 0)
                    continue;
                const auto next_hash = hash_base * k;
                if (next_hash == 0 || next_hash <= hash_base)
                    break;
                hash_base = next_hash;
            }
        }();
    }

    /**
     * @brief 执行解密操作
     * 
     * @param buf 
     * @param offset 
     * @return qqmusic::Task<qqmusic::Result<void>> 
     */
    qqmusic::Task<qqmusic::Result<void>> decrypt(qqmusic::utils::buffer& buf, size_t offset) {
        auto process_block = [&](auto&& func, size_t block_size) {
            const auto chunk = std::min(block_size, buf.size());
            if (chunk == 0)
                return true;
            func(buf.data(), chunk, offset);
            offset += chunk;
            buf.erase(buf.begin(), buf.begin() + chunk);
            return buf.empty();
        };

        // process first segment
        if (offset < FirstSegmentSize) {
            if (process_block([this](auto* buf,
                                     auto len,
                                     auto off) { this->enc_first_segment(buf, len, off); },
                              FirstSegmentSize - offset))
                return;
        }

        if (offset % SegmentSize != 0) {
            const auto remain = SegmentSize - (offset % SegmentSize);
            if (process_block([this](auto* buf,
                                     auto len,
                                     auto off) { this->enc_a_segment(buf, len, off); },
                              remain))
                return;
        }

        while (process_block([this](auto* buf,
                                    auto len,
                                    auto off) { this->enc_a_segment(buf, len, off); },
                             SegmentSize)) {
        }

        // process last segment
        if (!data.empty()) {
            enc_a_segment(buf.data(), buf.size(), offset);
        }

        co_return;
    }

private:
    // Encrypt first special segment
    void enc_first_segment(qqmusic::utils::buffer& buf, size_t len, size_t offset) const {
        for (size_t i = 0; i < len; ++i) {
            buf[i] ^= key[calc_segment_skip(offset + i)];
        }
    }

    // Encrypt regular segment
    void enc_a_segment(qqmusic::utils::buffer& buf, size_t len, size_t offset) const {
        auto local_box = box;
        size_t j = 0, k = 0;
        const auto skip_len = (offset % SegmentSize) + calc_segment_skip(offset / SegmentSize);
        // Forward skipping
        for (int i = -skip_len; i < static_cast<int>(len); ++i) {
            j = (j + i) % size_;
            k = (local_box[j] + k) % size_;
            std::swap(local_box[i], local_box[k]);
            if (i >= 0) {
                buf[i] ^= local_box[(local_box[j] + local_box[k]) % size_];
            }
        }
    }

    // Skip value calculation
    size_t calc_segment_skip(size_t id) const noexcept {
        const auto seed = key[id % size_];
        const auto idx = static_cast<size_t>((hash_base / ((id + 1) * seed)) * 100.0);
        return idx % size_;
    }

    std::vector<uint8_t> key_;
    std::vector<uint8_t> box;
    uint32_t hash_bash;
    size_t size_; // key 的长度

    static constexpr size_t SegmentSize = 5120;
    static constexpr size_t FirstSegmentSize = 128;
};

} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_CIPHER_RC4_H
