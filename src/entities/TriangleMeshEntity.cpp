/***************************************************************************************************
 * @file  TriangleMeshEntity.cpp
 * @brief Implementation of the TriangleMeshEntity class
 **************************************************************************************************/

#include "entities/TriangleMeshEntity.hpp"

#include "imgui.h"

TriangleMeshEntity::TriangleMeshEntity(const std::string& name, const Shader* shader)
    : DrawableEntity(name, shader) { }

void TriangleMeshEntity::draw(const mat4& view_projection_matrix) {
    if(shader != nullptr) {
        shader->use();
        update_uniforms(view_projection_matrix);
        mesh.draw(*shader);
    } else {
        std::cout << "[WARNING] TriangleMeshEntity '" << name << "' with nullptr shader.\n";
    }
}

void TriangleMeshEntity::add_to_object_editor() {
    DrawableEntity::add_to_object_editor();
    if(mesh.material != nullptr) {
        ImGui::Text("Material: %s", mesh.material->name.c_str());
        ImGui::ColorEdit3("Ambient Color", &mesh.material->ambient.x);
        ImGui::ColorEdit3("Diffuse Color", &mesh.material->diffuse.x);
        ImGui::ColorEdit3("Specular Color", &mesh.material->specular.x);
        ImGui::DragFloat("Specular Exponent", &mesh.material->specular_exponent);
    } else {
        ImGui::Text("Mesh doesn't have a material.");
    }
}

void TriangleMeshEntity::create_aabb() {
    vec3 min(std::numeric_limits<float>::max());
    vec3 max(std::numeric_limits<float>::lowest());
    mesh.get_min_max_axis_aligned_coordinates(min, max);
    bounding_volume = new AABB(min, max);
}