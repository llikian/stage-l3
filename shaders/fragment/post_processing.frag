/***************************************************************************************************
 * @file  post_processing.frag
 * @brief Fragment shader for post processing the rendered scene
 **************************************************************************************************/

#version 460 core

in vec2 v_tex_coords;

out vec4 frag_color;

uniform sampler2D u_texture;
uniform vec2 u_texture_resolution;
uniform vec2 u_resolution;

vec2 get_uv() {
    vec2 uv = v_tex_coords * u_resolution / u_texture_resolution;
    return clamp(uv, 0.0f, 1.0f);
}

vec2 get_uv_pixelated(int pixel_size) {
    vec2 uv = pixel_size / u_texture_resolution * floor(v_tex_coords * u_resolution / pixel_size);
    return clamp(uv, 0.0f, 1.0f);
}

void main() {
    vec4 color = texture(u_texture, get_uv());

    frag_color = color;

    frag_color.rgb = pow(frag_color.rgb, vec3(1.0f / 2.2f)); // Gamma Correction
}
