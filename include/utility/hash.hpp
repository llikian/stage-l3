/***************************************************************************************************
 * @file  hash.hpp
 * @brief Declaration of some hash functions
 **************************************************************************************************/

#pragma once

#include <iostream>
#include "maths/ivec3.hpp"

struct ivec3_hash {
    std::size_t operator()(const ivec3& vec) const;
};
