/***************************************************************************************************
 * @file  MeshEntity.cpp
 * @brief Implementation of the MeshEntity class
 **************************************************************************************************/

#include "entities/MeshEntity.hpp"

#include "imgui.h"

MeshEntity::MeshEntity(const std::string& name, const Shader& shader, Mesh& mesh)
    : DrawableEntity(name, shader), mesh(mesh), material(nullptr) { }

void MeshEntity::draw(const mat4& view_projection_matrix) const {
    shader->use();
    update_uniforms(view_projection_matrix);
    mesh.draw();
}

void MeshEntity::update_uniforms(const mat4& view_projection_matrix) const {
    DrawableEntity::update_uniforms(view_projection_matrix);
    if(material != nullptr) { material->update_shader_uniforms(*shader); }
}

void MeshEntity::add_to_object_editor() {
    DrawableEntity::add_to_object_editor();

    ImGui::NewLine();
    if(material != nullptr) {
        material->add_to_object_editor();
    } else {
        ImGui::Text("Mesh doesn't have a material.");
    }
}

void MeshEntity::create_aabb() {
    vec3 min(std::numeric_limits<float>::max());
    vec3 max(std::numeric_limits<float>::lowest());
    mesh.get_min_max_axis_aligned_coordinates(min, max);
    aabb = new AABB(min, max);
}
