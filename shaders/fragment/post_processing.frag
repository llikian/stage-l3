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

vec3 desaturate(vec3 color) {
    return vec3(0.299 * color.r + 0.587 * color.g + 0.114 * color.b);
}

vec3 sepia(vec3 color) {
    return vec3((color.r * 0.393f) + (color.g * 0.769f) + (color.b * 0.189f),
                (color.r * 0.349f) + (color.g * 0.686f) + (color.b * 0.168f),
                (color.r * 0.272f) + (color.g * 0.534f) + (color.b * 0.131f));
}

void main() {
    vec4 tex = texture(u_texture, get_uv());
//    vec4 tex = texture(u_texture, get_uv_pixelated(8));
    vec3 color = tex.rgb;

    color = pow(color, vec3(1.0f / 2.2f)); // Gamma Correction

    frag_color = vec4(color, tex.a);
}
