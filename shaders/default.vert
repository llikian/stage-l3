/***************************************************************************************************
 * @file  default.vert
 * @brief Default vertex shader
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

out vec3 position;
out vec3 normal;
out vec2 tex_coords;

uniform mat4 u_mvp;
uniform mat4 u_model;
uniform mat3 u_normals_model_matrix;

void main() {
    vec4 pos = vec4(a_position, 1.0f);

    gl_Position = u_mvp * pos;

    position = (u_model * pos).xyz;
    normal = normalize(u_normals_model_matrix * a_normal);
    tex_coords = a_tex_coords;
}
