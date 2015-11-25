// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <zlib.h>

/*for debugging*/
using std::cout;
using std::endl;

using std::size_t;
typedef unsigned char uchar;
//const uchar UCHAR_MAX = 0xFF;
const uchar MASK_IMAGE_DATA = 0xfe;
const uchar MASK_HIDDEN_DATA = 0x01;
const uchar BITS_PER_CHAR = sizeof(char) * 8;

void combine_bytes(std::vector<unsigned char> &image_data, const std::vector<unsigned char> &bytes) {
    for (size_t i = 0; i < image_data.size(); ++i) {
        if (i/8 < bytes.size()) {
            /*unset the leftmost bit*/
            image_data.at(i) &= MASK_IMAGE_DATA;
            /*set the leftmost bit to our bit*/
            image_data.at(i) |= (bytes.at(i / 8) >> (i % 8)) & 0x01;
        }
    }
}

std::vector<unsigned char> extract_bytes(const std::vector<unsigned char> &image_data) {
    /*initialize all bytes to 0x00*/
    std::vector<unsigned char> bytes(image_data.size() / 8, 0x00);
//    cout << "input image size: " << image_data.size() << endl;
//    cout << "data size: " << bytes.size() << endl;
    for (size_t i = 0; i < bytes.size(); ++i) {
        /*set each bit individually*/
        bytes.at(i) |= (image_data.at(i * 8 + 0) & MASK_HIDDEN_DATA) << 0;
        bytes.at(i) |= (image_data.at(i * 8 + 1) & MASK_HIDDEN_DATA) << 1;
        bytes.at(i) |= (image_data.at(i * 8 + 2) & MASK_HIDDEN_DATA) << 2;
        bytes.at(i) |= (image_data.at(i * 8 + 3) & MASK_HIDDEN_DATA) << 3;
        bytes.at(i) |= (image_data.at(i * 8 + 4) & MASK_HIDDEN_DATA) << 4;
        bytes.at(i) |= (image_data.at(i * 8 + 5) & MASK_HIDDEN_DATA) << 5;
        bytes.at(i) |= (image_data.at(i * 8 + 6) & MASK_HIDDEN_DATA) << 6;
        bytes.at(i) |= (image_data.at(i * 8 + 7) & MASK_HIDDEN_DATA) << 7;
    }
    return bytes;
}

std::vector<unsigned char> read_file_into_vector(const std::string &filename) {
    std::ifstream input_file(filename, std::ios::binary | std::ios::ate);
    /*start at end of file so we can use this to tell how big it is*/
    size_t file_size = (size_t) input_file.tellg();
    /*make a vector the same size as the file*/
    std::vector<unsigned char> bytes(file_size, 0x00);
    input_file.seekg(0, std::ios_base::beg);
    /*read directly into the vector*/
    input_file.read((char *) bytes.data(), file_size);
//    cout << "input file size: " << file_size << endl;
    return bytes;
}

void write_vector_to_file(const std::vector<unsigned char> &data, const std::string &filename) {
    std::ofstream output_file(filename, std::ios::binary);
    /*direclty write the vector*/
    output_file.write((const char *) data.data(), data.size());
//    cout << "output file size: " << data.size() << endl;
}


std::vector<unsigned char> compress_buffer(const std::vector<unsigned char> &data) {
    size_t output_length = data.size() * 2;
    std::vector<unsigned char> output(output_length, 0);
    int err = compress2(output.data(), &output_length, data.data(), data.size(), Z_BEST_COMPRESSION);
    switch (err) {
        case Z_BUF_ERROR:
            throw std::runtime_error("Z_BUF_ERROR");
        case Z_DATA_ERROR:
            throw std::runtime_error("Z_DATA_ERROR");
        case Z_MEM_ERROR:
            throw std::runtime_error("Z_MEM_ERROR");
        case Z_STREAM_ERROR:
            throw std::runtime_error("Z_STREAM_ERROR");
        case Z_OK:
            output.resize(output_length);
            return output;
        default:
            throw std::runtime_error("Some other (zlib?) error");
    }
}

std::vector<unsigned char> decompress_buffer(const std::vector<unsigned char> &data) {
    size_t output_length = data.size();
    /*extra big buffer because we're decompressing*/
    std::vector<unsigned char> output(data.size() * 2, 0);
    int err = uncompress(output.data(), &output_length, data.data(), data.size());
    switch (err) {
        case Z_BUF_ERROR:
            throw std::runtime_error("Z_BUF_ERROR");
        case Z_DATA_ERROR:
            throw std::runtime_error("Z_DATA_ERROR");
        case Z_MEM_ERROR:
            throw std::runtime_error("Z_MEM_ERROR");
        case Z_STREAM_ERROR:
            throw std::runtime_error("Z_STREAM_ERROR");
        case Z_OK:
            output.resize(output_length);
            return output;
        default:
            throw std::runtime_error("Some other (zlib?) error");
    }
}