/***************************************************************************************************
 * @file  default.frag
 * @brief Default fragment shader
 **************************************************************************************************/

#version 460 core

in vec3 normal;

out vec4 fragColor;

const vec3 LIGHT_POS = vec3(10.0f);
const vec3 LIGHT_COLOR = vec3(1.0f);

void main() {
    float ambient = 0.2f;
    float diffuse = max(dot(normalize(normal), normalize(LIGHT_POS)), 0.0f);

    fragColor = vec4((ambient + diffuse) * LIGHT_COLOR, 1.0f);
}
