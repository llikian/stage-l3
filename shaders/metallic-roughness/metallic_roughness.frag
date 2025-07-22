/***************************************************************************************************
 * @file  metallic_roughness.frag
 * @brief Fragment shader implementing the metallic-roughness shading model
 **************************************************************************************************/

#version 460 core

in vec2 v_tex_coords;

out vec4 frag_color;

const float PI = 3.141592653589793f;
const float INV_PI = 0.318309886183790f;

uniform bool u_test1;

//uniform samplerCube u_cubemap;

struct Light {
    float intensity;
    vec3 color;
    vec3 position;
};

uniform Light u_light;

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

// Needs to be defined in another .frag file.
void get_directions(out vec3 normal, out vec3 light_direction, out vec3 view_direction);

float pow2(float x) { return x * x; }
float pow5(const float x) {
    float x2 = x * x;
    return x2 * x2 * x;
}

vec3 F_Schlick(vec3 F0, float light_dot_halfway) {
    float f = pow5(1.0f - light_dot_halfway);
    return f + (1.0f - f) * F0;
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

float diffuse_lambert() {
    return INV_PI;
}

vec3 brdf(vec3 base_color, float metallic, float roughness) {
    vec3 normal;
    vec3 light_direction;
    vec3 view_direction;
    get_directions(normal, light_direction, view_direction);

    vec3 halfway_direction = normalize(view_direction + light_direction);

    float normal_dot_light = max(dot(normal, light_direction), 0.0f);
    float normal_dot_view = max(dot(normal, view_direction), 0.0f);
    float normal_dot_halfway = max(dot(normal, halfway_direction), 0.0f);

    vec3 F0 = mix(vec3(0.16f * pow2(u_material.reflectance)), base_color, metallic);

    vec3 F = F_Schlick(F0, max(dot(light_direction, halfway_direction), 0.0f));
    float D = D_GGX(normal_dot_halfway, roughness);
    float V = V_Smith_GGX_correlated(normal_dot_view, normal_dot_light, roughness);
    vec3 specular = D * V * F;

    vec3 diffuse_color = (1.0f - F) * (1.0f - metallic) * base_color;
    vec3 diffuse = diffuse_lambert() * diffuse_color;

    vec3 illuminance = normal_dot_light * u_light.intensity * u_light.color;

    return (diffuse + specular) * illuminance;
}

void main() {
    vec4 base_color = u_material.base_color * texture(u_material.base_color_map, v_tex_coords);

    frag_color.a = base_color.a;
    if (frag_color.a < 0.2f) { discard; }

    vec2 metallic_roughness = texture(u_material.metallic_roughness_map, v_tex_coords).bg;
    float metallic = u_material.metallic * metallic_roughness.x;
    float roughness = u_material.roughness * metallic_roughness.y;
    roughness = max(roughness * roughness, 0.01f);

    frag_color.rgb = brdf(base_color.rgb, metallic, roughness);
}
