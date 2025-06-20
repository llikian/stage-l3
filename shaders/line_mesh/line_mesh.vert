/***************************************************************************************************
 * @file  line_mesh.vert
 * @brief Vertex shader for rendering meshes made out of lines
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;

out vec3 v_color;

uniform mat4 u_mvp;

void main() {
    gl_Position = u_mvp * vec4(a_position, 1.0f);
    v_color = a_color;
}
