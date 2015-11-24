// (c) Copyright 2015 Josh Wright
#pragma once

#include <iostream>
#include <vector>
#include <string>

std::vector<bool> read_file_into_bool_vector(std::string input_filename);

void write_bool_vector_to_file(const std::vector<bool> &bits, std::string output_filename);

void combine_bits(std::vector<unsigned char> &image_data, std::vector<bool> &bits, bool pad = false);

std::vector<bool> extract_bits(const std::vector<unsigned char> &image_data);
