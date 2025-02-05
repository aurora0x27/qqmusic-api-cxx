#include <botan/base64.h>
#include <botan/cipher_mode.h>
#include <botan/auto_rng.h>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <qqmusic/details/device.h>
#include <qqmusic/details/qimei.h>
#include <qqmusic/utils/cache.h>

// static qqmusic::result<qqmusic::utils::buffer> aes_encrypt(qqmusic::utils::buffer& key,
//                                                            qqmusic::utils::buffer& buf) {
//     try {
//         /*construct a random number generator*/
//         Botan::AutoSeeded_RNG rng;
// 
//         /*calculate padding size*/
//         size_t padding_size = 16 - (buf.size()) % 16;
// 
//         /*prepare cipher encryptor*/
//         std::unique_ptr<Botan::Cipher_Mode> enc
//             = Botan::Cipher_Mode::create("AES-128/CBC", Botan::Cipher_Dir::Encryption);
//         if (!enc) {
//             return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
//                                                  "[aes_encrypt] -- cipher mode create failure..."));
//         }
//         enc->set_key(key.data(), key.size());
// 
//         /*insert buffer after iv*/
//         qqmusic::utils::buffer padding(padding_size, (char)('\0' + padding_size));
//         buf.insert(buf.end(), padding.begin(), padding.end());
//         enc->start(key);
//         enc->update(buf);
//         enc->finish(key);
// 
//         return Ok(buf);
//     } catch (const std::exception& e) {
//         return Err(
//             qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
//                                       std::format("[aes_encrypt] -- error ocurred: {}", e.what())));
//     }
// }

int main(int argc, char** argv) {
    auto d = qqmusic::details::get_device_info();
    if (d.isErr()) {
        std::cout << "Can't get device, reason: " << d.unwrapErr().what() << std::endl;
        return 1;
    }
    auto device = d.unwrap();
    auto res = qqmusic::details::get_qimei(device, "13.2.5.8");
    if (res.isErr()) {
        std::cout << res.unwrapErr().what() << std::endl;
    } else {
        std::cout << res.unwrap().q16 << std::endl;
        std::cout << res.unwrap().q36 << std::endl;
    }
    
    return 0;
}
