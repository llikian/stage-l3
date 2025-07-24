/***************************************************************************************************
 * @file  PhongMaterial.cpp
 * @brief Implementation of the PhongMaterial struct
 **************************************************************************************************/

#include "materials/PhongMaterial.hpp"

#include "assets/AssetManager.hpp"
#include "imgui.h"

PhongMaterial::PhongMaterial(const std::string& name)
    : Material(name),
      ambient(1.0f),
      diffuse(1.0f),
      specular(1.0f),
      specular_exponent(10.0f) { }

void PhongMaterial::update_shader_uniforms(const Shader* shader) const {
    shader->set_uniform("u_ambient", ambient);
    shader->set_uniform("u_diffuse", diffuse);
    shader->set_uniform("u_specular", specular);
    shader->set_uniform("u_specular_exponent", specular_exponent);

    diffuse_map.bind(0);
}

bool PhongMaterial::has_transparency() const {
    return diffuse_map.has_transparency();
}

void PhongMaterial::add_to_object_editor() {
    ImGui::Text("Material: %s", name.c_str());
    ImGui::ColorEdit3("Ambient Color", &ambient.x);
    ImGui::ColorEdit3("Diffuse Color", &diffuse.x);
    ImGui::ColorEdit3("Specular Color", &specular.x);
    ImGui::DragFloat("Specular Exponent", &specular_exponent);
}
