/***************************************************************************************************
 * @file  get_directions_tangent.frag
 * @brief Implementation of the get_directions function for a mesh with tangents
 **************************************************************************************************/

#version 460 core

in vec2 v_tex_coords;
in vec3 v_tangent_light_position;
in vec3 v_tangent_view_position;
in vec3 v_tangent_position;

struct Material {
    vec4 base_color;
    sampler2D base_color_map;
    float metallic;
    float roughness;
    sampler2D metallic_roughness_map;
    float reflectance;
    sampler2D normal_map;
};

uniform Material u_material;

void get_directions(out vec3 normal, out vec3 light_direction, out vec3 view_direction) {
    normal = normalize(texture(u_material.normal_map, v_tex_coords).rgb * 2.0f - 1.0f);
    light_direction = normalize(v_tangent_light_position - v_tangent_position);
    view_direction = normalize(v_tangent_view_position - v_tangent_position);
}
