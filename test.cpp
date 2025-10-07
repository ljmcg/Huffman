// Copyright(c) 2025 Lorcan McGuinness
// Released under the term specified in file LICENSE.txt
// SPDX short identifier : MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include "huffman.hpp"

TEST_CASE( "huffman_frequencies", "[huffman]" ) {
    auto frequencies = huffman_frequencies("aabc");
    REQUIRE( frequencies.size() == 3 );
    REQUIRE( frequencies['a'] == 2 );
    REQUIRE( frequencies['b'] == 1);
    REQUIRE( frequencies['c'] == 1);
}

TEST_CASE("huffman_bits", "[huffman]") {
    auto frequencies = huffman_frequencies("aabc");
    auto bits_vec = huffman_frequencies_to_bits(std::move(frequencies));
    REQUIRE(bits_vec.size() == 3);
    REQUIRE_THAT(bits_vec, Catch::Matchers::VectorContains(symbol_bits{ 'a',1}));
    REQUIRE_THAT(bits_vec, Catch::Matchers::VectorContains(symbol_bits{ 'b',2 }));
    REQUIRE_THAT(bits_vec, Catch::Matchers::VectorContains(symbol_bits{ 'c',2 }));
}

TEST_CASE("huffman_codes", "[huffman]") {
    auto frequencies = huffman_frequencies("aabc");
    auto bits_vec = huffman_frequencies_to_bits(std::move(frequencies));
    auto codes = canonical_codes(std::move(bits_vec));
    REQUIRE(codes.size() == 3);
    REQUIRE(codes['a'] == codeword{1, 0b00});
    REQUIRE(codes['b'] == codeword{2, 0b10});
    REQUIRE(codes['c'] == codeword{2, 0b11});
}
