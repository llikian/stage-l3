/***************************************************************************************************
 * @file  position_only-no_mvp.vert
 * @brief Vertex shader that only handles positions and doesn't apply an mvp matrix
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 a_position;

void main() {
    gl_Position = vec4(a_position, 1.0f);
}
