// Copyright(c) 2025 Lorcan McGuinness
// Released under the term specified in file LICENSE.txt
// SPDX short identifier : MIT

#pragma once

#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <map>
#include <string_view>
#include <vector>

std::map<char,int> huffman_frequencies(std::string_view s);

struct symbol_bits
{
	bool operator==(symbol_bits const& other)const
	{
		return symbol == other.symbol && bits == other.bits;
	}
	char symbol;
	int bits;
};

std::vector<symbol_bits> huffman_frequencies_to_bits(std::map<char, int>&& frequencies);

struct codeword
{
	bool operator==(codeword const& other)const
	{
		return total_length == other.total_length && suffix == other.suffix;
	}
	//int leading_1s;
	int total_length;
	unsigned int suffix;
};

std::map<char, codeword> canonical_codes(std::vector<symbol_bits> &&);

#endif
