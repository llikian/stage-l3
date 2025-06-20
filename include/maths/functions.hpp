/***************************************************************************************************
 * @file  functions.hpp
 * @brief Declaration of maths functions
 **************************************************************************************************/

#pragma once

#include "vec3.hpp"

/**
 * @brief Computes the square of a value.
 * @tparam Type The type of the value.
 * @param value The value to square.
 * @return value * value.
 */
template <typename Type>
Type pow2(Type value) {
    return value * value;
}

vec3 hue_to_rgb(unsigned short hue);
