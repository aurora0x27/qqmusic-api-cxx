#include <qqmusic/result.h>
#include <future>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <qqmusic/details/device.h>
#include <qqmusic/details/qimei.h>
#include <qqmusic/utils/async-executor.h>
#include <qqmusic/utils/cache.h>

// int main(int argc, char** argv) {
//     auto d = qqmusic::details::get_device_info();
//     if (d.isErr()) {
//         std::cout << "Can't get device, reason: " << d.unwrapErr().what() << std::endl;
//         return 1;
//     }
//     auto device = d.unwrap();
//     auto res = qqmusic::details::get_qimei(device, "13.2.5.8");
//     if (res.isErr()) {
//         std::cout << res.unwrapErr().what() << std::endl;
//     } else {
//         std::cout << res.unwrap().q16 << std::endl;
//         std::cout << res.unwrap().q36 << std::endl;
//     }
//     return 0;
// }

qqmusic::Task<int> add(int a, int b) {
    std::cout << "adding " << a << ' ' << b << std::endl;
    co_return a + b;
}

qqmusic::Task<int> sum(std::vector<int>& vec) {
    int sum = 0;
    int i = 0;
    for (i = 0; i < vec.size(); i += 2) {
        sum += co_await add(vec[i], vec[i + 1]);
    }
    if (i < vec.size() - 1) {
        sum += vec[vec.size() - 1];
    }
    co_return sum;
}

int main(int argc, char** argv) {
    std::vector vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::promise<void> promise;
    auto future = promise.get_future();

    qqmusic::utils::AsyncExecutor::get_instance().async_exec(sum(vec), [&](int sum) {
        std::cout << "sum is " << sum << std::endl;
        promise.set_value();
    });

    std::cout << "hello world" << std::endl;

    future.wait();

    return 0;
}
