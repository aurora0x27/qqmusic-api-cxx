/**
 * @file qmc.h
 * @author kingpoem
 * @brief QQ音乐加密文件解码器核心模块
 * @version 0.1
 * @date 2025-03-26
 * 本模块实现QQ音乐多种加密格式的统一解密，支持qmc0/qmc2/qmcflac等20+格式
 * 包含密钥查找、算法路由、格式验证等核心机制
//  工作流程分为初始化、验证解密、数据读取三个阶段：
// 1. 初始化阶段
// [NewDecoder] → 创建解码器实例
//    │
//    ↓
// [用户调用 Validate()] → 进入核心验证流程

// 2. 验证解密阶段
//               ┌───────────────┐
//               │ searchKey()   │
//               │ 1. 分析文件尾 │
//               │ 2. 识别特征码 │
//               └───┬───┬───┬───┘
//                   │   │   │
//         ┌─────────┘   │   └───────────┐
//         │             │               │
// ┌───────▼───────┐ ┌───▼───┐ ┌─────────▼─────────┐
// │ readRawMetaQTag │ │ cex处理 │ │ readRawKey      │
// │ (QTag格式解析)  │ │ (音乐扩展标签)│ │ (原始密钥提取)  │
// └───────┬───────┘ └───┬───┘ └─────────┬─────────┘
//         │             │               │
//         └─────┬───────┴───────┬───────┘
//               │ 获取解密密钥 │
//               └───────┬───────┘
//                       │
//           ┌───────────▼───────────┐
//           │ 选择解密算法           │
//           │ • RC4(密钥>300字节)    │
//           │ • MapCipher(密钥≠0)   │
//           │ • StaticCipher(默认)  │
//           └───────────┬───────────┘
//                       │
//           ┌───────────▼───────────┐
//           │ validateDecode()      │
//           │ • 解密文件头64字节     │
//           │ • 验证音频格式有效性   │
//           └───────────┬───────────┘
//                       │
//           ┌───────────▼───────────┐
//           │ 准备音频读取器         │
//           │ • 重置文件指针         │
//           │ • 创建长度限制读取器   │
//           └───────────┬───────────┘

// 3. 数据读取阶段
//        ┌───────▼───────┐
//        │ Read() 循环调用 │
//        │ 1. 读取加密数据 │
//        │ 2. 实时解密    │
//        │ 3. 记录偏移量  │
//        │ 4. 缓存探测数据│
//        └───────────────┘
 * 
 * @copyright Copyright (c) 2025
 * @see https://git.unlock-music.dev/um/cli/src/branch/main/algo/qmc/qmc.go
 * 
 */
#ifndef QQMUSIC_CRYPTO_QMC_H
#define QQMUSIC_CRYPTO_QMC_H

#include <botan/secmem.h>
#include <cstdint>
#include <memory>
#include <qqmusic/crypto/cipher_map.h>
#include <qqmusic/crypto/cipher_rc4.h>
#include <qqmusic/crypto/cipher_static.h>
#include <qqmusic/crypto/cipher_tea.h>
#include <qqmusic/crypto/key_derive.h>
#include <qqmusic/details/result.h>
#include <qqmusic/utils/buffer.h>
#include <utility>
#include <vector>

namespace qqmusic::crypto {

class Decoder {
public:
    explicit Decoder(std::string_view key);

    /**
     * @brief 获取解密后的音频数据
     */
    void decrypt(std::vector<uint8_t>& buf, size_t offset);

    /**
     * @brief 执行完整解密验证流程
     * 1. 搜索并推导解密密钥
     * 2. 初始化解密算法
     * 3. 验证解密有效性（文件头）
     * 4. 准备音频读取器
     */
    bool validate();

    /**
     * @brief 将缓冲区中的数据存取到 buf_in 中
     * 
     * @param buf_ 缓冲区（存放未解密的数据）
     * @return 缓冲区大小和 buf_in
     */
    std::pair<size_t, qqmusic::utils::buffer> read2buf(qqmusic::utils::buffer&& buf_);

private:
    std::vector<uint8_t> key;                ///< 密钥
    size_t audio_len;                        ///< 音频长度
    static size_t songID;                    ///< parse_qtag_meta 中修改
    static size_t rawMetaExtra2;             ///< parse_qtag_meta 中修改
    qqmusic::utils::buffer buf_in{1024};     ///< 输入解密数据,应为一个类的实例
    qqmusic::utils::buffer buf_out{1024};    ///< 输出解密数据
    static std::vector<uint8_t> decoded_key; ///< 新密钥
    size_t current_offset = 0;
    static std::unique_ptr<qqmusic::crypto::Cipher> cipher;

    // 密钥处理系统
    /**
     * @brief 验证环节，不通过则不进行解码
     * 
     * @return qqmusic::Result<std::string> 
     */
    bool search_key();
    bool read_raw_key(size_t len);

    /**
     * @brief 读取QTag格式的元数据
     * 1. 移动到倒数第8字节位置，读取4字节大端序数据作为元数据长度
     * 2. 移动到元数据起始位置，读取元数据内容
     * 3. 将元数据按逗号分割成三部分，分别处理解码密钥 、歌曲ID和额外参数
     */
    static bool parse_qtag_meta(qqmusic::utils::buffer& buf_in);

    static bool read_raw_key(size_t rawKeyLen, const qqmusic::utils::buffer& buf_in);

    static bool parse_cex_meta(qqmusic::utils::buffer& buf_in);

    // 算法路由
    void init_cipher();

    // 文件验证
    void validate_header();
};

} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_QMC_H
