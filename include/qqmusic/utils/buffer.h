/*-------------------------qqmusic/utils/buffer.h-------------------------------
 * general description of io buffer
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_BUFFER_HPP
#define QQMUSIC_UTILS_BUFFER_HPP
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace qqmusic::utils {

class buffer : public std::vector<uint8_t> {
public:
    buffer() = default;

    buffer(size_t size)
        : std::vector<uint8_t>(size){};

    buffer(const uint8_t* src_head, size_t src_size)
        : std::vector<uint8_t>(src_head, src_head + src_size){};

    buffer(std::vector<uint8_t>&& other) noexcept
        : std::vector<uint8_t>(std::move(other)) {}

    buffer(buffer&& other) noexcept
        : std::vector<uint8_t>(std::move(other)) {}

    buffer(size_t size, char c)
        : std::vector<uint8_t>(size, c) {}

    ~buffer() = default;

    void append(const void* src_data_buf, size_t data_buf_size) {
        this->insert(this->end(), (uint8_t*) src_data_buf, (uint8_t*) src_data_buf + data_buf_size);
    };

    buffer(const buffer& other) = default;

    buffer& operator=(const buffer& other) = default;

    buffer operator+=(const buffer& a) {
        this->insert(this->end(), a.begin(), a.end());
        return *this;
    }

    buffer operator+(const buffer& a) {
        buffer res = *this;
        return res += a;
    }
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_BUFFER_HPP
