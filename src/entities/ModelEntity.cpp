/***************************************************************************************************
 * @file  ModelEntity.cpp
 * @brief Implementation of the ModelEntity class
 **************************************************************************************************/

#include "entities/ModelEntity.hpp"

#include "AssetManager.hpp"
#include "glad/glad.h"
#include "imgui.h"

ModelEntity::ModelEntity(const std::string& name, const Shader& shader, Model& model)
    : DrawableEntity(name, shader), model(model) { }

void ModelEntity::draw(const mat4& view_projection_matrix) {
    shader.use();
    update_uniforms(view_projection_matrix);
    model.draw(shader);
}

void ModelEntity::add_to_object_editor() {
    DrawableEntity::add_to_object_editor();

    static Material* selected = nullptr;

    ImGui::BeginTable("Materials", 1, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("Materials");
    ImGui::TableHeadersRow();
    for(Material& material : model.materials) {
        ImGui::TableNextColumn();
        if(ImGui::Selectable(material.name.c_str(), &material == selected)) { selected = &material; }
    }
    ImGui::EndTable();

    if(selected != nullptr) {
        ImGui::Text("Material: %s", selected->name.c_str());
        ImGui::ColorEdit3("Ambient Color", &selected->ambient.x);
        ImGui::ColorEdit3("Diffuse Color", &selected->diffuse.x);
        ImGui::ColorEdit3("Specular Color", &selected->specular.x);
        ImGui::DragFloat("Specular Exponent", &selected->specular_exponent);
    }
}

void ModelEntity::create_aabb() {
    vec3 min(std::numeric_limits<float>::max());
    vec3 max(std::numeric_limits<float>::lowest());
    model.get_min_max_axis_aligned_coordinates(min, max);
    aabb = new AABB(min, max);
}
