/***************************************************************************************************
 * @file  terrain.tesc
 * @brief Tesselation control shader used for rendering a terrain
 **************************************************************************************************/

#version 460 core

layout (vertices = 4) out;

struct Plane {
    vec3 normal;
    float distance;
};

struct Frustum {
    Plane near_plane;
    Plane far_plane;
    Plane top_plane;
    Plane bottom_plane;
    Plane left_plane;
    Plane right_plane;
};

uniform Frustum u_frustum;
uniform float u_chunk_size;

bool is_on_or_above_plane(Plane plane, vec2 center, float extent) {
    float signed_distance = plane.normal.x * center.x + plane.normal.z * center.y - plane.distance;
    return signed_distance >= -2.0f * max(extent * abs(plane.normal.x), extent * abs(plane.normal.z));
}

bool is_in_frustum(Frustum frustum, vec2 center, float extent) {
    return is_on_or_above_plane(frustum.near_plane, center, extent) &&
           is_on_or_above_plane(frustum.far_plane, center, extent) &&
           is_on_or_above_plane(frustum.top_plane, center, extent) &&
           is_on_or_above_plane(frustum.bottom_plane, center, extent) &&
           is_on_or_above_plane(frustum.left_plane, center, extent) &&
           is_on_or_above_plane(frustum.right_plane, center, extent);
}

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if(gl_InvocationID % 4 == 0) {
        float extent = u_chunk_size / 2.0f;
        vec2 center = gl_in[gl_InvocationID].gl_Position.xz + vec2(extent);
        float level = is_in_frustum(u_frustum, center, extent) ? 16.0f : 0.0f;

        gl_TessLevelOuter[0] = level;
        gl_TessLevelOuter[1] = level;
        gl_TessLevelOuter[2] = level;
        gl_TessLevelOuter[3] = level;

        gl_TessLevelInner[0] = level;
        gl_TessLevelInner[1] = level;
    }
}
