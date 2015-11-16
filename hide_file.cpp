// (c) Copyright 2015 Josh Wright
#include <iostream>
// #include <vector>
// #include <string>
// #include <cstdlib>
#include "lodepng.h"
#include "lib.h"

/*
g++    -std=gnu++14     lodepng.cpp lib.cpp hide_file.cpp -o hide_file
g++ -g -std=gnu++14 -O0 lodepng.cpp lib.cpp hide_file.cpp -o hide_file
./hide_file
*/

int main(int argc, char const *argv[]) {
    using namespace std;
    std::string image_input_filename;
    std::string image_output_filename;
    std::string data_filename;
    if (argc < 3) {
        // return 1;
        image_input_filename = "test_image.png";
        image_output_filename = "test_image.hidden.png";
        data_filename = "test.bin";
    } else {
        data_filename = argv[1];
        image_input_filename = argv[2];
        image_output_filename = argv[3];
    }

    std::vector<unsigned char> image_data;
    unsigned width, height;
    lodepng::decode(image_data, width, height, image_input_filename);
    cout << "read input image" << endl;

    std::vector<bool> data_to_hide = read_file_into_bool_vector(data_filename);
    cout << "read data file" << endl;
    conbine_bits(image_data, data_to_hide);
    cout << "combined bits" << endl;

    lodepng::encode(image_output_filename, image_data, width, height);
    cout << "wrote output image" << endl;
    return 0;
}
