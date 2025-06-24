/***************************************************************************************************
 * @file  trigonometry.cpp
 * @brief Implementation of functions regarding trigonometry
 **************************************************************************************************/

#include "maths/trigonometry.hpp"

#include <cmath>

float degrees_to_radians(float deg) {
    return deg * M_PIf / 180.0f;
}

float radians_to_degrees(float rad) {
    return rad * 180.0f / M_PIf;
}
