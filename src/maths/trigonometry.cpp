/***************************************************************************************************
 * @file  trigonometry.cpp
 * @brief Implementation of functions regarding trigonometry
 **************************************************************************************************/

#include "maths/trigonometry.hpp"

#include <cmath>

float radians(float deg) {
    return deg * M_PIf / 180.0f;
}

float degrees(float rad) {
    return rad * 180.0f / M_PIf;
}
