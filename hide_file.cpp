// (c) Copyright 2015 Josh Wright
#include <iostream>
// #include <vector>
// #include <string>
// #include <cstdlib>
#include "lib/lodepng.h"
#include "lib/lib.h"

//#define HIDE_FILE_DEBUG

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

    std::vector<unsigned char> image_data;
    unsigned width, height;
    if (encode) {
        lodepng::decode(image_data, width, height, image_input_filename, LCT_RGB);
//        cout << "read image" << endl;
        std::vector<unsigned char> bytes = read_file_into_vector(data_filename);
        combine_bytes(image_data, bytes);
//        cout << "writing image" << endl;
        lodepng::encode(output_filename, image_data, width, height, LCT_RGB);
    } else {
        lodepng::decode(image_data, width, height, image_input_filename, LCT_RGB);
//        cout << "read image" << endl;
        std::vector<unsigned char> bytes = extract_bytes(image_data);
//        cout << "writing file" << endl;
        write_vector_to_file(bytes, output_filename);
    }

    return 0;
}
