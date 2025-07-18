/***************************************************************************************************
 * @file  Material.cpp
 * @brief Implementation of the Material struct
 **************************************************************************************************/

#include "mesh/Material.hpp"

#ifdef DEBUG
#include "debug.hpp"
#endif

Material::Material(const std::string& name)
    : name(name),
      ambient(1.0f),
      diffuse(1.0f),
      specular(1.0f),
      specular_exponent(10.0f) { }

void Material::update_shader_uniforms(const Shader& shader) {
    shader.set_uniform("u_ambient", ambient);
    shader.set_uniform("u_diffuse", diffuse);
    shader.set_uniform("u_specular", specular);
    shader.set_uniform("u_specular_exponent", specular_exponent);

    if(diffuse_map.is_default_texture()) { diffuse_map.create(255, 255, 255); }
    diffuse_map.bind(0);
}

bool Material::has_transparency() const {
    return diffuse_map.has_transparency();
}
