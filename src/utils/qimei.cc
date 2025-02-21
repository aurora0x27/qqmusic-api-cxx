#include <boost/beast.hpp>
#include <boost/beast/http/impl/read.hpp>
#include <boost/beast/http/impl/verb.ipp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/url.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <botan/asn1_obj.h>
#include <botan/auto_rng.h>
#include <botan/base64.h>
#include <botan/block_cipher.h>
#include <botan/cipher_mode.h>
#include <botan/data_src.h>
#include <botan/hex.h>
#include <botan/pem.h>
#include <botan/pubkey.h>
#include <botan/rsa.h>
#include <botan/secmem.h>
#include <botan/x509_key.h>
#include <chrono>
#include <ctime>
#include <format>
#include <nlohmann/json.hpp>
#include <qqmusic/result.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/device.h>
#include <qqmusic/utils/qimei.h>
#include <set>
#include <string>
#include <vector>

const static char PUBLIC_KEY[] = "-----BEGIN PUBLIC KEY-----\n\
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDEIxgwoutfwoJxcGQeedgP7FG9qaIuS0qzf\
R8gWkrkTZKM2iWHn2ajQpBRZjMSoSf6+KJGvar2ORhBfpDXyVtZCKpqLQ+FLkpncClKVIrBwv6\
PHyUvuCb0rIarmgDnzkfQAqVufEtR64iazGDKatvJ9y6B9NMbHddGSAUmRTCrHQIDAQAB\n\
-----END PUBLIC KEY-----";
const static char SECRET[] = "ZdJqM15EeO2zWc08";
const static char APP_KEY[] = "0AND0HD6FE4HY80F";

static qqmusic::Result<qqmusic::utils::buffer> rsa_encrypt(qqmusic::utils::buffer& buf);
static qqmusic::Result<qqmusic::utils::buffer> aes_encrypt(qqmusic::utils::buffer& key,
                                                           qqmusic::utils::buffer& buf);
static std::string random_beacon_id();
/*load random payload by device*/
static nlohmann::json load_rand_payload(qqmusic::utils::Device& device, std::string_view version);

