#include <format>
#include <fstream>
#include <iostream>
#include <qqmusic/utils/buffer.h>
#include <qqmusic/utils/qrc-decoder.h>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "Too few arguments...exiting..." << std::endl;
        std::cerr << std::format("usage: {} <file1> [<file2> <file3> ...]", argv[0]) << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::fstream input_file;
        input_file.open(argv[i], std::ios::binary | std::ios::in);

        std::cout << "reading file begin..." << std::endl;
        input_file.seekg(0, std::ios::end);
        size_t input_file_size = input_file.tellg();
        input_file.seekg(0, std::ios::beg);
        qqmusic::utils::buffer in_buf(input_file_size);
        input_file.read((char*) in_buf.data(), (long) in_buf.size());
        std::cout << "reading file end..." << std::endl;

        input_file.close();

        std::cout << std::format("decode file {} begin", argv[i]) << std::endl;

        // decode...
        auto res = qqmusic::utils::qrc_decode(in_buf, qqmusic::utils::qrc_type::local);
        if (res.isOk()) {
            std::cout << std::format( "decode file {} end", argv[i]) << std::endl;

            std::fstream output_file;
            std::string output_file_name = (std::string) argv[i] + ".decode";
            std::cout << std::format("output file is {}", output_file_name) << std::endl;
            output_file.open(output_file_name, std::ios::out);
            std::cout << "writing file begin..." << std::endl;
            output_file.write(res.unwrap().data(), (long)res.unwrap().size());
            std::cout << "writing file end..." << std::endl;

            output_file.close();
        } else if (res.isErr()) {
            std::cout << res.unwrapErr().what() << std::endl;;
        }
    }

    return 0;
}
