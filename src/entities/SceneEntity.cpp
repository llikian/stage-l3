/***************************************************************************************************
 * @file  SceneEntity.cpp
 * @brief Implementation of the SceneEntity class
 **************************************************************************************************/

#include "entities/SceneEntity.hpp"

SceneEntity::SceneEntity(const std::string& name, const std::filesystem::path& path)
    : Entity(name), scene(path) { }

void SceneEntity::draw(const mat4& view_projection_matrix, const Frustum& frustum) const {
    if(is_visible) { draw(view_projection_matrix); }
    for(Entity* child : children) { child->draw(view_projection_matrix, frustum); }
}

void SceneEntity::draw(const mat4& view_projection_matrix) const {
    scene.draw(view_projection_matrix, transform);
}
