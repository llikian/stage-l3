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

struct Noise {
    float frequency;
    float amplitude;
    float height;
};

uniform Frustum u_frustum;
uniform float u_chunk_size;

float fade(in float x) {
    float x3 = x * x * x;
    return 6.0f * x3 * x * x - 15.0f * x3 * x + 10.0f * x3;
}

float smooth_lerp(in float a, in float b, in float t) {
    return a + fade(t) * (b - a);
}

float random_2D(in vec2 co) {
    return fract(sin(dot(co, vec2(12.9898f, 78.233f))) * 43758.5453f);
}

float perlin_noise(in vec2 pos) {
    vec2 floor_pos = floor(pos);
    vec2 fract_pos = fract(pos);

    float g00 = random_2D(vec2(floor_pos.x, floor_pos.y));
    float g10 = random_2D(vec2(floor_pos.x + 1.0f, floor_pos.y));
    float g0 = smooth_lerp(g00, g10, fract_pos.x);

    float g01 = random_2D(vec2(floor_pos.x, floor_pos.y + 1.0f));
    float g11 = random_2D(vec2(floor_pos.x + 1.0f, floor_pos.y + 1.0f));
    float g1 = smooth_lerp(g01, g11, fract_pos.x);

    return smooth_lerp(g0, g1, fract_pos.y);
}

float get_noise(vec2 position, Noise noise) {
    return (perlin_noise(position * noise.frequency) - 0.5f) * noise.amplitude;
}

float get_height(vec2 position) {
    Noise plains = Noise(0.01f, 25.0f, -37.0f);
    Noise plateaux = Noise(0.003f, 130.0, 0.0f);
    Noise mountains = Noise(0.004f, 250.0f, 25.0f);

    float heightPlain = plains.height;
    float heightPlateau = plateaux.height;
    float heightMountain = mountains.height;

    for (uint i = 0; i < 8u; ++i) {
        heightPlain += get_noise(position, plains);
        plains.frequency *= 2.0f;
        plains.amplitude /= 2.0f;

        heightPlateau += get_noise(position, plateaux);
        plateaux.frequency *= 2.0f;
        plateaux.amplitude /= 2.0f;

        heightMountain += get_noise(position, mountains);
        mountains.frequency *= 2.0f;
        mountains.amplitude /= 2.0f;
    }

    return max(max(heightPlain, heightPlateau), heightMountain);
}

vec3 get_position(vec2 pos) {
    return vec3(pos.x, get_height(pos), pos.y);
}

bool is_on_or_above_plane(Plane plane, vec3 point) {
    return dot(plane.normal, point) - plane.distance >= 0.0f;
}

bool is_in_frustum(Frustum frustum, vec3 point) {
    return is_on_or_above_plane(frustum.near_plane, point) &&
           is_on_or_above_plane(frustum.far_plane, point) &&
           is_on_or_above_plane(frustum.top_plane, point) &&
           is_on_or_above_plane(frustum.bottom_plane, point) &&
           is_on_or_above_plane(frustum.left_plane, point) &&
           is_on_or_above_plane(frustum.right_plane, point);
}

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if(gl_InvocationID % 4 == 0) {
        float extent = u_chunk_size / 2.0f;
        vec2 center = gl_in[gl_InvocationID].gl_Position.xz + vec2(extent);

        float level = 0.0f;
        if(is_in_frustum(u_frustum, get_position(gl_in[gl_InvocationID].gl_Position.xz)) ||
           is_in_frustum(u_frustum, get_position(gl_in[gl_InvocationID + 1].gl_Position.xz)) ||
           is_in_frustum(u_frustum, get_position(gl_in[gl_InvocationID + 2].gl_Position.xz)) ||
           is_in_frustum(u_frustum, get_position(gl_in[gl_InvocationID + 3].gl_Position.xz))) {
            level = 16.0f;
        }

        gl_TessLevelOuter[0] = level;
        gl_TessLevelOuter[1] = level;
        gl_TessLevelOuter[2] = level;
        gl_TessLevelOuter[3] = level;

        gl_TessLevelInner[0] = level;
        gl_TessLevelInner[1] = level;
    }
}
