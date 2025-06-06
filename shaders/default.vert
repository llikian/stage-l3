/***************************************************************************************************
 * @file  default.vert
 * @brief Default vertex shader
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;

uniform mat4 mvp;
uniform mat3 normalModel;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0f);
    normal = normalize(normalModel * aNormal);
}
