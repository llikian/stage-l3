/***************************************************************************************************
 * @file  tangent.vert
 * @brief Vertex shader that handles 4 vertex attributes: positions, normals, texture coordinates
 * and tangents.
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;
layout (location = 4) in vec4 a_tangent;

out vec2 v_tex_coords;
out vec3 v_tangent_light_position;
out vec3 v_tangent_view_position;
out vec3 v_tangent_position;

uniform mat4 u_mvp;
uniform mat4 u_model;
uniform mat3 u_normals_model_matrix;

struct Light {
    float intensity;
    vec3 color;
    vec3 position;
};

uniform Light u_light;
uniform vec3 u_camera_position;

void main() {
    vec4 pos = vec4(a_position, 1.0f);

    gl_Position = u_mvp * pos;

    vec3 position = (u_model * pos).xyz;
    v_tex_coords = a_tex_coords;

    vec3 normal = normalize(vec3(u_normals_model_matrix * a_normal));
    vec3 tangent = normalize(vec3(u_normals_model_matrix * a_tangent.xyz));
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = a_tangent.w * cross(normal, tangent);
    mat3 TBN = transpose(mat3(tangent, bitangent, normal));
    v_tangent_light_position = TBN * u_light.position;
    v_tangent_view_position = TBN * u_camera_position;
    v_tangent_position = TBN * position;
}
