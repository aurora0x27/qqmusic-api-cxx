#include <iostream>
#include <nlohmann/detail/conversions/to_json.hpp>
#include <nlohmann/json.hpp>
#include <qqmusic/details/device.h>

int main(int argc, char** argv) {
    qqmusic::details::Device d;
    nlohmann::json j;
    nlohmann::to_json(j, d);
    auto s = j.dump();
    std::cout << s << std::endl;
    return 0;
}
