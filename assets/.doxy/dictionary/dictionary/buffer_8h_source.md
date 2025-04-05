

# File buffer.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**utils**](dir_478616d8952f43e793f28d8ded6e3463.md) **>** [**buffer.h**](buffer_8h.md)

[Go to the documentation of this file](buffer_8h.md)


```C++

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
```


