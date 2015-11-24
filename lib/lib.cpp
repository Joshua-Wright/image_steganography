// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>

typedef unsigned char uchar;
const uchar UCHAR_MAX = 0xFF;
const uchar MASK_IMAGE_DATA = 0xfe;
const uchar MASK_HIDDEN_DATA = 0x01;
const uchar BITS_PER_CHAR = sizeof(char) * 8;

std::vector<bool> read_file_into_bool_vector(std::string input_filename) {
    /* open at the end of the file so we know it's length */
    std::ifstream input_file(input_filename, std::ios::binary | std::ios::ate);
    /* initialize vector to size of file */
    std::vector<bool> bits((unsigned long) 8 * input_file.tellg());
    /* seek back to the beginning */
    input_file.seekg(0, std::ios_base::beg);
    char data;
    int i;
    while (input_file.read(&data, 1)) {
//        for (int i = 0; i < BITS_PER_CHAR; i++) {
//            // >> i : (shift) find the bit
//            // & 1  : isolate that bit
//            // != 0 : implicitly convert that bit to bool
//            bits.at((unsigned long) (input_file.tellg() + i)) = ((data >> i) & 1) != 0;
//        }
        bits.at(i + 0) = ((data >> 0) & 1) != 0;
        bits.at(i + 1) = ((data >> 1) & 1) != 0;
        bits.at(i + 2) = ((data >> 2) & 1) != 0;
        bits.at(i + 3) = ((data >> 3) & 1) != 0;
        bits.at(i + 4) = ((data >> 4) & 1) != 0;
        bits.at(i + 5) = ((data >> 5) & 1) != 0;
        bits.at(i + 6) = ((data >> 6) & 1) != 0;
        bits.at(i + 7) = ((data >> 7) & 1) != 0;
        i += 8;

    }
    return bits;
}

void write_bool_vector_to_file(const std::vector<bool> &bits, std::string output_filename) {
    /* TODO: devise a method to know when this is padded */
    std::ofstream output_file(output_filename, std::ios::binary);
    /* floor them so we don't extract the final partial byte */
    for (int i = 0; i < bits.size(); i += 8) {
        output_file.put(
                (((unsigned char) bits.at(i + 0)) << 0) |
                (((unsigned char) bits.at(i + 1)) << 1) |
                (((unsigned char) bits.at(i + 2)) << 2) |
                (((unsigned char) bits.at(i + 3)) << 3) |
                (((unsigned char) bits.at(i + 4)) << 4) |
                (((unsigned char) bits.at(i + 5)) << 5) |
                (((unsigned char) bits.at(i + 6)) << 6) |
                (((unsigned char) bits.at(i + 7)) << 7)
        );
    }
}

void combine_bits(std::vector<unsigned char> &image_data, std::vector<bool> &bits, bool pad = false) {
    /* TODO: add upscaling for bigger data input */
    /* TODO: Find a way to indicate when result has been padded*/
    // iterate like this so that we skip the pixels representing transparency
    // (since that's a dead giveaway)
    if (bits.size() > (image_data.size() * 3 + 3) / 4) {
        std::stringstream ss;
        ss << "Image not large enough to encode data! Image: ";
        ss << image_data.size() << " (" << (image_data.size() * 3 + 3) / 4 << ") ";
        ss << "Data: " << bits.size() / 8;
        throw std::runtime_error(ss.str());
    }
    /* if we don't have enough bits to fill the image, pad it with zeros */
    if (pad && bits.size() < (image_data.size() * 3 + 3) / 4) {
        bits.resize((image_data.size() * 3 + 3) / 4, 0);
    }

    int i_image = 0;
    int i_bits = 0;
    while (i_image < image_data.size() && i_bits < bits.size()) {
        if (i_image % 4 == 3) {
            /* alpha pixel */
            i_image++;
            continue;
        }
        image_data.at(i_image) =
                (image_data.at(i_image) & MASK_IMAGE_DATA) | (((unsigned char) bits.at(i_bits)) & MASK_HIDDEN_DATA);
        i_bits++;
        i_image++;
    }
}

std::vector<bool> extract_bits(const std::vector<unsigned char> &image_data) {
    // create this vector ourselves since it needs to start empty
    std::vector<bool> bits((image_data.size() * 3) / 4, 0);
    int i_image = 0;
    int i_bits = 0;
    while (i_image < image_data.size() && i_bits < bits.size()) {
        if (i_image % 4 == 3) {
            /* alpha pixel */
            i_image++;
            continue;
        }
        bits.at(i_bits) = image_data.at(i_image) & MASK_HIDDEN_DATA;
        i_bits++;
        i_image++;
    }
    return bits;
}
