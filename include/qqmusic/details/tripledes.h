/*------------------------qqmusic/details/tripledes.h---------------------------
 * 3des decrypt functions, for qrc-decoder.
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETAILS_TRIPLEDES_H
#define QQMUSIC_DETAILS_TRIPLEDES_H

#include <cstdint>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::utils {
enum class tripledes_crypt_mode { encrypt, decrypt };

using tripledes_key_schedule = std::vector<std::vector<std::vector<uint32_t>>>;

// decrypt buf_in and append the result to the end of buf_out
void tripledes_crypt(qqmusic::utils::buffer& buf_in,
                     qqmusic::utils::buffer& buf_out,
                     qqmusic::utils::tripledes_key_schedule key_schedule);

qqmusic::utils::tripledes_key_schedule tripledes_key_setup(
    const uint8_t* key, size_t key_size, qqmusic::utils::tripledes_crypt_mode mode);

} // namespace qqmusic::utils

#endif // !QQMUSIC_DETAILS_TRIPLEDES_H
