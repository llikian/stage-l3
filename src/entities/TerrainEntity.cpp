/***************************************************************************************************
 * @file  TerrainEntity.cpp
 * @brief Implementation of the TerrainEntity class
 **************************************************************************************************/

#include "entities/TerrainEntity.hpp"

#include "imgui.h"

TerrainEntity::TerrainEntity(const std::string& name,
                             const Shader& shader,
                             float chunk_size,
                             unsigned int chunks_on_line)
    : DrawableEntity(name, shader), terrain(shader, chunk_size, chunks_on_line) { }

void TerrainEntity::draw(const mat4& view_projection_matrix) {
    terrain.draw(view_projection_matrix);
}

void TerrainEntity::add_to_object_editor() {
    ImGui::Text("Selected Entity: '%s'", name.c_str());

    if(ImGui::Checkbox("Is Object Visible", &is_visible)) {
        for(Entity* child : children) { child->set_visibility(is_visible); }
    }
}

void TerrainEntity::create_aabb() {
    // TODO: aabb for terrain
}
