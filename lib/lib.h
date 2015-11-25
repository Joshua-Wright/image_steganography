// (c) Copyright 2015 Josh Wright
#pragma once

#include <iostream>
#include <vector>
#include <string>

void combine_bytes(std::vector<unsigned char> &image_data, const std::vector<unsigned char> &bytes);

std::vector<unsigned char> extract_bytes(const std::vector<unsigned char> &image_data);

std::vector<unsigned char> read_file_into_vector(const std::string &filename);

void write_vector_to_file(const std::vector<unsigned char> &data, const std::string &filename);

std::vector<unsigned char> compress_buffer(const std::vector<unsigned char> &data);

std::vector<unsigned char> decompress_buffer(const std::vector<unsigned char> &data);