/***************************************************************************************************
 * @file  default.vert
 * @brief Default vertex shader
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

out vec3 normal;

uniform mat4 mvp;
uniform mat3 normals_model_matrix;

void main() {
    gl_Position = mvp * vec4(a_position, 1.0f);
    normal = normalize(normals_model_matrix * a_normal);
}
