/***************************************************************************************************
 * @file  Material.cpp
 * @brief Implementation of the Material struct
 **************************************************************************************************/

#include "mesh/Material.hpp"

Material::Material()
    : ambient(1.0f), diffuse(1.0f), specular(1.0f), specular_exponent(10.0f) { }

void Material::update_shader_uniforms(const Shader& shader) const {
    shader.set_uniform("u_ambient", ambient);
    shader.set_uniform("u_diffuse", diffuse);
    shader.set_uniform("u_specular", specular);
    shader.set_uniform("u_specular_exponent", specular_exponent);

    static Texture default_ambient_map(255, 255, 255);
    static Texture default_diffuse_map(255, 255, 255);

    if(ambient_map.is_default_texture()) {
        default_ambient_map.bind(0);
    } else {
        ambient_map.bind(0);
    }

    if(diffuse_map.is_default_texture()) {
        default_diffuse_map.bind(1);
    } else {
        diffuse_map.bind(1);
    }
}
