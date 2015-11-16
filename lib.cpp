// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
// using namespace std;

typedef unsigned char uchar;
const uchar UCHAR_MAX = 0xFF;
const uchar MASK_IMAGE_DATA = 0xFE;
const uchar MASK_HIDDEN_DATA = 0x01;
const uchar BITS_PER_CHAR = sizeof(char) * 8;

std::vector<bool> read_file_into_bool_vector(std::string input_filename) {
    // create this vector ourselves since it needs to start empty
    std::vector<bool> bits;
    std::ifstream file(input_filename, std::ios::binary);
    // don't even need an array since it's just one byte at a time
    char data;
    while (file.read(&data, 1)) {
        for (int i = 0; i < BITS_PER_CHAR; i++) {
            // >> i : (shift) find the bit
            // & 1  : isolate that bit
            // != 0 : implicitly convert to bool
            bits.push_back(((data >> i) & 1) != 0);
        }
    }
    return bits;
}

void conbine_bits(std::vector<unsigned char> &image_data, std::vector<bool> &bits, bool pad = false) {
    // iterate like this so that we skip the pixels representing transparency
    // (since that's a dead giveaway)
    if (bits.size() > (image_data.size() * 3 + 3) / 4) {
        throw std::runtime_error("Image not large enough to encode data!");
    }
    if (pad & bits.size() < (image_data.size() * 3 + 3) / 4) {
        bits.resize((image_data.size() * 3 + 3) / 4, 0);
    }
    for (int i = 0; i < image_data.size(); i += 4) {
        for (int j = i; j < (i + 3); j++) {
            if ((j * 3 + 3) / 4 >= bits.size()) {
                return;
            }
            image_data[j] = (image_data[j] & MASK_IMAGE_DATA) | (bits[(j * 3 + 3) / 4] & MASK_HIDDEN_DATA);
        }
    }
}

std::vector<bool> extract_bits(const std::vector<unsigned char> &image_data) {
    // create this vector ourselves since it needs to start empty
    std::vector<bool> bits((image_data.size() * 3) / 4, 0);
    // iterate like this so that we skip the pixels representing transparency
    // (since that's a dead giveaway)
    for (int i = 0; i < image_data.size(); i += 4) {
        for (int j = i; j < (i + 3); j++) {
            // simply use the mask on every bit
            bits[(j * 3 + 3) / 4] = image_data[j] & MASK_HIDDEN_DATA;
        }
    }
    return bits;
}
