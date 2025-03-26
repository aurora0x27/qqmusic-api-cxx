/**------------------------------------qqmusic/utils/buffer.h---------------------------------------
 *
 * @file qqmusic/utils/buffer.h
 *
 * @brief 二进制缓冲区类
 *
 * @date 2025-3-21
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_BUFFER_HPP
#define QQMUSIC_UTILS_BUFFER_HPP
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace qqmusic::utils {

/**
 * @brief 二进制缓冲区的统一表示
 * */
class buffer : public std::vector<uint8_t> {
public:

    /**
     * @brief 默认初始化
     * */
    buffer() = default;

    /**
     * @brief 预先分配异丁醇长度
     *
     * @param size 预先分配长度
     * */
    buffer(size_t size)
        : std::vector<uint8_t>(size){};

    /**
     * @brief 直接从内存区域进行构造
     *
     * @param src_head 内存区域的首地址
     * @param src_size 内存区域长度
     *
     * @note 是拷贝初始化
     * */
    buffer(const uint8_t* src_head, size_t src_size)
        : std::vector<uint8_t>(src_head, src_head + src_size){};

    /**
     * @brief 从std::vector<uint8_t>移动构造
     *
     * @param other
     * */
    buffer(std::vector<uint8_t>&& other) noexcept
        : std::vector<uint8_t>(std::move(other)) {}

    /**
     * @brief 移动构造
     *
     * @param other
     * */
    buffer(buffer&& other) noexcept
        : std::vector<uint8_t>(std::move(other)) {}

    /**
     * @brief 初始化为特定字符
     *
     * @param size 预先初始化长度
     * @param c    需要填充的字符
     * */
    buffer(size_t size, char c)
        : std::vector<uint8_t>(size, c) {}

    ~buffer() = default;

    /**
     * @brief 将一段内存插到这段buffer后面
     *
     * @param src_data_buf 待插入数据的首地址
     * @param data_buf_size 待插入数据的长度
     * */
    void append(const void* src_data_buf, size_t data_buf_size) {
        this->insert(this->end(), (uint8_t*) src_data_buf, (uint8_t*) src_data_buf + data_buf_size);
    };

    /**
     * @brief 拷贝构造函数
     *
     * @param other
     * */
    buffer(const buffer& other) = default;

    /**
     * @brief 拷贝构造函数, 重载`=`
     *
     * @param other
     * */
    buffer& operator=(const buffer& other) = default;

    /**
     * @brief 将一段buffer插入本段buffer末尾
     *
     * @param a
     * */
    buffer operator+=(const buffer& a) {
        this->insert(this->end(), a.begin(), a.end());
        return *this;
    }

    /**
     * @brief 两段buffer拼接, 并返回构造新的buffer
     *
     * @param a
     * */
    buffer operator+(const buffer& a) {
        buffer res = *this;
        return res += a;
    }
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_BUFFER_HPP
