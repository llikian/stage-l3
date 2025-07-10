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
    sampler2D albedo_map;
    sampler2D metallic_roughness_map;
    vec4 albedo;
    float metallic;
    float roughness;
    float fresnel0;
};

uniform Material u_material;

float pow2(float x) { return x * x; }
float pow5(float x) { return x * x * x * x * x; }

float lambda(float cosine, float roughness) {
    return sqrt(1.0f + pow2(roughness) * (1.0f / pow2(cosine) - 1.0f));
}

vec3 diffuse_brdf(vec3 albedo, vec3 F) {
    return vec3(INV_PI * (1.0f - F) * albedo.rgb);
}

vec3 specular_brdf(float roughness, vec3 F0, out vec3 F) {
    vec3 normal = normalize(v_normal);

    vec3 light_direction = normalize(u_light_position - v_position);
    vec3 view_direction = normalize(u_camera_position - v_position);
    vec3 halfway_direction = normalize(view_direction + light_direction);

    float cos_theta = max(dot(normal, light_direction), 0.0f);
    float cos_view = max(dot(normal, view_direction), 0.0f);
    float cos_halfway = max(dot(normal, halfway_direction), 0.0f);

    F = F0 + (1.0f - F0) * pow5(1.0f - dot(view_direction, halfway_direction));

    if (cos_theta > 0.0f && cos_view > 0.0f && cos_halfway > 0.0f) {
        float D = INV_PI * pow2(roughness / (1.0f + pow2(cos_halfway) * (pow2(roughness) - 1.0f)));
        float G = 2.0f / (lambda(cos_theta, roughness) * lambda(cos_view, roughness));

        return F * D * G / (4.0f * cos_theta * cos_view);
    } else {
        return vec3(0.0f);
    }
}

vec3 metal_brdf(vec3 albedo, float roughness) {
    vec3 F;
    vec3 specular = specular_brdf(roughness, albedo.rgb, F);

    float cos_theta = max(dot(normalize(v_normal), normalize(u_light_position - v_position)), 0.0f);

    return vec3(specular * u_light_color * cos_theta);
}

vec3 dielectric_brdf(vec3 albedo, float roughness) {
    vec3 F;
    vec3 specular = specular_brdf(roughness, vec3(u_material.fresnel0), F);
    vec3 diffuse = diffuse_brdf(albedo, F);

    float cos_theta = max(dot(normalize(v_normal), normalize(u_light_position - v_position)), 0.0f);

    return vec3((diffuse.rgb + specular) * u_light_color * cos_theta);
}

void main() {
    vec4 albedo = u_material.albedo * texture(u_material.albedo_map, v_tex_coords);
    vec3 metallic_roughness = texture(u_material.metallic_roughness_map, v_tex_coords).rgb;
    float metallic = u_material.metallic * metallic_roughness.b;
    float roughness = pow2(u_material.roughness * metallic_roughness.g);

    frag_color.rgb = mix(dielectric_brdf(albedo.rgb, roughness), metal_brdf(albedo.rgb, roughness), metallic);
    frag_color.a = albedo.a;
}
