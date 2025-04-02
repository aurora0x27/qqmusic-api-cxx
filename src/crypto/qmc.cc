#include <format>
#include <qqmusic/crypto/cipher_map.h>
#include <qqmusic/crypto/cipher_rc4.h>
#include <qqmusic/crypto/cipher_static.h>
#include <qqmusic/crypto/cipher_tea.h>
#include <qqmusic/crypto/key_derive.h>
#include <qqmusic/crypto/qmc.h>
#include <qqmusic/details/result.h>
#include <qqmusic/utils/buffer.h>
#include <utility>

size_t qqmusic::crypto::Decoder::songID = 0;
size_t qqmusic::crypto::Decoder::rawMetaExtra2 = 0;
std::vector<uint8_t> qqmusic::crypto::Decoder::decoded_key;

namespace qqmusic::crypto {

std::pair<size_t, qqmusic::utils::buffer> read2buf(qqmusic::utils::buffer&& buf_) {
    qqmusic::utils::buffer buf_in(std::move(buf_));
    return {buf_in.size(), std::move(buf_in)};
}

bool Decoder::search_key() {
    // 获取文件大小,判断文件是否正常
    if (buf_in.size() < 4) {
        std::cout << std::format("[Decoder::search_key] buf_in size {} < 4", buf_in.size())
                  << std::endl;
        return false;
    }

    // macOS 特殊处理
    // TODO
    // if (isDarwin && !extensionStartsWithQmc) {
    //     auto keyResult = readKeyFromMMKV(filepath);
    //     if (keyResult) {
    //         audio_len = static_cast<int>(buf_in.size());
    //         decoded_key = std::move(*keyResult);
    //         return true;
    //     }
    // }

    // 读取文件尾标识
    const uint8_t* suffix_buf = buf_in.data() + buf_in.size() - 4;
    const std::string_view suffix(reinterpret_cast<const char*>(suffix_buf), 4);

    // 处理不同类型后缀
    if (suffix == "QTag") {
        return parse_qtag_meta(buf_in);
    } else if (suffix == "STag") {
        std::cout << std::format("suffix is {} which is not supported", suffix) << std::endl;
        return false;
    } else if (suffix == "cex\x00") {
        // TODO
        // const auto footer = parse_cex_meta(buf_in);
        // audio_len = static_cast<int>(buf_in.size() - footer.tagSize);
        // decoded_key = readKeyFromMMKVCustom(footer.mediaFilename);
        return !decoded_key.empty();
    }

    // 处理密钥长度
    const auto key_len = *reinterpret_cast<const uint32_t*>(suffix_buf);
    if (key_len <= 0xFFFF) {
        return read_raw_key(key_len, buf_in);
    }

    // 使用默认静态密码
    audio_len = buf_in.size();
    return true;
}

/**
 * meta_len 从缓冲区末尾-8位置读取的元数据字段长度
 * meta_start 元数据在缓冲区中的起始位置
 * key_part 密钥种子字符串（第一个逗号前的内容）
 * song_id 中间段的数字ID字符串
 * extra 最后的拓展参数字符串
 *
 * decoded_key 最终密钥
 * songID 转换后的整型歌曲ID
 * rawMetaExtra2 转换后的整型拓展参数
 */
bool Decoder::parse_qtag_meta(qqmusic::utils::buffer& buf_in) {
    size_t len = buf_in.size();
    if (len < 8)
        return false;

    // 大端序解析元数据长度
    const uint32_t meta_len = [&] {
        const uint8_t* p = buf_in.data() + len - 8;
        return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
    }();

    // 验证元数据区域有效性
    const size_t meta_start = len - 8 - meta_len;
    if (meta_len == 0 || meta_start >= len)
        return false;

    // 分割元数据字段
    std::string_view meta(reinterpret_cast<const char*>(buf_in.data() + meta_start), meta_len);
    const auto [key_part, remain] = [&] {
        size_t pos = meta.find(',');
        return (pos != std::string_view::npos)
                   ? std::pair(meta.substr(0, pos), meta.substr(pos + 1))
                   : std::pair(std::string_view{}, std::string_view{});
    }();

    // 解析ID部分
    const auto [song_id, extra] = [&] {
        size_t pos = remain.find(',');
        return (pos != std::string_view::npos)
                   ? std::pair(remain.substr(0, pos), remain.substr(pos + 1))
                   : std::pair(std::string_view{}, std::string_view{});
    }();

    // 数据有效性检查
    if (key_part.empty() || song_id.empty() || extra.empty())
        return false;

    // 赋值给Decoder成员变量
    auto key_part_ = qqmusic::utils::buffer(reinterpret_cast<const uint8_t*>(key_part.data()),
                                            key_part.size());
    auto decoded_key = qqmusic::crypto::KeyDerive::derive(std::move(key_part_));

    songID = std::stoi(std::string(song_id));
    rawMetaExtra2 = std::stoi(std::string(extra));

    return true;
}

bool Decoder::read_raw_key(size_t rawKeyLen, const qqmusic::utils::buffer& buf_in) {
    const auto processKeyData = [](const uint8_t* data, size_t len) -> bool {
        // 清理尾部零值
        auto cleanData = std::vector<uint8_t>(data, data + len);
        cleanData.erase(std::find_if(cleanData.rbegin(),
                                     cleanData.rend(),
                                     [](uint8_t c) { return c != 0; })
                            .base(),
                        cleanData.end());

        if (cleanData.empty())
            return false;

        // 密钥派生
        qqmusic::utils::buffer keyMaterial(std::move(cleanData));
        auto derivedKey = qqmusic::crypto::KeyDerive::derive(std::move(keyMaterial));
        if (derivedKey.empty())
            return false;

        decoded_key = std::move(derivedKey);
        return true;
    };

    // 验证缓冲区有效性
    const size_t totalLen = buf_in.size();
    const size_t keyDataStart = totalLen - (4 + rawKeyLen);
    if (rawKeyLen <= 0 || keyDataStart >= totalLen)
        return false;

    // 提取密钥数据
    const uint8_t* keyData = buf_in.data() + keyDataStart;
    return processKeyData(keyData, static_cast<size_t>(rawKeyLen));
}

bool Decoder::validate() {
    if (!search_key())
        return false;

    // 初始化解密算法
    // if (decoded_key.size() > 300) {
    //     cipher = std::make_unique<RC4Cipher>();
    // } else if (!decoded_key.empty()) {
    //     cipher = std::make_unique<MapCipher>();
    // } else {
    //     cipher = std::make_unique<StaticCipher>();
    // }
    //
    if (buf_in.size() < 64)
        return false;

    // auto header = {buf_in.data() + offset, length};
    // cipher->decrypt(header, 0);

    // TODO
    // if(!sniff_audio_header(header)) {
    //     return false;
    // }

    current_offset = 0;
    return true;
}

} // namespace qqmusic::crypto
