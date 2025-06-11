/***************************************************************************************************
 * @file  default.frag
 * @brief Default fragment shader
 **************************************************************************************************/

#version 460 core

in vec3 position;
in vec3 normal;
in vec2 tex_coords;

out vec4 frag_color;

const vec3 LIGHT_POS = vec3(0.0f, 10.0f, 0.0f);
const vec3 LIGHT_COLOR = vec3(1.0f);

uniform vec3 u_camera_position;

uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform sampler2D u_ambient_map;
uniform sampler2D u_diffuse_map;
uniform vec3 u_specular;
uniform float u_specular_exponent;

void main() {
    vec3 normalized_normal = normalize(normal);
    vec3 light_direction = normalize(LIGHT_POS - position);

    vec3 ambient = u_ambient * texture(u_ambient_map, tex_coords).rgb;

    float diffuse_strength = max(dot(normalized_normal, light_direction), 0.0f);
    vec3 diffuse = diffuse_strength * u_diffuse * texture(u_diffuse_map, tex_coords).rgb;

    vec3 view_direction = normalize(u_camera_position - position);
    vec3 reflected_direction = reflect(-light_direction, normalized_normal);
    float specular_strength = pow(max(dot(view_direction, reflected_direction), 0.0f), u_specular_exponent);
    vec3 specular = specular_strength * u_specular;

    frag_color.xyz = (ambient + diffuse + specular) * LIGHT_COLOR;
    frag_color.w = 1.0f;
}
