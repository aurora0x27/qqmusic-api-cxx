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
#include <vector>

namespace qqmusic::crypto {

class RC4Cipher {
public:
    explicit RC4Cipher(const std::vector<uint8_t>&& key_)
        : key(key_)
        , hash_base(1) {
        box.resize(key.size());
        std::iota(box.begin(), box.end(), 0);

        // KSA key scheduling algorithm
        [this]() {
            size_t j = 0;
            for (size_t i = 0; i < key.size(); ++i) {
                j = (j + box[i] + key[i % key.size()] % key.size());
                std::swap(box[i], box[j]);
            }
        }();

        // Calculate hash value
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
     */
    void decrypt(qqmusic::utils::buffer& buf, size_t offset) {
        auto process_block =
            [&](const std::function<void(qqmusic::utils::buffer&, size_t, size_t)>& encryptor,
                size_t block_size) {
                const size_t chunk = std::min(block_size, static_cast<size_t>(buf.size()));
                if (chunk == 0)
                    return true;
                encryptor(buf, chunk, offset);
                offset += chunk;
                buf.erase(buf.begin(), buf.begin() + static_cast<long>(chunk));
                return buf.empty();
            };

        // process first segment
        if (offset < FirstSegmentSize) {
            if (process_block([this](qqmusic::utils::buffer& buf,
                                     auto len,
                                     auto off) { this->enc_first_segment(buf, len, off); },
                              FirstSegmentSize - offset))
                return;
        }

        if (offset % SegmentSize != 0) {
            const auto remain = SegmentSize - (offset % SegmentSize);
            if (process_block([this](qqmusic::utils::buffer& buf,
                                     auto len,
                                     auto off) { this->enc_a_segment(buf, len, off); },
                              remain))
                return;
        }

        while (process_block([this](qqmusic::utils::buffer& buf,
                                    auto len,
                                    auto off) { this->enc_a_segment(buf, len, off); },
                             SegmentSize)) {
        }

        // process last segment
        if (!buf.empty()) {
            enc_a_segment(buf, buf.size(), offset);
        }
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
        for (size_t i = -skip_len; i < len; ++i) {
            j = (j + i) % key.size();
            k = (local_box[j] + k) % key.size();
            std::swap(local_box[i], local_box[k]);
            if (i >= 0) {
                buf[i] ^= local_box[(local_box[j] + local_box[k]) % key.size()];
            }
        }
    }

    // Skip value calculation
    size_t calc_segment_skip(size_t id) const noexcept {
        const auto seed = key[id % key.size()];
        const auto idx = static_cast<size_t>((hash_base / ((id + 1) * seed)) * 100);
        return idx % key.size();
    }

    std::vector<uint8_t> key;
    std::vector<uint8_t> box;
    uint32_t hash_base;

    static constexpr size_t SegmentSize = 5120;
    static constexpr size_t FirstSegmentSize = 128;
};

} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_CIPHER_RC4_H
