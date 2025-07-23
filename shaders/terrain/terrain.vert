/***************************************************************************************************
 * @file  terrain.vert
 * @brief Vertex shader used for rendering a terrain
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec2 a_position;

void main() {
    gl_Position = vec4(a_position.x, 0.0f, a_position.y, 1.0f);
}
