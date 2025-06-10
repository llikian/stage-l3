/***************************************************************************************************
 * @file  hash.hpp
 * @brief Declaration of some hash functions
 **************************************************************************************************/

#pragma once

#include <cstdint>

// https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function
uint64_t hash_pair(uint64_t x, uint64_t y);

uint64_t hash_triplet(uint64_t x, uint64_t y, uint64_t z);