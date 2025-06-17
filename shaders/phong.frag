/***************************************************************************************************
 * @file  phong.frag
 * @brief Fragment shader implementing the phong lighting model
 **************************************************************************************************/

#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_tex_coords;

out vec4 frag_color;

uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform vec3 u_camera_position;

uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_specular_exponent;
uniform sampler2D u_ambient_map;
uniform sampler2D u_diffuse_map;

void main() {
    vec4 diffuse_map = texture(u_diffuse_map, v_tex_coords);

    frag_color.a = diffuse_map.a;
    if (frag_color.a < 0.2f) { discard; }

    vec3 normal = normalize(v_normal);
    vec3 light_direction = normalize(u_light_position - v_position);

    vec4 ambient_map = texture(u_ambient_map, v_tex_coords);
    float ambient_strength = 0.2f;
    vec3 ambient = ambient_strength * u_ambient * ambient_map.rgb;

    float diffuse_strength = max(dot(normal, light_direction), 0.0f);
    vec3 diffuse = diffuse_strength * u_diffuse * diffuse_map.rgb;

    vec3 view_direction = normalize(u_camera_position - v_position);
    vec3 reflected_direction = reflect(-light_direction, normal);
    float specular_strength = pow(max(dot(view_direction, reflected_direction), 0.0f), u_specular_exponent);
    vec3 specular = specular_strength * u_specular;

    frag_color.rgb = (ambient + diffuse + specular) * u_light_color;
}
