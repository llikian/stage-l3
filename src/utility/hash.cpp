/***************************************************************************************************
 * @file  hash.cpp
 * @brief Implementation of some hash functions
 **************************************************************************************************/

#include "utility/hash.hpp"

int hash_pair(int x, int y) {
    return (x + y) * (x + y + 1) / 2;
}

int hash_triplet(int x, int y, int z) {
    return hash_pair(x, hash_pair(y, z));
}
