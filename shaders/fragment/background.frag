/***************************************************************************************************
 * @file  phong.frag
 * @brief Fragment shader for rendering a background with two sky colors being mixed depending on
 * the height
 **************************************************************************************************/

#version 460 core

out vec4 frag_color;

const vec3 world_up = vec3(0.0f, 1.0f, 0.0f);

uniform vec2 u_resolution;

uniform vec3 u_camera_direction;
uniform vec3 u_camera_right;
uniform vec3 u_camera_up;

uniform vec3 u_sky_color_low = vec3(0.671f, 0.851f, 1.0f);
uniform vec3 u_sky_color_high = vec3(0.239f, 0.29f, 0.761f);

void main() {
    vec2 uv = (2.0f * gl_FragCoord.xy - u_resolution) / u_resolution.y;
    vec3 direction = normalize(u_camera_direction + uv.x * u_camera_right + uv.y * u_camera_up);
    frag_color.rgb = mix(u_sky_color_low, u_sky_color_high, 0.5f + 0.5f * dot(direction, world_up));
    frag_color.a = 1.0f;
}
