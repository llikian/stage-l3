/***************************************************************************************************
 * @file  terrain.tesc
 * @brief Tesselation control shader used for rendering a terrain
 **************************************************************************************************/

#version 460 core

layout (vertices = 4) out;

uniform mat4 u_frustum_view_projection_matrix;
uniform float u_chunk_size;

struct Noise {
    float frequency;
    float amplitude;
    float height;
};

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

vec4 get_position(vec2 pos) {
    return vec4(pos.x, get_height(pos), pos.y, 1.0f);
}

bool is_in_frustum(vec4 point[4]) {
    uint points_on_or_above_plane[6] = { 0, 0, 0, 0, 0, 0 };

    for(uint i = 0 ; i < 4 ; ++i) {
        vec4 p = u_frustum_view_projection_matrix * point[i];
        if(p.x < -p.w) { ++points_on_or_above_plane[0]; }
        if(p.x > p.w) { ++points_on_or_above_plane[1]; }
        if(p.y < -p.w) { ++points_on_or_above_plane[2]; }
        if(p.y > p.w) { ++points_on_or_above_plane[3]; }
        if(p.z < -p.w) { ++points_on_or_above_plane[4]; }
        if(p.z > p.w) { ++points_on_or_above_plane[5]; }
    }

    for(uint i = 0 ; i < 6 ; ++i) {
        if(points_on_or_above_plane[i] == 4) { return false; }
    }

    return true;
}

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if(gl_InvocationID == 0) {
        vec4 points[4] = {
            get_position(gl_in[0].gl_Position.xz),
            get_position(gl_in[1].gl_Position.xz),
            get_position(gl_in[2].gl_Position.xz),
            get_position(gl_in[3].gl_Position.xz)
        };

        float level = is_in_frustum(points) ? 16.0f : 0.0f;

        gl_TessLevelOuter[0] = level;
        gl_TessLevelOuter[1] = level;
        gl_TessLevelOuter[2] = level;
        gl_TessLevelOuter[3] = level;

        gl_TessLevelInner[0] = level;
        gl_TessLevelInner[1] = level;
    }
}
