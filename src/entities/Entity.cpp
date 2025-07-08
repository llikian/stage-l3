/***************************************************************************************************
 * @file  Entity.cpp
 * @brief Implementation of the Entity class
 **************************************************************************************************/

#include "entities/Entity.hpp"

#include <glad/glad.h>
#include "AssetManager.hpp"
#include "debug.hpp"
#include "entities/DrawableEntity.hpp"
#include "imgui.h"

Entity::Entity(const std::string& name) : name(name), parent(nullptr), is_visible(true) { }

Entity::~Entity() {
    for(const Entity* child : children) { delete child; }
}

void Entity::set_visibility(bool is_visible) {
    this->is_visible = is_visible;
    for(Entity* child : children) { child->set_visibility(is_visible); }
}

bool Entity::get_visibility() const {
    return is_visible;
}

void Entity::toggle_visibility() {
    is_visible = !is_visible;
    for(Entity* child : children) { child->set_visibility(is_visible); }
}

void Entity::update_transform_and_children() {
    if(transform.is_local_model_dirty()) {
        force_update_transform_and_children();
    } else {
        for(Entity* child : children) { child->update_transform_and_children(); }
    }
}

void Entity::force_update_transform_and_children() {
    if(parent != nullptr) {
        transform.update_global_model(parent->transform.get_global_model());
    } else {
        transform.update_global_model();
    }

    for(Entity* child : children) { child->force_update_transform_and_children(); }
}

void Entity::draw_drawables(const mat4& view_projection_matrix, const Frustum& frustum) {
    if(is_drawable()) {
        DrawableEntity::total_drawable_entities++;

        if(is_visible) {
            DrawableEntity::total_not_hidden_entities++;

            DrawableEntity* entity = static_cast<DrawableEntity*>(this);
            if(entity->aabb != nullptr) {
                if(entity->aabb->is_in_frustum(frustum.view_projection * transform.get_global_model())) {
                    DrawableEntity::total_drawn_entities++;
                    entity->draw(view_projection_matrix);

#ifdef DEBUG_SHOW_BOUNDING_BOXES
                    const Shader& bounding_volume_shader = AssetManager::get_shader("flat");
                    bounding_volume_shader.use();
                    bounding_volume_shader.set_uniform("u_mvp", view_projection_matrix
                                                                * entity->aabb->get_global_model_matrix(transform));
                    bounding_volume_shader.set_uniform("u_color", vec4(1.0f, 0.0f, 0.0f, 1.0f));
                    glLineWidth(3.0f);
                    AssetManager::get_mesh("wireframe cube").draw();
                    glLineWidth(1.0f);
#endif
                }
            } else {
                DrawableEntity::total_drawn_entities++;
                entity->draw(view_projection_matrix);
            }
        }
    }

    for(Entity* child : children) { child->draw_drawables(view_projection_matrix, frustum); }
}

bool Entity::is_drawable() const {
    return false;
}

void Entity::add_to_object_editor() {
    ImGui::Text("Selected Entity: '%s'", name.c_str());

    if(ImGui::Checkbox("Is Object Visible", &is_visible)) {
        for(Entity* child : children) { child->set_visibility(is_visible); }
    }

    bool is_dirty = ImGui::DragFloat3("Local Position", &transform.get_local_position_reference().x);

    quaternion& orientation = transform.get_local_orientation_reference();
    if(ImGui::DragFloat4("Local Orientation", &orientation.x, 0.1f)) {
        is_dirty = true;
        orientation.normalize();
    }

    is_dirty = is_dirty || ImGui::DragFloat3("Local Scale", &transform.get_local_scale_reference().x, 0.1f, 0.1f);

    if(is_dirty) { transform.set_local_model_to_dirty(); }
}
