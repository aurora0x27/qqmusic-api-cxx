#include <qqmusic/result.h>
#include <future>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <qqmusic/details/device.h>
#include <qqmusic/details/qimei.h>
#include <qqmusic/utils/async-executor.h>

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
