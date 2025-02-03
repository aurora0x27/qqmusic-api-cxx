#include <botan/auto_rng.h>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <qqmusic/details/cache.h>
#include <qqmusic/details/device.h>
#include <qqmusic/details/qimei.h>

int main(int argc, char** argv) {

    qqmusic::details::Device device;
    auto res = qqmusic::details::get_qimei(device, "114514");
    if (res.isErr()) {
        std::cout << res.unwrapErr().what() << std::endl;
    } else {
        std::cout << res.unwrap().q16 << std::endl;
        std::cout << res.unwrap().q36 << std::endl;
    }

    return 0;
}
