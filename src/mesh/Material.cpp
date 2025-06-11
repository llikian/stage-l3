/***************************************************************************************************
 * @file  Material.cpp
 * @brief Implementation of the Material struct
 **************************************************************************************************/

#include "mesh/Material.hpp"

void Material::update_shader_uniforms(const Shader& shader) const {
    shader.set_uniform("u_ambient", ambient);
    shader.set_uniform("u_diffuse", diffuse);
    shader.set_uniform("u_specular", specular);
    shader.set_uniform("u_specular_exponent", specular_exponent);

    ambient_map.bind(0);
    diffuse_map.bind(1);
}