qqmusic::Result<qqmusic::utils::QimeiResult> qqmusic::utils::get_qimei(
    qqmusic::utils::Device& device, std::string_view version) {
    using namespace boost::beast;

    /*if generate error, return default qimei*/
    const char QIMEI_ENTRY[] = "https://api.tencentmusic.com/tme/trpc/proxy";
    Botan::AutoSeeded_RNG rng;

    try {
        auto gen_hex_strings = [&rng](int len) {
            const char table[] = "abcdef1234567890";
            uint64_t randi = 0;
            std::vector<uint8_t> buffer(sizeof(randi));
            rng.randomize(buffer);
            memcpy(&randi, buffer.data(), sizeof(randi));
            uint64_t idx = randi % 16;
            std::vector<char> res(len);
            for (auto& i : res) {
                rng.randomize(buffer);
                memcpy(&randi, buffer.data(), sizeof(randi));
                i = table[randi % 16];
            }
            return std::string(res.data(), res.size());
        };

        auto crypt_key = gen_hex_strings(16);
        auto nonce = gen_hex_strings(16);

        std::string key;
        std::string params;

        qqmusic::utils::buffer crypt_key_buf((uint8_t*) crypt_key.data(), crypt_key.size());
        auto rsa_res = rsa_encrypt(crypt_key_buf);
        if (rsa_res.isErr()) {
            return Err(
                qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                          std::format("[get_qimei] -- failed to get qimei: {}",
                                                      rsa_res.unwrapErr().what())));
        } else {
            key = Botan::base64_encode(rsa_res.unwrap());
        }

        auto payload = nlohmann::to_string(load_rand_payload(device, version));
        qqmusic::utils::buffer buf((uint8_t*) payload.data(), payload.size());
        auto aes_res = aes_encrypt(crypt_key_buf, buf);
        if (aes_res.isErr()) {
            return Err(
                qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                          std::format("[get_qimei] -- failed to get qimei: {}",
                                                      rsa_res.unwrapErr().what())));
        } else {
            params = Botan::base64_encode(aes_res.unwrap());
        }

        std::string extra = R"({"appKey":")";
        extra += APP_KEY;
        extra += R"("})";

        /*get time stamp by second*/
        uint64_t ts = std::chrono::time_point<std::chrono::system_clock>(
                          std::chrono::system_clock::now())
                          .time_since_epoch()
                          .count();
        ts /= 1000000000;

        boost::uuids::detail::md5 hash;
        boost::uuids::detail::md5::digest_type d;
        std::string ts_s = std::to_string(ts * 1000);
        auto hash_buf = key + params + ts_s + nonce + SECRET + extra;
        hash.process_bytes(hash_buf.data(), hash_buf.size());
        hash.get_digest(d);
        std::string sign = Botan::hex_encode(d, sizeof(d), false);

        /*prepare params*/
        boost::uuids::detail::md5 header_hash;
        boost::uuids::detail::md5::digest_type hd;
        ts_s = std::to_string(ts);
        auto header_sign_buf = "qimei_qq_androidpzAuCmaFAaFaHrdakPjLIEqKrGnSOOvH" + ts_s;
        header_hash.process_bytes(header_sign_buf.data(), header_sign_buf.size());
        header_hash.get_digest(hd);

        /*request header table*/
        nlohmann::json headers = {{"Host", "api.tencentmusic.com"},
                                  {"method", "GetQimei"},
                                  {"service", "trpc.tme_datasvr.qimeiproxy.QimeiProxy"},
                                  {"appid", "qimei_qq_android"},
                                  {"sign", Botan::hex_encode(hd, sizeof(hd), false)},
                                  {"user-agent", "QQMusic"},
                                  {"timestamp", ts_s},
                                  {"content-type", "application/json"},
                                  {"accept", "*/*"},
                                  {"accept-encoding", "gzip, deflate"}};

        /*request body table*/
        nlohmann::json body = {
            {"app", 0},
            {"os", 1},
            {"qimeiParams",
             {
                 {"key", key},
                 {"params", params},
                 {"time", ts_s},
                 {"nonce", nonce},
                 {"sign", sign},
                 {"extra", extra},
             }},
        };

        boost::urls::url url(QIMEI_ENTRY);
        http::request<http::string_body> req{http::verb::post, url, 11};
        for (auto& i : headers.items()) {
            req.set(i.key(), i.value());
        }

        req.body() = body.dump();
        req.prepare_payload();

        boost::asio::io_context ioc;
        tcp_stream tcps(ioc);
        auto resolver = boost::asio::ip::tcp::resolver(ioc);
        tcps.connect(resolver.resolve(url.host(), "http"));

        http::write(tcps, req);
        flat_buffer fb;
        http::response<http::dynamic_body> res;
        http::read(tcps, fb, res);

        /*raw_json_res["data"] is string*/
        auto qimei_res = nlohmann::json::parse(
            std::string(nlohmann::json::parse(buffers_to_string(res.body().data()))["data"]));

        if (qimei_res["code"] != 0) {
            /*get qimei failure*/
            return Ok(qqmusic::utils::QimeiResult{.q16 = "",
                                                  .q36 = "6c9d3cd110abca9b16311cee10001e717614"});
        } else {
            /*get qimei success*/
            return Ok(qqmusic::utils::QimeiResult{.q16 = qimei_res["data"]["q16"],
                                                  .q36 = qimei_res["data"]["q36"]});
        }

    } catch (const std::exception& e) {
        /*exception, for debug*/
        // return Err(
        //     qqmusic::utils::Exception(qqmusic::utils::Exception::UnknownError,
        //                               std::format("[get_qimei] -- Error ocurred: {}",
        //                               e.what())));
        return Ok(
            qqmusic::utils::QimeiResult{.q16 = "", .q36 = "6c9d3cd110abca9b16311cee10001e717614"});
    }
}

/*rsa encrypt, padding: PKCS1v15*/
static qqmusic::Result<qqmusic::utils::buffer> rsa_encrypt(qqmusic::utils::buffer& buf) {
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

/*aes encrypt*/
static qqmusic::Result<qqmusic::utils::buffer> aes_encrypt(qqmusic::utils::buffer& key,
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

        /*use padding_size as char to pad after buffer*/
        qqmusic::utils::buffer padding(padding_size, (char) ('\0' + padding_size));
        buf.insert(buf.end(), padding.begin(), padding.end());
        enc->start(key);
        enc->update(buf);
        enc->finish(key);

        return Ok(buf);
    } catch (const std::exception& e) {
        return Err(
            qqmusic::utils::Exception(qqmusic::utils::Exception::DataDestroy,
                                      std::format("[aes_encrypt] -- error ocurred: {}", e.what())));
    }
}

