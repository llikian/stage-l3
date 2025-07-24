/***************************************************************************************************
 * @file  MRMaterial.cpp
 * @brief Implementation of the MRMaterial struct
 **************************************************************************************************/

#include "materials/MRMaterial.hpp"

#include "glad/glad.h"
#include "imgui.h"

MRMaterial::MRMaterial(const std::string& name)
    : Material(name),
      metallic(0.0f), // Dielectric
      roughness(0.5f),
      reflectance(0.5f) // Index of Refraction = 1.5f, 4% reflectance
{ }

void MRMaterial::update_shader_uniforms(const Shader* shader) const {
    base_color_map.bind(0);
    metallic_roughness_map.bind(1);
    normal_map.bind(2);

    shader->set_uniform_if_exists("u_material.base_color", base_color);
    shader->set_uniform_if_exists("u_material.metallic", metallic);
    shader->set_uniform_if_exists("u_material.roughness", roughness);
    shader->set_uniform_if_exists("u_material.reflectance", reflectance);
}

bool MRMaterial::has_transparency() const {
    return base_color.w < 1.0f || base_color_map.has_transparency();
}

void MRMaterial::add_to_object_editor() {
    ImGui::Image(base_color_map.get_id(), ImVec2(128.0f, 128.0f));
    ImGui::SameLine();
    ImGui::Image(metallic_roughness_map.get_id(), ImVec2(128.0f, 128.0f));
    ImGui::SameLine();
    ImGui::Image(normal_map.get_id(), ImVec2(128.0f, 128.0f));
    ImGui::ColorEdit4("base_color", &base_color.x);
    ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
    ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
    ImGui::SliderFloat("Reflectance", &reflectance, 0.35f, 0.56f);
}
