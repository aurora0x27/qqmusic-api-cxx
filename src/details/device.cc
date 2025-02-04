#include <boost/uuid.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <botan/auto_rng.h>
#include <botan/hex.h>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <nlohmann/detail/conversions/from_json.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/details/device.h>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/cache.h>
#include <string>

qqmusic::result<qqmusic::details::Device> qqmusic::details::get_device_info() {
    try {
        auto cache_path = qqmusic::details::CacheManager::get_instance().get_cache_path()
                          / std::filesystem::path("device.json");

        qqmusic::details::Device device;

        std::fstream fs(cache_path.c_str());
        if (fs.good()) {
            /*file exist, load cache*/
            std::ostringstream oss;
            std::string line;
            while (std::getline(fs, line)) {
                oss << line << '\n';
            }
            std::string s = oss.str();

            try {
                nlohmann::json j = nlohmann::json::parse(s);
                nlohmann::from_json(j, device);
                return Ok(device);
            } catch (const std::exception& e) {
                /*parse error, data destroy, create a new one*/
                fs.open(cache_path.c_str(), std::ios::out);
                nlohmann::json j;
                nlohmann::to_json(j, device);
                fs << nlohmann::to_string(j);
                return Ok(device);
            }
        } else {
            /*file not exist, create a new one*/
            fs.open(cache_path.c_str(), std::ios::out);
            nlohmann::json j;
            nlohmann::to_json(j, device);
            fs << nlohmann::to_string(j);
            return Ok(device);
        }
    } catch (const std::exception& e) {
        return Err(qqmusic::utils::Exception(
            qqmusic::utils::Exception::DataDestroy,
            std::format("[get_device_info] -- get random device info failure: {}", e.what())));
    }
}

qqmusic::details::OSVersion::OSVersion() {
    incremental = "5891938";
    release = "10";
    codename = "REL";
    sdk = 29;
}

qqmusic::details::Device::Device() {
    /*default initialization*/

    Botan::AutoSeeded_RNG rng;

    /*random unsigned long long generator*/
    auto randull = [&rng]() -> uint64_t {
        uint64_t res = 0;
        std::vector<uint8_t> buffer(sizeof(res));
        rng.randomize(buffer);
        memcpy(&res, buffer.data(), sizeof(res));
        return res;
    };

    /*random string generator*/
    auto randstr = [&randull](int len) -> std::string {
        static const char alphanum[] = "0123456789"
                                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                       "abcdefghijklmnopqrstuvwxyz";
        std::string tmp_s;
        tmp_s.reserve(len);

        for (int i = 0; i < len; ++i) {
            tmp_s += alphanum[randull() % (sizeof(alphanum) - 1)];
        }

        return tmp_s;
    };

    /*random imei generator */
    /* 
     * TODO: The imei cannot pass check, either the python version.
     *       def random_imei() -> str:
     *           """生成随机 IMEI 号码.
     *           Returns:
     *               随机生成的 IMEI 号码
     *           """
     *           imei = []
     *           sum_ = 0
     *           for i in range(14):
     *               num = random.randint(0, 9)
     *               if (i + 2) % 2 == 0:
     *                   num *= 2
     *                   if num >= 10:
     *                       num = (num % 10) + 1
     *               sum_ += num
     *               imei.append(str(num))
     *           ctrl_digit = (sum_ * 9) % 10
     *           imei.append(str(ctrl_digit))
     *           return "".join(imei)
     * */
    auto randimei = [&randull]() -> std::string {
        std::string res;
        int sum = 0;
        for (int i = 0; i < 14; ++i) {
            int num = (int) (randull() % 10);
            if ((i + 2) % 2 == 0) {
                num *= 2;
                num %= 10;
            }
            sum += num;
            res += (char) ('0' + num);
        }
        int ctrl_digit = (sum * 9) % 10;
        res += (char) ('0' + ctrl_digit);
        return res;
    };

    display = std::format("QMAPI.{}.001", randull() % 9999999 + 1000000);
    product = "iarim";
    device = "sagit";
    board = "eomam";
    model = "MI 6";

    fingerprint = std::format("xiaomi/iarim/sagit:10/eomam.200122.001/{}:user/release-keys",
                              randull() % 9999999 - 1000000);

    boost::uuids::uuid u = boost::uuids::random_generator()();
    boot_id = to_string(u);

    proc_version = std::format("Linux 5.4.0-54-generic-{} (android-build@google.com)", randstr(8));

    imei = randimei();
    brand = "Xiaomi";
    bootloader = "U-boot";
    base_band = "";
    version = OSVersion();
    sim_info = "T-Mobile";
    os_type = "android";
    mac_address = "00:50:56:C0:00:08";
    ip_address = {10, 0, 1, 3};
    wifi_bssid = "00:50:56:C0:00:08";
    wifi_ssid = "<unknown ssid>";

    /*calculate md5 sum and store as std::vector<int> by bytes*/
    std::vector<uint8_t> imsi_buf(16);
    rng.random_vec(imsi_buf);
    boost::uuids::detail::md5 hash;
    boost::uuids::detail::md5::digest_type d;
    hash.process_bytes(imsi_buf.data(), imsi_buf.size());
    hash.get_digest(d);
    qqmusic::utils::buffer imsi_res_buf(d, 16);

    imsi_md5 = std::vector<int>(imsi_res_buf.begin(), imsi_res_buf.end());

    std::vector<uint8_t> android_id_buf(8);
    rng.randomize(android_id_buf);
    android_id = Botan::hex_encode(android_id_buf, false);
    apn = "wifi";
    vendor_name = "MIUI";
    vendor_os_name = "qmapi";
}
