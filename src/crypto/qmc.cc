#include <qqmusic/crypto/cipher_map.h>
#include <qqmusic/crypto/cipher_rc4.h>
#include <qqmusic/crypto/cipher_tea.h>
#include <qqmusic/crypto/key_derive.h>
#include <qqmusic/crypto/qmc.h>
#include <qqmusic/utils/buffer.h>
#include <utility>

namespace qqmusic::crypto {

Decoder::Decoder(std::string&& ekey)
    : ekey((uint8_t*) ekey.data(), ekey.size()){};

bool read2buf(qqmusic::utils::buffer&& buf) {
    qqmusic::utils::buffer buf_in(std::move(buf));
    if (buf_in.size() < 0) {
        return false;
    } else {
        return true;
    }
}

bool Decoder::decrypt() {
    // 解密密钥
    auto key = qqmusic::crypto::KeyDerive::derive(std::move(ekey));

    // 初始化解密算法
    if (key.size() > 300) {
        cipher = std::make_unique<RC4Cipher>(key);
        cipher->decrypt(buf_in, 0);
        buf_out.insert(buf_out.end(), buf_in.begin(), buf_in.end());
    } else if (!key.empty()) {
        cipher = std::make_unique<MapCipher>(key);
        cipher->decrypt(buf_in, 0);
        buf_out.insert(buf_out.end(), buf_in.begin(), buf_in.end());
    } else {
        std::cout << "Decoder decrypt failed! key size abnormal!" << std::endl;
        return false;
    }
    return true;
}

} // namespace qqmusic::crypto