static nlohmann::json load_rand_payload(qqmusic::utils::Device& device, std::string_view version) {
    Botan::AutoSeeded_RNG rng;
    /*generate an uint64_t range(0, 14400)*/
    auto fixed_rand = [&rng]() {
        uint64_t res = 0;
        std::vector<uint8_t> buffer(sizeof(res));
        rng.randomize(buffer);
        memcpy(&res, buffer.data(), sizeof(res));
        return res;
    }() % 14400;

    std::chrono::time_point<std::chrono::system_clock> tp(std::chrono::system_clock::now());
    /*add random offset*/
    tp -= std::chrono::duration(std::chrono::seconds(fixed_rand));
    /*zoned time with random offset*/
    auto zoned_time = std::chrono::zoned_time{std::chrono::current_zone(), tp};
    std::string uptimes = std::format("{0:%F} {0:%H}:{0:%M}:{0:%OS}", zoned_time);

    nlohmann::json reserved = {{"harmony", "0"},
                               {"clone", "0"},
                               {"containe", ""},
                               {"oz", "UhYmelwouA+V2nPWbOvLTgN2/m8jwGB+yUB5v9tysQg="},
                               {"oo", "Xecjt+9S1+f8Pz2VLSxgpw=="},
                               {"kelong", "0"},
                               {"uptimes", uptimes},
                               {"multiUser", "0"},
                               {"bod", device.brand},
                               {"dv", device.device},
                               {"firstLevel", ""},
                               {"manufact", device.brand},
                               {"name", device.model},
                               {"host", "se.infra"},
                               {"kernel", device.proc_version}};
    return nlohmann::json{
        {"androidId", device.android_id},
        {"platformId", 1},
        {"appKey", APP_KEY},
        {"appVersion", version},
        {"beaconIdSrc", random_beacon_id()},
        {"brand", device.brand},
        {"channelId", "10003505"},
        {"cid", ""},
        {"imei", device.imei},
        {"imsi", ""},
        {"mac", ""},
        {"model", device.model},
        {"networkType", "unknown"},
        {"oaid", ""},
        {"osVersion",
         std::format("Android {},level {}", device.version.release, device.version.sdk)},
        {"qimei", ""},
        {"qimei36", ""},
        {"sdkVersion", "1.2.13.6"},
        {"targetSdkVersion", "33"},
        {"audit", ""},
        {"userId", "{}"},
        {"packageId", "com.tencent.qqmusic"},
        {"deviceType", "Phone"},
        {"sdkName", ""},
        {"reserved", nlohmann::to_string(reserved)},
    };
};

static std::string random_beacon_id() {
    std::ostringstream beacon_id;
    const static std::set<int> numtable
        = {1, 2, 13, 14, 17, 18, 21, 22, 25, 26, 29, 30, 33, 34, 37, 38};

    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now = *std::localtime(&time_t_now);

    std::ostringstream time_month;
    time_month << std::put_time(&tm_now, "%Y-%m-") << "01";

    Botan::AutoSeeded_RNG rng;

    /*random unsigned long long generator*/
    auto randull = [&rng]() -> uint64_t {
        uint64_t res = 0;
        std::vector<uint8_t> buffer(sizeof(res));
        rng.randomize(buffer);
        memcpy(&res, buffer.data(), sizeof(res));
        return res;
    };

    auto rand1 = randull() % 899999 + 100000;
    auto rand2 = randull() % 899999999 + 100000000;

    for (int i = 1; i <= 40; ++i) {
        if (numtable.contains(i)) {
            beacon_id << "k" << i << ":" << time_month.str() << rand1 << "." << rand2;
        } else if (i == 3) {
            beacon_id << "k3:0000000000000000";
        } else if (i == 4) {
            beacon_id << "k4:";
            for (int j = 0; j < 16; ++j) {
                const char hex_chars[] = "123456789abcdef";
                beacon_id << hex_chars[randull() % 16];
            }
        } else {
            beacon_id << "k" << i << ":" << randull() % 10000;
        }
        beacon_id << ";";
    }

    return beacon_id.str();
}
