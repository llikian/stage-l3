/***************************************************************************************************
 * @file  metallic_roughness.frag
 * @brief Fragment shader implementing the metallic-roughness shading model
 **************************************************************************************************/

#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_tex_coords;

out vec4 frag_color;

const float PI = 3.141592653589793f;
const float INV_PI = 0.318309886183790f;

uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform vec3 u_camera_position;

struct Material {
    vec4 base_color;
    sampler2D base_color_map;
    float metallic;
    float roughness;
    sampler2D metallic_roughness_map;
    float fresnel0;
};

uniform Material u_material;

float pow2(float x) { return x * x; }
float pow5(float x) { return x * x * x * x * x; }

vec3 F_Schlick(vec3 F0, float light_dot_halfway) {
    return F0 + (1.0f - F0) * pow5(1.0f - light_dot_halfway);
}

float D_GGX(float normal_dot_halfway, float roughness) {
    float roughness2 = roughness * roughness;
    return roughness2 * INV_PI / pow2(pow2(normal_dot_halfway) * (roughness2 - 1.0f) + 1.0);
}

float V_Smith_GGX_correlated(float normal_dot_view, float normal_dot_light, float roughness) {
    float roughness2 = roughness * roughness;
    float view_GGX = normal_dot_light * sqrt(roughness2 + (1.0f - roughness2) * normal_dot_view);
    float light_GGX = normal_dot_view * sqrt(roughness2 + (1.0f - roughness2) * normal_dot_light);
    return 0.5f / (view_GGX + light_GGX);
}

vec3 brdf(vec3 base_color, float metallic, float roughness) {
    vec3 normal = normalize(v_normal);

    vec3 light_direction = normalize(u_light_position - v_position);
    vec3 view_direction = normalize(u_camera_position - v_position);
    vec3 halfway_direction = normalize(view_direction + light_direction);

    float normal_dot_light = max(dot(normal, light_direction), 0.0f);
    float normal_dot_view = max(dot(normal, view_direction), 0.0f);
    float normal_dot_halfway = max(dot(normal, halfway_direction), 0.0f);

    vec3 F0 = mix(vec3(u_material.fresnel0), base_color, metallic);

    vec3 F = F_Schlick(F0, max(dot(view_direction, halfway_direction), 0.0f));
    float D = D_GGX(normal_dot_halfway, roughness);
    float V = V_Smith_GGX_correlated(normal_dot_view, normal_dot_light, roughness);

    vec3 specular = (D * V) * F;
    vec3 diffuse = (INV_PI * (1.0f - metallic) * base_color) * (1.0f - F);

    return (diffuse + specular) * u_light_color;
}

void main() {
    vec4 base_color = u_material.base_color * texture(u_material.base_color_map, v_tex_coords);
    vec3 metallic_roughness = texture(u_material.metallic_roughness_map, v_tex_coords).rgb;
    float metallic = u_material.metallic * metallic_roughness.b;
    float roughness = u_material.roughness * metallic_roughness.g;

    frag_color.rgb = brdf(base_color.rgb, metallic, roughness);
    frag_color.a = base_color.a;
}
