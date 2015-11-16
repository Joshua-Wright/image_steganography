// (c) Copyright 2015 Josh Wright
#pragma once

#include <iostream>
#include <vector>
#include <string>
// using namespace std;

std::vector<bool> read_file_into_bool_vector(std::string input_filename);

void conbine_bits(std::vector<unsigned char> &image_data, std::vector<bool> &bits, bool pad = false);

std::vector<bool> extract_bits(const std::vector<unsigned char> &image_data);
