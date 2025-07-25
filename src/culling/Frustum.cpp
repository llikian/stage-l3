/***************************************************************************************************
 * @file  Frustum.cpp
 * @brief Implementation of the Frustum struct
 **************************************************************************************************/

#include "culling/Frustum.hpp"

void Frustum::update(const Camera& camera) {
    view_projection = camera.get_view_projection_matrix();

    static const vec4 projection_space_points[8]{
        vec4(1.0f, 1.0f, 1.0f, 1.0f),
        vec4(1.0f, 1.0f, -1.0f, 1.0f),
        vec4(1.0f, -1.0f, 1.0f, 1.0f),
        vec4(1.0f, -1.0f, -1.0f, 1.0f),
        vec4(-1.0f, 1.0f, 1.0f, 1.0f),
        vec4(-1.0f, 1.0f, -1.0f, 1.0f),
        vec4(-1.0f, -1.0f, 1.0f, 1.0f),
        vec4(-1.0f, -1.0f, -1.0f, 1.0f)
    };

    mat4 inverse_projection = camera.get_inverse_projection_matrix();
    for(int i = 0 ; i < 8 ; ++i) {
        points[i] = inverse_projection * projection_space_points[i];
        points[i] /= points[i].w;
    }
}
