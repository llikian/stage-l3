/***************************************************************************************************
 * @file  hash.hpp
 * @brief Declaration of some hash functions
 **************************************************************************************************/

#pragma once

#include <iostream>
#include "maths/vec3.hpp"

/**
 * @brief Class used to hash an ivec3.
 */
struct ivec3_hash {
    /**
     * @brief Hashing function for an ivec3.
     * @param vec The ivec3 to hash.
     * @return The ivec3's hash.
     */
    std::size_t operator()(const ivec3& vec) const;
};
