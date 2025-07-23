/***************************************************************************************************
 * @file  geometry.hpp
 * @brief Declaration of some functions regarding vector maths
 **************************************************************************************************/

#pragma once

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

/**
 * @brief Calculates the length of a vec2.
 * @param vec The vec2.
 * @return The length.
 */
float length(const vec2& vec);

/**
 * @brief Calculates the length of a vec3.
 * @param vec The vec3.
 * @return The length.
 */
float length(const vec3& vec);

/**
 * @brief Calculates the length of a vec4.
 * @param vec The vec4.
 * @return The length.
 */
float length(const vec4& vec);

/**
 * @brief Calculates the dot product of two vec2.
 * @param left The left operand.
 * @param right The right operand.
 * @return The dot product of the two vec2.
 */
float dot(const vec2& left, const vec2& right);

/**
 * @brief Calculates the dot product of two vec3.
 * @param left The left operand.
 * @param right The right operand.
 * @return The dot product of the two vec3.
 */
float dot(const vec3& left, const vec3& right);

/**
 * @brief Calculates the dot product of two vec4.
 * @param left The left operand.
 * @param right The right operand.
 * @return The dot product of the two vec4.
 */
float dot(const vec4& left, const vec4& right);

/**
 * @brief Calculates the normalized vector of a vec2.
 * @param vec The vec2.
 * @return The normalized vec2.
 */
vec2 normalize(const vec2& vec);

/**
 * @brief Calculates the normalized vector of a vec3.
 * @param vec The vec3.
 * @return The normalized vec3.
 */
vec3 normalize(const vec3& vec);

/**
 * @brief Calculates the normalized vector of a vec4.
 * @param vec The vec4.
 * @return The normalized vec4.
 */
vec4 normalize(const vec4& vec);

/**
 * @brief Calculates the cross product of two vec3.
 * @param left The left operand.
 * @param right The right operand.
 * @return The cross product of the two vec3.
 */
vec3 cross(const vec3& left, const vec3& right);