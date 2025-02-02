#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <qqmusic/details/cache.h>
#include <qqmusic/details/device.h>

int main(int argc, char** argv) {
    qqmusic::details::Device dev;
    auto res = qqmusic::details::get_device_info();
    if (res.isErr()) {
        std::cout << res.unwrapErr().what() << std::endl;
    } else {
        dev = res.unwrap();
        nlohmann::json j;
        nlohmann::to_json(j, dev);
        std::cout << nlohmann::to_string(j) << std::endl;
    }
    return 0;
}
