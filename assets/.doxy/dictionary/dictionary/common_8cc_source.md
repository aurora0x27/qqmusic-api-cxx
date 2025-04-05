

# File common.cc

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**utils**](dir_313caf1132e152dd9b58bea13a4052ca.md) **>** [**common.cc**](common_8cc.md)

[Go to the documentation of this file](common_8cc.md)


```C++
#include <boost/asio/buffer.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <botan/auto_rng.h>
#include <botan/base64.h>
#include <botan/hex.h>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <qqmusic/details/tripledes.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/common.h>
#include <span>
#include <string>
#include <zlib.h>

namespace qqmusic::utils {

buffer resp2buf(http::response<http::dynamic_body>&& resp) {
    /* TODO: I haven't found a better way to convert http::request to normal buffer*/
    auto* ptr = static_cast<const uint8_t*>((*resp.body().data().begin()).data());
    auto size = resp.body().data().buffer_bytes();
    return qqmusic::utils::buffer{ptr, size};
}

buffer hex2buf(std::string_view hex) {
    return buffer{Botan::hex_decode(hex)};
}

static std::string head(std::span<uint8_t> data);
static std::string tail(std::span<uint8_t> data);
static std::string middle(std::span<uint8_t> data);

std::string sign(const nlohmann::json& params) {
    boost::uuids::detail::md5 hash;
    boost::uuids::detail::md5::digest_type digest;
    auto str = params.dump();
    hash.process_bytes(str.data(), str.size());
    hash.get_digest(digest);
    auto md5_str = Botan::hex_encode(digest);
    auto h = head(std::span<uint8_t>((uint8_t*) md5_str.data(), md5_str.size()));
    auto e = tail(std::span<uint8_t>((uint8_t*) md5_str.data(), md5_str.size()));
    auto m = middle(std::span<uint8_t>((uint8_t*) md5_str.data(), md5_str.size()));
    std::string res = "zzb" + h + m + e;
    /*iterate the string and do some format jobs
     *
     * origin python code:
     * ```
     * return res.lower().replace("/", "").replace("+", "").replace("=", "")
     * ```
     * */
    for (auto itr = res.begin(); itr != res.end(); /*left blank*/) {
        switch (*itr) {
        case '+':
        case '/':
        case '=':
            /*delete char '+', '=', '/'*/
            itr = res.erase(itr);
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            /*lower case the string*/
            *itr -= ('A' - 'a');
        default:
            itr++;
        }
    }
    return res;
}

static std::string head(std::span<uint8_t> data) {
    const int p[] = {21, 4, 9, 26, 16, 20, 27, 30};
    return {
        (char) data[p[0]],
        (char) data[p[1]],
        (char) data[p[2]],
        (char) data[p[3]],
        (char) data[p[4]],
        (char) data[p[5]],
        (char) data[p[6]],
        (char) data[p[7]],
    };
}

static std::string tail(std::span<uint8_t> data) {
    const int p[] = {18, 11, 3, 2, 1, 7, 6, 25};
    return {
        (char) data[p[0]],
        (char) data[p[1]],
        (char) data[p[2]],
        (char) data[p[3]],
        (char) data[p[4]],
        (char) data[p[5]],
        (char) data[p[6]],
        (char) data[p[7]],
    };
}

static std::string middle(std::span<uint8_t> data) {
    auto zd = [](unsigned char ch) -> unsigned char {
        switch (ch) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return ch - '0';
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return ch - 'A' + 10;
        default:
            throw std::runtime_error("Assert data is hex string failed");
        }
    };
    const unsigned char ol[]
        = {212, 45, 80, 68, 195, 163, 163, 203, 157, 220, 254, 91, 204, 79, 104, 6};
    size_t size = data.size();
    std::vector<unsigned char> res;
    unsigned int j = 0;
    for (size_t i = 0; i < size; i += 2) {
        unsigned char one = zd(data[i]);
        unsigned char two = zd(data[i + 1]);
        unsigned char r = one * 16 ^ two;
        res.push_back(r ^ ol[j++]);
    }
    return Botan::base64_encode(res);
}

static int decompress(const buffer& src, buffer& dest);

// qmc decoder
static void qmc1_decrypt(buffer& src);

qqmusic::Result<std::string> qrc_decode(const buffer& src, qrc_type type) {
    buffer tmp;
    if (type == qrc_type::local) {
        buffer raw(src);
        qmc1_decrypt(raw);
        tmp.assign(raw.begin() + 11, raw.end());
    } else {
        tmp = src;
    }

    size_t tmp_size = tmp.size();
    // check if size is integer multiple of 8 bytes
    if (tmp_size % 8 != 0) {
        return Err(Exception(Exception::Kind::DataDestroy,
                             "qrc-decoder: buffer size cannot devided by 8"));
    }

    // QRC_KEY = b"!@#)(*$%123ZXC!@!@#)(NHL"
    constexpr uint8_t qrc_key[] = "\x21\x40\x23\x29\x28\x2A\x24\x25\x31\x32\x33\x5A\x58\x43\x21\x40"
                                  "\x21\x40\x23\x29\x28\x4E\x48\x4C";

    constexpr size_t qrc_key_size = sizeof(qrc_key) - 1;

    // generate key schedule
    const auto schedule = tripledes_key_setup(qrc_key,
                                              qrc_key_size,
                                              details::tripledes_crypt_mode::decrypt);

    buffer compressed_buffer;

    // iterate encrypted_text_byte function in units of 8 bytes
    // origin python code:
    // for i in range(0, len(encrypted_text_byte), 8):
    //     data += tripledes_crypt(encrypted_text_byte[i:], schedule)
    uint8_t* head = tmp.data();
    for (size_t i = 0; i < tmp_size / 8; ++i) {
        buffer tmp_section(head + i * 8, 8);
        details::tripledes_crypt(tmp_section, compressed_buffer, schedule);
    }

    // decompress the buffer
    buffer dest;
    int decompress_res = decompress(compressed_buffer, dest);

    switch (decompress_res) {
    case -1:
    case 1:
        return Err(Exception(Exception::Kind::RuntimeError, "qrc-decoder: memory alloc error"));
    case 2:
        return Err(Exception(Exception::Kind::DataDestroy,
                             "qrc-decoder: data destroy when decompressing decoding lyric"));
    case 0:
        break;
    default:
        return Err(Exception(Exception::Kind::UnknownError,
                             "qrc-decoder: unknown error when decompressing decoded lyric"));
    }

    return Ok(std::string((char*) dest.data(), dest.size()));
}

static int decompress(const buffer& src, buffer& dest) {
    // prepare receive buffer
    size_t tmp_dest_size = src.size() * 4;
    auto tmp_dest_head = (uint8_t*) malloc(tmp_dest_size);

    // prepare input buffer
    size_t src_size = src.size();
    const uint8_t* src_head = src.data();

    // ZEXTERN int ZEXPORT uncompress(Bytef *dest,   uLongf *destLen,
    //                                const Bytef *source, uLong sourceLen);
#ifdef PLATFORM_WINDOWS
    int uncompress_res = uncompress(tmp_dest_head, (uLongf*) &tmp_dest_size, src_head, src_size);
#else
    int uncompress_res = uncompress(tmp_dest_head, &tmp_dest_size, src_head, src_size);
#endif // PLATFORM_WINDOWS
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
    dest.append(tmp_dest_head, tmp_dest_size);

    free(tmp_dest_head);
    return 0;
}

// qmc decryptor
// def qmc1_decrypt(data: bytearray) -> None:
//     for i, _value in enumerate(data):
//         data[i] ^= PRIVKEY[(i % 0x7FFF) & 0x7F] if i > 0x7FFF else PRIVKEY[i & 0x7F]
static void qmc1_decrypt(buffer& src) {
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

    size_t size = src.size();
    uint8_t* head = src.data();

    for (size_t i = 0; i < size; ++i) {
        head[i] ^= i > 0x7fff ? private_key[(i % 0x7fff) & 0x7f] : private_key[i & 0x7f];
    }
}

uint64_t hash33(std::string_view str, uint64_t prev) {
    /* python code:
     * ```
     * def hash33(s: str, h: int = 0) -> int:
     * for c in s:
     *     h = (h << 5) + h + ord(c)
     * return 2147483647 & h
     * ```
     * */
    uint64_t h = prev;
    size_t i = 0;

    /* Get single characters in utf-8 string*/
    while (i < str.length()) {
        uint32_t codepoint = 0;

        if ((str[i] & 0x80) == 0) {
            // 1 byte character (ASCII)
            codepoint = static_cast<unsigned char>(str[i]);
            i += 1;
        } else if ((str[i] & 0xE0) == 0xC0) {
            // 2 byte character
            codepoint = (str[i] & 0x1F) << 6 | (str[i + 1] & 0x3F);
            i += 2;
        } else if ((str[i] & 0xF0) == 0xE0) {
            // 3 byte character
            codepoint = (str[i] & 0x0F) << 12 | (str[i + 1] & 0x3F) << 6 | (str[i + 2] & 0x3F);
            i += 3;
        } else if ((str[i] & 0xF8) == 0xF0) {
            // 4 byte character
            codepoint = (str[i] & 0x07) << 18 | (str[i + 1] & 0x3F) << 12 | (str[i + 2] & 0x3F) << 6
                        | (str[i + 3] & 0x3F);
            i += 4;
        }

        h = (h << 5) + h + codepoint;
    }

    return 2147483647 & h;
}

std::string get_search_id() {
    Botan::AutoSeeded_RNG rng;

    /*random unsigned long long generator*/
    auto randull = [&rng]() -> uint64_t {
        uint64_t res = 0;
        std::vector<uint8_t> buffer(sizeof(res));
        rng.randomize(buffer);
        memcpy(&res, buffer.data(), sizeof(res));
        return res;
    };

    auto e = randull() % 20;
    auto t = e * 18014398509481984;
    auto n = (randull() % 4194304) * 4294967296;

#ifdef PLATFORM_APPLE
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto duration = now.time_since_epoch() % 1000;
    auto millis = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration)
                      .count();
    std::tm local_tm{};
    localtime_r(&now_time_t, &local_tm);
    auto seconds = mktime(&local_tm);
    double ts = static_cast<double>(seconds) * 1000.0 + millis;
#else
    std::chrono::time_point<std::chrono::system_clock> tp{std::chrono::system_clock::now()};
    auto zoned_time = std::chrono::zoned_time{std::chrono::current_zone(), tp};
    double ts = std::chrono::duration<double, std::milli>(
                    zoned_time.get_local_time().time_since_epoch())
                    .count();
#endif

    uint64_t a = lround(ts);
    auto r = a % (24 * 60 * 60 * 1000);

    return std::to_string(t + n + r);
}

} // namespace qqmusic::utils
```


