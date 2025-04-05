

# File tripledes.h

[**File List**](files.md) **>** [**details**](dir_6e6556f41d7f85330fbda245682a6a39.md) **>** [**tripledes.h**](tripledes_8h.md)

[Go to the documentation of this file](tripledes_8h.md)


```C++

#ifndef QQMUSIC_DETAILS_TRIPLEDES_H
#define QQMUSIC_DETAILS_TRIPLEDES_H

#include <cstdint>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::details {
enum class tripledes_crypt_mode { encrypt, decrypt };

using tripledes_key_schedule = std::vector<std::vector<std::vector<uint32_t>>>;

// decrypt buf_in and append the result to the end of buf_out
void tripledes_crypt(qqmusic::utils::buffer& buf_in,
                     qqmusic::utils::buffer& buf_out,
                     tripledes_key_schedule key_schedule);

tripledes_key_schedule tripledes_key_setup(const uint8_t* key,
                                           size_t key_size,
                                           tripledes_crypt_mode mode);

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_TRIPLEDES_H
```


