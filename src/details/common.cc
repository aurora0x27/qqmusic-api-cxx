#include <boost/uuid/detail/md5.hpp>
#include <botan/base64.h>
#include <botan/hex.h>
#include <format>
#include <qqmusic/details/common.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <span>

static std::string head(std::span<uint8_t> data);
static std::string tail(std::span<uint8_t> data);
static std::string middle(std::span<uint8_t> data);

std::string qqmusic::details::sign(const nlohmann::json& params) {
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
    for (int i = 0; i < size; i += 2) {
        unsigned char one = zd(data[i]);
        unsigned char two = zd(data[i + 1]);
        unsigned char r = one * 16 ^ two;
        res.push_back(r ^ ol[j++]);
    }
    return Botan::base64_encode(res);
}
