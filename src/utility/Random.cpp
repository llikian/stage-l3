/***************************************************************************************************
 * @file  Random.cpp
 * @brief Implementation of the Random class
 **************************************************************************************************/

#include "utility/Random.hpp"

float Random::get_float(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(generator);
}

vec2 Random::get_vec2(float min, float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return vec2(distribution(generator), distribution(generator));
}

vec2 Random::get_vec2(vec2 min, vec2 max) {
    return vec2(get_float(min.x, max.x), get_float(min.y, max.y));
}

vec3 Random::get_vec3(float min, float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return vec3(distribution(generator), distribution(generator), distribution(generator));
}

vec3 Random::get_vec3(vec3 min, vec3 max) {
    return vec3(get_float(min.x, max.x), get_float(min.y, max.y), get_float(min.z, max.z));
}

vec4 Random::get_vec4(float min, float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return vec4(distribution(generator), distribution(generator), distribution(generator), distribution(generator));
}

vec4 Random::get_vec4(vec4 min, vec4 max) {
    return vec4(get_float(min.x, max.x), get_float(min.y, max.y), get_float(min.z, max.z), get_float(min.w, max.w));
}
