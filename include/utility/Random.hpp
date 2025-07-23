/***************************************************************************************************
 * @file  Random.hpp
 * @brief Declaration of the Random class
 **************************************************************************************************/

#pragma once

#include <random>
#include "maths/vec2.hpp"
#include "maths/vec3.hpp"
#include "maths/vec4.hpp"

/**
 * @class Random
 * @brief
 */
class Random {
public:
    static float get_float(float min, float max);

    static vec2 get_vec2(float min, float max);
    static vec2 get_vec2(vec2 min, vec2 max);

    static vec3 get_vec3(float min, float max);
    static vec3 get_vec3(vec3 min, vec3 max);

    static vec4 get_vec4(float min, float max);
    static vec4 get_vec4(vec4 min, vec4 max);

private:
    inline static std::random_device seed{};
    inline static std::default_random_engine generator{seed()};
};
