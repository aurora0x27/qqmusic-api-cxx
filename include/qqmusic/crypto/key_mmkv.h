/**
 * @file key_mmkv.h
 * @author kingpoem
 * @brief 从macOS平台下的MMKV加密库读取密钥
 * @version 0.1
 * @date 2025-04-02
 *
 * read_key_from_mmkv 主入口：文件路径->密钥
 * read_key_from_custom_mmkv 拓展入口：音乐ID->密钥
 *
 * open_mmkv 自定义库加载器
 *
 * get_relative_mmkv_dir 相对路径
 * get_default_mmkv_dir 系统默认路径
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef QQMUSIC_CRYPTO_KEY_MMKV_H
#define QQMUSIC_CRYPTO_KEY_MMKV_H

#include <botan/secmem.h>
#include <filesystem>
#include <qqmusic/crypto/key_derive.h>
#include <qqmusic/utils/buffer.h>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;
namespace qqmusic::crypto::mmkv {

inline static std::map<std::vector<uint8_t>, std::vector<uint8_t>> stream_key_vault; // 全局密钥存储

inline std::string normalize_unicode(std::string_view str) {
    // 实际实现需接入Unicode规范化库如ICU
    // TODO
    return std::string(str);
}

/**
 * @brief 获取mac平台mmkv库的系统默认路径
 */
inline fs::path get_default_mmkv_dir() {
    auto home = fs::path(std::getenv("HOME"));
    return home
           / "Library/Containers/com.tencent.QQMusicMac/Data/Library/Application "
             "Support/QQMusicMac/mmkv";
}

inline auto derive_key(qqmusic::utils::buffer&& buf) {
    return qqmusic::crypto::KeyDerive::derive(std::move(buf));
}

inline std::vector<uint8_t> read_key_from_mmkv(const fs::path& file) {
#ifndef APPLE
    return {};
#endif

    // 初始化存储库
    if (stream_key_vault.empty()) {
        auto mmkv_dir = get_default_mmkv_dir();
        // 这里需实现MMKV库加载逻辑
        // TODO
    }

    // 文件名匹配逻辑
    auto target = normalize_unicode(file.filename().string());
    auto keys = stream_key_vault; // 伪代码，实际需接入MMKV API

    // 查找匹配逻辑
    // for(const auto& [key, val] : keys) {
    // if(normalize_unicode(key) == target) {
    //     qqmusic::utils::buffer in_buf(val);
    //     return derive_key(std::move(in_buf));
    // }
    // }
    return {};
}

inline void open_mmkv(const fs::path& mmkv_path, std::string_view key) {
    // 需实现MMKV库解密加载
    // stream_key_vault = mmkv::load(mmkv_path, key);
}

} // namespace qqmusic::crypto::mmkv

#endif // !QQMUSIC_CRYPTO_KEY_MMKV_H
