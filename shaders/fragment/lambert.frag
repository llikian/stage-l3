/***************************************************************************************************
 * @file  phong.frag
 * @brief Fragment shader implementing a basic lambertian diffuse
 **************************************************************************************************/

#version 460 core

in vec3 v_position;
in vec3 v_normal;

out vec4 frag_color;

uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform vec4 u_color;

void main() {
    float ambient = 0.2f;
    float diffuse = max(dot(normalize(v_normal), normalize(u_light_position - v_position)), 0.0f);

    frag_color = vec4(u_color.rgb * (ambient + diffuse * u_light_color), u_color.a);
}
