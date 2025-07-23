/***************************************************************************************************
 * @file  geometry.cpp
 * @brief Implementation of some functions regarding vector maths
 **************************************************************************************************/

#include "maths/geometry.hpp"

#include <cmath>

float length(const vec2& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

float length(const vec3& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float length(const vec4& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

float dot(const vec2& left, const vec2& right) {
    return left.x * right.x + left.y * right.y;
}

float dot(const vec3& left, const vec3& right) {
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

float dot(const vec4& left, const vec4& right) {
    return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}

vec2 normalize(const vec2& vec) {
    return vec / length(vec);
}

vec3 normalize(const vec3& vec) {
    return vec / length(vec);
}

vec4 normalize(const vec4& vec) {
    return vec / length(vec);
}

vec3 cross(const vec3& left, const vec3& right) {
    return vec3(
        left.y * right.z - left.z * right.y,
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x
    );
}
