/***************************************************************************************************
 * @file  position_and_normal.vert
 * @brief Vertex shader that handles 2 vertex attributes: positions and normals
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

out vec3 v_position;
out vec3 v_normal;

uniform mat4 u_mvp;
uniform mat4 u_model;
uniform mat3 u_normals_model_matrix;

void main() {
    vec4 pos = vec4(a_position, 1.0f);

    gl_Position = u_mvp * pos;

    v_position = (u_model * pos).xyz;
    v_normal = normalize(u_normals_model_matrix * a_normal);
}
