/***************************************************************************************************
 * @file  point_mesh.frag
 * @brief Fragment shader for rendering meshes made out of points
 **************************************************************************************************/

#version 460 core

in vec3 v_color;

out vec4 frag_color;

void main() {
    frag_color = vec4(v_color, 1.0f);
}
