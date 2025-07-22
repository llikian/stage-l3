/***************************************************************************************************
 * @file  get_directions_no_tangent.frag
 * @brief Implementation of the get_directions function for a mesh without tangents
 **************************************************************************************************/

#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_tex_coords;

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

struct Light {
    float intensity;
    vec3 color;
    vec3 position;
};
uniform Light u_light;

uniform vec3 u_camera_position;

void get_directions(out vec3 normal, out vec3 light_direction, out vec3 view_direction) {
    normal = normalize(v_normal);
    light_direction = normalize(u_light.position - v_position);
    view_direction = normalize(u_camera_position - v_position);
}
