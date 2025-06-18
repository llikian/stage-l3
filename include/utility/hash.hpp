/***************************************************************************************************
 * @file  hash.hpp
 * @brief Declaration of some hash functions
 **************************************************************************************************/

#pragma once

#include <iostream>
#include "maths/vec3.hpp"

/**
 * @struct vector3_hash
 * @brief Class used to hash a vector3.
 * @tparam Type The type of the vector's components.
 */
template <typename Type>
struct vector3_hash {
    /**
     * @brief Hashing function for an vector3.
     * @param vec The vector3 to hash.
     * @return The vector3's hash.
     */
    std::size_t operator()(const vector3<Type>& vec) const {
        return std::hash<Type>{}(vec.x) ^ (std::hash<Type>{}(vec.y) << 1) ^ (std::hash<Type>{}(vec.z) << 2);
    }
};
