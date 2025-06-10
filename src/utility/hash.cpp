/***************************************************************************************************
 * @file  hash.cpp
 * @brief Implementation of some hash functions
 **************************************************************************************************/

#include "utility/hash.hpp"

uint64_t hash_pair(uint64_t x, uint64_t y) {
    return (x + y) * (x + y + 1) / 2 + y;
}

uint64_t hash_triplet(uint64_t x, uint64_t y, uint64_t z) {
    return hash_pair(hash_pair(x, y), z);
}
