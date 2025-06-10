/***************************************************************************************************
 * @file  hash.hpp
 * @brief Declaration of some hash functions
 **************************************************************************************************/

#pragma once

// https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function
int hash_pair(int x, int y);

int hash_triplet(int x, int y, int z);