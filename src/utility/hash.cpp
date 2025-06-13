/***************************************************************************************************
 * @file  hash.cpp
 * @brief Implementation of some hash functions
 **************************************************************************************************/

#include "utility/hash.hpp"

size_t ivec3_hash::operator()(const ivec3& vec) const {
    return std::hash<int>{}(vec.x) ^ (std::hash<int>{}(vec.y) << 1) ^ (std::hash<int>{}(vec.z) << 2);
}
