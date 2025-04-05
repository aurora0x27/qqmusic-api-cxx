/**
 * @file qmc.h
 * @author kingpoem
 * @brief QQ音乐加密文件解码器核心模块
 * @version 0.1
 * @date 2025-03-26
 * 将API请求得到的ekey存取到解码器的ekey中，根据长度选择解密算法（>300字节为RC4,其他为MapCipher）
 * 将解密后的密钥存取到key中，完成解密过程
 * 需要解密的数据存取到buf_in中，解密完成的数据存取到buf_out中
 * @copyright Copyright (c) 2025
 * @see https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/qmc.go
 * @code{.cc}
 * SongInfo song = ...;
 * if (song.ekey.has_value()) {
 *      qqmusic::crypto::Decoder decoder(*song.ekey);
 *      if (read2buf(buf)) {
 *          if (decoder.decrypt()) {
 *              std::cout << "decrypt successful" << std::endl;
 *          }
 *      }
 * } else {
 *      // throw error
 * }
 * // then can use `buf_out`
 * @endcode
 * @note 该接口未测试，暂不可用
 */
#ifndef QQMUSIC_CRYPTO_QMC_H
#define QQMUSIC_CRYPTO_QMC_H

#include <cstdint>
#include <memory>
#include <qqmusic/crypto/cipher_map.h>
#include <qqmusic/crypto/cipher_rc4.h>
#include <qqmusic/crypto/cipher_tea.h>
#include <qqmusic/crypto/key_derive.h>
#include <qqmusic/details/result.h>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::crypto {

class Decoder {
public:
    /**
     * @brief 解码器构造函数
     * @param ekey 需要传入加密的密钥
     */
    explicit Decoder(std::string&& ekey);

    /**
     * @brief 解码器完整解码过程
     * 1. 解密密钥
     * 2. 初始化解密算法
     * 3. 进行解密
     */
    bool decrypt();

    /**
     * @brief 存取待解密数据到 buf_in 中
     * @param buf 缓冲区（存放未解密的数据）
     */
    bool read2buf(qqmusic::utils::buffer&& buf);

private:
    qqmusic::utils::buffer ekey;                     ///< 加密密钥
    std::vector<uint8_t> key;                        ///< 解密后的密钥
    qqmusic::utils::buffer buf_in{1024};             ///< 待解密数据
    qqmusic::utils::buffer buf_out{1024};            ///< 成功解密的数据
    std::unique_ptr<qqmusic::crypto::Cipher> cipher; ///< 解密算法
};
} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_QMC_H
