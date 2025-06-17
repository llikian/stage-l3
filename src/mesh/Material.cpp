/***************************************************************************************************
 * @file  Material.cpp
 * @brief Implementation of the Material struct
 **************************************************************************************************/

#include "mesh/Material.hpp"

Material::Material()
    : ambient(1.0f),
      diffuse(1.0f),
      specular(1.0f),
      specular_exponent(10.0f) { }

void Material::update_shader_uniforms(const Shader& shader) {
    shader.set_uniform("u_ambient", ambient);
    shader.set_uniform("u_diffuse", diffuse);
    shader.set_uniform("u_specular", specular);
    shader.set_uniform("u_specular_exponent", specular_exponent);

    if(ambient_map.is_default_texture()) {
        ambient_map.create(255, 255, 255);
    } else {
        ambient_map.bind(0);
    }

    if(diffuse_map.is_default_texture()) {
        diffuse_map.create(255, 255, 255);
    } else {
        diffuse_map.bind(1);
    }
}
