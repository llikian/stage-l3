/***************************************************************************************************
 * @file  Material.cpp
 * @brief Implementation of the Material struct
 **************************************************************************************************/

#include "mesh/Material.hpp"

void Material::update_shader_uniforms(const Shader& shader) const {
    shader.set_uniform("ambient", ambiant);
    shader.set_uniform("diffuse", diffuse);
    shader.set_uniform("specular", specular);
}
