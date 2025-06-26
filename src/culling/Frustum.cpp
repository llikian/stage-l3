/***************************************************************************************************
 * @file  Frustum.cpp
 * @brief Implementation of the Frustum struct
 **************************************************************************************************/

#include "culling/Frustum.hpp"

#include <cmath>

#include "maths/geometry.hpp"

Frustum::Frustum(const Camera& camera, float aspect_ratio) {
    float far_quad_half_height = camera.far_distance * std::tan(camera.fov / 2.0f);
    float far_quad_half_width = far_quad_half_height * aspect_ratio;

    vec3 far_center = camera.far_distance * camera.direction;
    vec3 far_up = far_quad_half_height * camera.up;
    vec3 far_right = far_quad_half_width * camera.right;

    near_plane.distance = camera.near_distance;
    far_plane.distance = camera.far_distance;
    top_plane.distance = bottom_plane.distance = left_plane.distance = right_plane.distance = 0.0f;

    near_plane.normal = camera.direction;
    far_plane.normal = -camera.direction;
    top_plane.normal = normalize(cross(far_center + far_up, camera.right));
    bottom_plane.normal = normalize(cross(camera.right, far_center - far_up));
    left_plane.normal = normalize(cross(far_center - far_right, camera.up));
    right_plane.normal = normalize(cross(camera.up, far_center + far_right));
}
