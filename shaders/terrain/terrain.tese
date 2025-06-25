/***************************************************************************************************
 * @file  terrain.frag
 * @brief Tesselation evaluation shader used for rendering a terrain
 **************************************************************************************************/

#version 460 core

layout (quads) in;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_tess_coord;

uniform mat4 u_view_projection;
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

vec3 get_position(vec2 tess_coord) {
    vec2 p0 = mix(gl_in[0].gl_Position.xz, gl_in[3].gl_Position.xz, tess_coord.x);
    vec2 p1 = mix(gl_in[1].gl_Position.xz, gl_in[2].gl_Position.xz, tess_coord.x);
    vec2 position = mix(p0, p1, tess_coord.y);
    return vec3(position.x, get_height(position), position.y);
}

void main() {
    float delta = u_chunk_size / gl_TessLevelInner[0];
    vec3 p0 = get_position(vec2(gl_TessCoord.x + delta, gl_TessCoord.y));
    vec3 p1 = get_position(vec2(gl_TessCoord.x, gl_TessCoord.y + delta));

    v_position = get_position(gl_TessCoord.xy);
    v_normal = normalize(cross(p0 - v_position, p1 - v_position));
    v_tess_coord = gl_TessCoord.xy;

    gl_Position = u_view_projection * vec4(v_position, 1.0f);
}
