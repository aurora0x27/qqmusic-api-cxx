#include "qqmusic/error.h"
#include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <zlib.h>

#include <utils/tripledes.h>
#include <utils/qrc_decoder.h>
#include <utils/buffer.h>

static int decompress(qqmusic::utils::buffer* src,
                      qqmusic::utils::buffer* dest);

// qmc decoder
static void qmc1_decrypt(qqmusic::utils::buffer* src);

qqmusic::result 
qqmusic::utils::qrc_decode(qqmusic::utils::buffer*  src,
                           qqmusic::utils::buffer*  dest,
                           qqmusic::utils::qrc_type type)
{
    // copy a instance of src
    qqmusic::utils::buffer* tmp = NULL;
    if (type == qqmusic::utils::qrc_type::local) {
        // local qrc needs qmc decrypt and delete 11 bytes in the beginning
        qqmusic::utils::buffer pre_processed_buf(src->get_head(), src->get_size());
        qmc1_decrypt(&pre_processed_buf);
        try {
            tmp = new qqmusic::utils::buffer(pre_processed_buf.get_head() + 11, pre_processed_buf.get_size() - 11);
        } catch (std::bad_alloc& e) {
            qqmusic::result res(qqmusic::state::error, "memory alloc error when decoding lyrics");
            return res;
        }
    } else if (type == qqmusic::utils::qrc_type::cloud) {
        try {
            tmp = new qqmusic::utils::buffer(src->get_head(), src->get_size());
        } catch (std::bad_alloc& e) {
            qqmusic::result res(qqmusic::state::error, "memory alloc error when decoding lyrics");
            return res;
        }
    }

    size_t tmp_size = tmp->get_size();
    // check if size is integer multiple of 8 bytes
    if (tmp_size % 8 != 0) {
        qqmusic::result res(qqmusic::state::error, "data destroy after pre-process lyrics");
        return res;
    }

    // QRC_KEY = b"!@#)(*$%123ZXC!@!@#)(NHL"
    const     uint8_t qrc_key[] =
    "\x21\x40\x23\x29\x28\x2A\x24\x25\x31\x32\x33\x5A\x58\x43\x21\x40\x21\x40\x23\x29\x28\x4E\x48\x4C";

    constexpr size_t  qrc_key_size = sizeof(qrc_key) - 1;

    // generate key schedule
    qqmusic::utils::tripledes_key_schedule schedule = qqmusic::utils::tripledes_key_setup((uint8_t*)qrc_key,
                                                                                           qrc_key_size,
                                                                                           qqmusic::utils::tripledes_crypt_mode::decrypt);

    qqmusic::utils::buffer compressed_buffer;

    // iterate encrypted_text_byte function in units of 8 bytes
    // origin python code:
    // for i in range(0, len(encrypted_text_byte), 8):
    //     data += tripledes_crypt(encrypted_text_byte[i:], schedule)
    uint8_t* head = tmp->get_head();
    for (size_t i = 0; i < tmp_size / 8; ++i) {
        qqmusic::utils::buffer tmp_section(head + i * 8, 8);
        qqmusic::utils::tripledes_crypt(&tmp_section, &compressed_buffer, schedule);
    }
    delete tmp;

    // decompress the buffer
    int decompress_res = decompress(&compressed_buffer, dest);

    switch (decompress_res) {
        case -1:
        case 1:
            return qqmusic::result(qqmusic::state::error, "memory alloc error when decompressing data");
        case 2:
            return qqmusic::result(qqmusic::state::error, "data destroy when decompressing data");
        case 0:
            break;
        default:
            return qqmusic::result(qqmusic::state::error, "unknown error occurred when decompressing data");
    }

    return qqmusic::result(qqmusic::state::ok, "lyric decode ok");
}

static int
decompress(qqmusic::utils::buffer* src,
           qqmusic::utils::buffer* dest)
{
    // prepare receive buffer
    size_t   tmp_dest_size = src->get_size() * 4;
    uint8_t* tmp_dest_head = (uint8_t*)malloc(tmp_dest_size);

    // prepare input buffer
    size_t   src_size = src->get_size();
    uint8_t* src_head = src->get_head();

    // ZEXTERN int ZEXPORT uncompress(Bytef *dest,   uLongf *destLen,
    //                                const Bytef *source, uLong sourceLen);
    int uncompress_res = uncompress(tmp_dest_head, &tmp_dest_size, src_head, src_size);
    switch (uncompress_res) {
    case Z_OK:
        break;
    case Z_MEM_ERROR:
        free(tmp_dest_head);
        return -1; // mem alloc error
        break;
    case Z_BUF_ERROR:
        free(tmp_dest_head);
        return 1; // buffer too small
        break;
    case Z_DATA_ERROR:
        free(tmp_dest_head);
        return 2; // data demage
        break;
    default:
        free(tmp_dest_head);
        return 3; // unknown error
    }

    // write buffer into dest
    dest->append(tmp_dest_head, tmp_dest_size);

    free(tmp_dest_head);
    return 0;
}

// qmc decoder
// def qmc1_decrypt(data: bytearray) -> None:
//     for i, _value in enumerate(data):
//         data[i] ^= PRIVKEY[(i % 0x7FFF) & 0x7F] if i > 0x7FFF else PRIVKEY[i & 0x7F]
static void
qmc1_decrypt(qqmusic::utils::buffer* src)
{
    uint8_t private_key[128] = {
	    0xc3, 0x4a, 0xd6, 0xca, 0x90, 0x67, 0xf7, 0x52,
	    0xd8, 0xa1, 0x66, 0x62, 0x9f, 0x5b, 0x09, 0x00,

	    0xc3, 0x5e, 0x95, 0x23, 0x9f, 0x13, 0x11, 0x7e,
	    0xd8, 0x92, 0x3f, 0xbc, 0x90, 0xbb, 0x74, 0x0e,

	    0xc3, 0x47, 0x74, 0x3d, 0x90, 0xaa, 0x3f, 0x51,
	    0xd8, 0xf4, 0x11, 0x84, 0x9f, 0xde, 0x95, 0x1d,

	    0xc3, 0xc6, 0x09, 0xd5, 0x9f, 0xfa, 0x66, 0xf9,
	    0xd8, 0xf0, 0xf7, 0xa0, 0x90, 0xa1, 0xd6, 0xf3,

	    0xc3, 0xf3, 0xd6, 0xa1, 0x90, 0xa0, 0xf7, 0xf0,
	    0xd8, 0xf9, 0x66, 0xfa, 0x9f, 0xd5, 0x09, 0xc6,

	    0xc3, 0x1d, 0x95, 0xde, 0x9f, 0x84, 0x11, 0xf4,
	    0xd8, 0x51, 0x3f, 0xaa, 0x90, 0x3d, 0x74, 0x47,

	    0xc3, 0x0e, 0x74, 0xbb, 0x90, 0xbc, 0x3f, 0x92,
	    0xd8, 0x7e, 0x11, 0x13, 0x9f, 0x23, 0x95, 0x5e,

	    0xc3, 0x00, 0x09, 0x5b, 0x9f, 0x62, 0x66, 0xa1,
	    0xd8, 0x52, 0xf7, 0x67, 0x90, 0xca, 0xd6, 0x4a,
    };

    size_t size = src->get_size();
    uint8_t* head = src->get_head();

    for (size_t i = 0; i < size; ++i) {
        head[i] ^= i > 0x7fff ? private_key[(i % 0x7fff) & 0x7f] : private_key[i & 0x7f];
    }
}
