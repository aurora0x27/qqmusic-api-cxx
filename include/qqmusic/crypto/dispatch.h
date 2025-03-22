/**
 * @file dispatch.h
 * @author kingpoem
 * @brief 解码器调度模块
 * @version 0.1
 * @date 2025-03-22
 * 实现多格式解码器的动态注册与匹配机制，支持通过文件后缀自动路由到解码器
 * @copyright Copyright (c) 2025
 *
 */
#ifndef QQMUSIC_CRYPTO_DISPATCH_H
#define QQMUSIC_CRYPTO_DISPATCH_H

#include <algorithm>
#include <functional>
#include <qqmusic/crypto/qmc.h>
#include <string>
#include <vector>

namespace qqmusic::crypto {

class Decoder;

using DecoderFactory = struct {
    std::string suffix;                                                 ///< 目标后缀
    std::function<std::unique_ptr<Decoder>(const std::string&)> create; ///< 工厂函数
};

inline std::vector<DecoderFactory> decoder_registry;                 ///< 全局注册表
inline void registry_decoder(std::stirng suffix, auto create_func) { ///< 注册函数
    decoder_registry.emplace_back(std::move(suffix), create_func);
}

} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_DISPATCH_H
