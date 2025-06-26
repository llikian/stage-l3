/***************************************************************************************************
 * @file  Plane.cpp
 * @brief Implementation of the Plane struct
 **************************************************************************************************/

#include "culling/Plane.hpp"

#include "maths/geometry.hpp"

Plane::Plane() : normal(0.0f, 1.0f, 0.0f), distance(0.0f) { }

Plane::Plane(const vec3& normal, float distance) : normal(normalize(normal)), distance(distance){ }

float Plane::get_signed_distance(const vec3& point) const {
    return dot(normal, point) - distance;
}
