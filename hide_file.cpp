// (c) Copyright 2015 Josh Wright
#include <iostream>
// #include <vector>
// #include <string>
// #include <cstdlib>
#include "lib/lodepng.h"
#include "lib/lib.h"

#define HIDE_FILE_DEBUG

/*
g++    -std=gnu++14 -O3 lodepng.cpp lib.cpp hide_file.cpp -o hide_file
g++ -g -std=gnu++14 -O0 lodepng.cpp lib.cpp hide_file.cpp -o hide_file
./hide_file
*/

int main(int argc, char const *argv[]) {
    using namespace std;

    std::string image_input_filename;
    std::string output_filename;
    std::string data_filename;

    bool encode;

    if (argc < 3) {
#ifdef HIDE_FILE_DEBUG
//        encode = true;
//        image_input_filename = "test_image.png";
//        output_filename = "test_image.hidden.png";
//        data_filename = "test.bin";
        encode = false;
        image_input_filename = "/home/j0sh/Dropbox/code/Cpp/hide_stuff_in_images/tests/image_test_2.png";
        output_filename = "/home/j0sh/Dropbox/code/Cpp/hide_stuff_in_images/tests/test.bin.new";
#else
        cout << "Usage: " << argv[0] << " -e <data to hide> <input image> <output image>" << endl;
        cout << "Usage: " << argv[0] << " -d <image with hidden data> <output file>" << endl;
        cout << "-e    encode" << endl;
        cout << "-d    decode" << endl;
        return 0; /* not technically an error */
    } else {
        string enc_dec = argv[1];
        if (enc_dec == "-d") {
            encode = false;
            image_input_filename = argv[2];
            output_filename = argv[3];
        } else if (enc_dec == "-e") {
            encode = true;
            data_filename = argv[2];
            image_input_filename = argv[3];
            output_filename = argv[4];
        } else {
            cerr << "Unrecognized option: " << enc_dec << endl;
            return 1;
        }
#endif
    }

//#ifdef HIDE_FILE_DEBUG
//    std::vector<unsigned char> image_data;
//    unsigned width, height;
//    lodepng::decode(image_data, width, height, image_input_filename);
//    cout << "read input image" << endl;
//    std::vector<bool> data_to_hide = read_file_into_bool_vector(data_filename);
//    cout << "read data file" << endl;
//    conbine_bits(image_data, data_to_hide);
//    cout << "combined bits" << endl;
//    lodepng::encode(output_filename, image_data, width, height);
//    cout << "wrote output image" << endl;
//    return 0;
//#endif

    std::vector<unsigned char> image_data;
    unsigned width, height;
    if (encode) {
        lodepng::decode(image_data, width, height, image_input_filename);

//        std::vector<bool> data_to_hide = read_file_into_bool_vector(data_filename);
//        combine_bits(image_data, data_to_hide);

        std::vector<unsigned char> bytes = read_file_into_vector(data_filename);
        combine_bytes(image_data, bytes);

        lodepng::encode(output_filename, image_data, width, height);
    } else {
        lodepng::decode(image_data, width, height, image_input_filename);

//        std::vector<bool> hidden_data = extract_bits(image_data);
//        write_bool_vector_to_file(bytes, output_filename);

        std::vector<unsigned char> bytes = extract_bytes(image_data);
        write_vector_to_file(bytes, output_filename);
    }

    return 0;
}
