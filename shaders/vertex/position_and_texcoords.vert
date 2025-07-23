/***************************************************************************************************
 * @file  position_and_texcoords.vert
 * @brief Vertex shader that handles 2 vertex attributes: positions and texture coordinates
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 2) in vec2 a_tex_coords;

out vec2 v_tex_coords;

uniform mat4 u_mvp;

void main() {
    gl_Position = u_mvp * vec4(a_position, 1.0f);
    v_tex_coords = a_tex_coords;
}
