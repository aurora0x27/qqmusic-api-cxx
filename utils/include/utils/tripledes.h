//-------------utils/tripledes.h.h---------------
//  tripledes decrypt functions for qrc-decoder
#ifndef TRIPLEDES_H
#define TRIPLEDES_H
#include <vector>
#include <stdint.h>

#include <utils/buffer.h>

namespace qqmusic::utils {
    enum class tripledes_crypt_mode {
        encrypt,
        decrypt
    };

    using  tripledes_key_schedule = std::vector<std::vector<std::vector<uint32_t>>>;
    
    // decrypt buf_in and append the result to the end of buf_out
    void tripledes_crypt(qqmusic::utils::buffer*                buf_in,
                         qqmusic::utils::buffer*                buf_out,
                         qqmusic::utils::tripledes_key_schedule key_schedule);

    qqmusic::utils::tripledes_key_schedule tripledes_key_setup(uint8_t*                             key,
                                                               size_t                               key_size,
                                                               qqmusic::utils::tripledes_crypt_mode mode);

}

#endif
