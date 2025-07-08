/***************************************************************************************************
 * @file  FlatShadedMeshEntity.cpp
 * @brief Implementation of the FlatShadedMeshEntity class
 **************************************************************************************************/

#include "entities/FlatShadedMeshEntity.hpp"

#include "imgui.h"

FlatShadedMeshEntity::FlatShadedMeshEntity(const std::string& name,
                                           const Shader& shader,
                                           Mesh& mesh,
                                           const vec4& color)
    : MeshEntity(name, shader, mesh), color(color) { }

void FlatShadedMeshEntity::update_uniforms(const mat4& view_projection_matrix) {
    MeshEntity::update_uniforms(view_projection_matrix);
    shader.set_uniform_if_exists("u_color", color);
}

void FlatShadedMeshEntity::add_to_object_editor() {
    MeshEntity::add_to_object_editor();
    ImGui::ColorEdit4("Object color", &color.x);
}
