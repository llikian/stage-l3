/***************************************************************************************************
 * @file  functions.cpp
 * @brief Implementation of maths functions
 **************************************************************************************************/

#include "maths/functions.hpp"
#include <cmath>

vec3 hue_to_rgb(unsigned short hue) {
    float x = 1.0f - std::abs(std::fmod(static_cast<float>(hue) / 60.0f, 2.0f) - 1.0f);

    switch((hue / 60) % 6) {
        case 0: return vec3(1.0f, x, 0.0f);
        case 1: return vec3(x, 1.0f, 0.0f);
        case 2: return vec3(0.0f, 1.0f, x);
        case 3: return vec3(0.0f, x, 1.0f);
        case 4: return vec3(x, 0.0f, 1.0f);
        case 5: return vec3(1.0f, 0.0f, x);
        default: return vec3();
    }
}
