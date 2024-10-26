#include <fstream>

#include <utils/qrcdecoder.h>
#include <utils/buffer.h>

int main(int argc, char** argv)
{
    if (argc == 1) {
        printf("Too few args...\n");
        printf("Usage: ");
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        std::fstream input_file;
        input_file.open(argv[i], std::ios::binary | std::ios::in);
        input_file.seekg(0, std::ios::end);
        size_t input_file_size = input_file.tellg();
        input_file.seekg(0, std::ios::beg);
        uint8_t* input_file_buf = new uint8_t[input_file_size];
        input_file.read((char*)input_file_buf, input_file_size);
        input_file_buf[input_file_size] = '\0';
        qqmusic::utils::buffer* in_buf = new qqmusic::utils::buffer(input_file_buf, input_file_size);
        delete[] input_file_buf;
        input_file.close();
        qqmusic::utils::buffer* out_buf = new qqmusic::utils::buffer();
        // decode...
        auto res = qqmusic::utils::qrc_decode(in_buf, out_buf, qqmusic::utils::qrc_type::local);
        std::fstream output_file;
        std::string output_file_name = (std::string)argv[i] + ".decode";
        output_file.open(output_file_name, std::ios::out);
        std::string output_file_buf((char*)out_buf->get_head(), out_buf->get_size());
        output_file.write(output_file_buf.c_str(), output_file_buf.size());
        delete out_buf;
        output_file.close();
    }

    return 0;
}
