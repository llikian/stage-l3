/***************************************************************************************************
 * @file  position_and_texcoords_no_mvp.vert
 * @brief Vertex shader that handles 2 vertex attributes: positions and texture coordinates and
 * doesn't apply an mvp matrix
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 2) in vec2 a_tex_coords;

out vec2 v_tex_coords;

void main() {
    gl_Position = vec4(a_position, 1.0f);
    v_tex_coords = a_tex_coords;
}
