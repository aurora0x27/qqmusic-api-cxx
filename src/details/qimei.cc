#include <botan/asn1_obj.h>
#include <botan/auto_rng.h>
#include <botan/block_cipher.h>
#include <botan/cipher_mode.h>
#include <botan/data_src.h>
#include <botan/pem.h>
#include <botan/pubkey.h>
#include <botan/rsa.h>
#include <botan/secmem.h>
#include <botan/x509_key.h>
#include <qqmusic/details/qimei.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <vector>

const static char PUBLIC_KEY[] = "-----BEGIN PUBLIC KEY-----\n\
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDEIxgwoutfwoJxcGQeedgP7FG9qaIuS0qzf\
R8gWkrkTZKM2iWHn2ajQpBRZjMSoSf6+KJGvar2ORhBfpDXyVtZCKpqLQ+FLkpncClKVIrBwv6\
PHyUvuCb0rIarmgDnzkfQAqVufEtR64iazGDKatvJ9y6B9NMbHddGSAUmRTCrHQIDAQAB\n\
-----END PUBLIC KEY-----";
const static char SECRET[] = "ZdJqM15EeO2zWc08";
const static char APP_KEY[] = "0AND0HD6FE4HY80F";

static qqmusic::result<qqmusic::utils::buffer> rsa_encrypt(qqmusic::utils::buffer& buf);
static qqmusic::result<qqmusic::utils::buffer> aes_encrypt(qqmusic::utils::buffer& key,
                                                           qqmusic::utils::buffer& buf);

qqmusic::result<qqmusic::details::QimeiResult> qqmusic::details::get_qimei(std::string_view version) {
    /*if generate error, return default qimei*/
    return Ok(
        qqmusic::details::QimeiResult{.q16 = "", .q36 = "6c9d3cd110abca9b16311cee10001e717614"});
}

/*rsa encrypt, padding: PKCS1v15*/
static qqmusic::result<qqmusic::utils::buffer> rsa_encrypt(qqmusic::utils::buffer& buf) {
    try {
        /*construct a random number generator*/
        Botan::AutoSeeded_RNG rng;
        /*load public key*/
        auto ds = Botan::DataSource_Memory(PUBLIC_KEY);
        std::unique_ptr<Botan::Public_Key> pub_key(Botan::X509::load_key(ds));

        /*construct an encryptor*/
        Botan::PK_Encryptor_EME enc(*pub_key, rng, "PKCS1v15");

        /*encrypt data*/
        auto res = enc.encrypt(buf.data(), buf.size(), rng);
        return Ok(qqmusic::utils::buffer(res.data(), res.size()));
    } catch (const std::exception& e) {
        /*encrypt failed*/
        return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                             std::format("[rsa_encrypt] -- encrypt failed: {}",
                                                         e.what())));
    }
}

static qqmusic::result<qqmusic::utils::buffer> aes_encrypt(qqmusic::utils::buffer& key,
                                                           qqmusic::utils::buffer& buf) {
    try {
        /*construct a random number generator*/
        Botan::AutoSeeded_RNG rng;

        /*calculate padding size*/
        size_t padding_size = 16 - (buf.size()) % 16;

        /*prepare cipher encryptor*/
        std::unique_ptr<Botan::Cipher_Mode> enc
            = Botan::Cipher_Mode::create("AES-128/CBC", Botan::Cipher_Dir::Encryption);
        if (!enc) {
            return Err(qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                                 "[aes_encrypt] -- cipher mode create failure..."));
        }
        enc->set_key(key.data(), key.size());

        /*insert buffer after iv*/
        qqmusic::utils::buffer res(padding_size, (char) ('\x000' + padding_size));
        res.insert(res.end(), buf.begin(), buf.end());
        enc->start(key);
        enc->finish(res);

        return Ok(res);
    } catch (const std::exception& e) {
        return Err(
            qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                      std::format("[aes_encrypt] -- error ocurred: {}", e.what())));
    }
}
