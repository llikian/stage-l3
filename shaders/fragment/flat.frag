/***************************************************************************************************
 * @file  flat.frag
 * @brief Fragment shader that renders a mesh with a flat color
 **************************************************************************************************/

#version 460 core

out vec4 frag_color;

uniform vec4 u_color;

void main() {
    frag_color = u_color;
}
