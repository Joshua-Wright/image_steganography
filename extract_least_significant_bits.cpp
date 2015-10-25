// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <vector>
#include "lodepng.h"
// #include <string>
// #include <cstdlib>
/*
g++    -std=gnu++14     lodepng.cpp extract_least_significant_bits.cpp -o extract_least_significant_bits
g++ -g -std=gnu++14 -O0 lodepng.cpp extract_least_significant_bits.cpp -o extract_least_significant_bits
./extract_least_significant_bits
*/

typedef unsigned char uchar;
const uchar UCHAR_MAX        = 0xFF;

int main(int argc, char const *argv[])
{
	using namespace std;
	
	std::string input_filename;
	std::string output_filename;
	if (argc < 2) {
		// return 1;
		input_filename = "test_image.png";
	} else {
		input_filename = argv[1];
	}
	output_filename = input_filename + ".least_significant.png";

	std::vector<unsigned char> image_data;
	unsigned width, height;
	lodepng::decode(image_data, width, height, input_filename);
	// cout << image_data.size() << endl;
	// for (auto& x : image_data) {
	// iterate like this so that we skip the pixels representing transparency
	// (since that's a dead giveaway)
	for (int i=0; i<image_data.size(); i+=4) {
		for (int j=i; j<(i+3); j++) {
			// bit shift so now least significant is most significant
			image_data[j] <<= 7;
		}
	}

	lodepng::encode(output_filename, image_data, width, height);
	return 0;
}
