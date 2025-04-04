#include <qqmusic/crypto/cipher_map.h>
#include <qqmusic/crypto/cipher_rc4.h>
#include <qqmusic/crypto/cipher_tea.h>
#include <qqmusic/crypto/key_derive.h>
#include <qqmusic/crypto/qmc.h>
#include <qqmusic/utils/buffer.h>
#include <span>
#include <utility>

namespace qqmusic::crypto {

Decoder::Decoder(std::string_view ekey) {}

std::pair<size_t, qqmusic::utils::buffer> read2buf(qqmusic::utils::buffer&& buf_) {
    qqmusic::utils::buffer buf_in(std::move(buf_));
    return {buf_in.size(), std::move(buf_in)};
}

bool Decoder::decrypt() {
    // 解密密钥
    auto key = qqmusic::crypto::KeyDerive::derive(std::move(ekey));

    // 初始化解密算法
    if (key.size() > 300) {
        cipher = std::make_unique<RC4Cipher>(key);
    } else if (!key.empty()) {
        cipher = std::make_unique<MapCipher>(key);
    }

    constexpr size_t validate_size = 64;
    if (buf_in.size() < validate_size) {
        return false;
    }

    // auto header = std::span(buf_in).subspan(0, validate_size);
    // cipher->decrypt(header, 0);

    current_offset = 0;
    return true;
}

} // namespace qqmusic::crypto
