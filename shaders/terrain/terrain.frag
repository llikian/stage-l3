/***************************************************************************************************
 * @file  terrain.frag
 * @brief Fragment shader used for rendering a terrain
 **************************************************************************************************/

#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_tess_coord;

out vec4 frag_color;

#define LIGHT_DIRECTION vec3(1.0f, 1.0f, 1.0f)

void main() {
    vec3 color = vec3(0.929f, 0.878f, 0.82f);
    if(v_tess_coord.x < 0.05f || v_tess_coord.y < 0.05f || v_tess_coord.x > 0.95f || v_tess_coord.y > 0.95f) {
        color = vec3(0.29f, 0.267f, 0.243f);
    }

    float ambient = 0.2f;

    vec3 normal = normalize(v_normal);
    vec3 light_direction = normalize(LIGHT_DIRECTION);
    float diffuse = max(dot(normal, light_direction), 0.0f);

    frag_color = vec4(ambient + diffuse * color, 1.0f);
}
