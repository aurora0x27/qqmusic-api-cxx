#include <cstdint>
#include <stdint.h>
#include <string>
#include <string.h>

#include <zlib.h>

#include <utils/tripledes.h>
#include <utils/qrcdecoder.h>
#include <utils/buffer.h>

static int decompress(qqmusic::utils::buffer* src,
                      qqmusic::utils::buffer* dest);

// I don't know how to "decode('utf-8')", do not use this function
static int encoding_utf8(qqmusic::utils::buffer* src,
                         qqmusic::utils::buffer* dest);

static void builtin_memcpy(void* dest, void* src, size_t size);

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
            return qqmusic::result::mem_alloc_error;
        }
    } else if (type == qqmusic::utils::qrc_type::cloud) {
        try {
            tmp = new qqmusic::utils::buffer(src->get_head(), src->get_size());
        } catch (std::bad_alloc& e) {
            return qqmusic::result::mem_alloc_error;
        }
    }

    size_t tmp_size = tmp->get_size();
    // check if size is integer multiple of 8 bytes
    if (tmp_size % 8 != 0) {
        return qqmusic::result::data_destroy;
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
    for (size_t i = 0; i < tmp_size / 8; i += 8) {
        qqmusic::utils::buffer tmp_section(tmp->get_head() + i * 8, 8);
        qqmusic::utils::tripledes_crypt(&tmp_section, &compressed_buffer, schedule);
    }

    // decompress the buffer
    int decompress_res = decompress(&compressed_buffer, dest);

    switch (decompress_res) {
    case -1:
    case 1:
        delete tmp;
        return qqmusic::result::mem_alloc_error;
    case 2:
        delete tmp;
        return qqmusic::result::data_destroy;
    case 0:
        break;
    default:
        delete tmp;
        return qqmusic::result::unknown_error;
    }

    delete tmp;
    return qqmusic::result::excecuted_success;
}

static void
builtin_memcpy(void* dest, void* src, size_t size)
{
    uint8_t* d = (uint8_t*)dest;
    uint8_t* s = (uint8_t*)src;
    for (size_t i = 0; i < size; ++i) {
        d[i] = s[i];
    }
}

static int
encoding_utf8(qqmusic::utils::buffer* src,
              qqmusic::utils::buffer* dest)
{
    auto chunk = std::string((char*)src->get_head(), (int)src->get_size());
    if (chunk.size() != dest->get_size()) {
        dest->resize(chunk.size());
    }
    builtin_memcpy((void*)dest->get_head(), (void*)chunk.c_str(), chunk.size());
    return 0;
}

static int
decompress(qqmusic::utils::buffer* src,
           qqmusic::utils::buffer* dest)
{
    // resize the receive buffer as 4 times larger than src
    size_t dest_size = src->get_size() * 4;
    uint8_t* dest_head = dest->get_head();
    bool resize_res = dest->resize(dest_size);
    if (resize_res) {
        return -1;
    }

    size_t src_size = src->get_size();
    uint8_t* src_head = src->get_head();

    // ZEXTERN int ZEXPORT uncompress(Bytef *dest,   uLongf *destLen,
    //                                const Bytef *source, uLong sourceLen);
    int uncompress_res = uncompress(dest_head, &dest_size, src_head, src_size);
    switch (uncompress_res) {
    case Z_OK:
        break;
    case Z_MEM_ERROR:
        return -1; // mem alloc error
        break;
    case Z_BUF_ERROR:
        return 1; // buffer too small
        break;
    case Z_DATA_ERROR:
        return 2; // data demage
        break;
    }

    dest->resize(dest_size);

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
        head[i] ^= i > 0x7FFF ? private_key[(i % 0x7FFF) & 0x7F] : private_key[i & 0x7FF];
    }
}
