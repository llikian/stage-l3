/***************************************************************************************************
 * @file  terrain.frag
 * @brief Tesselation evaluation shader used for rendering a terrain
 **************************************************************************************************/

#version 460 core

layout (quads) in;

uniform mat4 u_view_projection;

vec3 get_position(vec2 tess_coord) {
    vec2 top = mix(gl_in[0].gl_Position.xz, gl_in[3].gl_Position.xz, tess_coord.x);
    vec2 bottom = mix(gl_in[1].gl_Position.xz, gl_in[2].gl_Position.xz, tess_coord.x);
    return vec3(mix(top, bottom, tess_coord.y), 0.0f).xzy;
}

void main() {
    gl_Position = u_view_projection * vec4(get_position(gl_TessCoord.xy), 1.0f);
}
