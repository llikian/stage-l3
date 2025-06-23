/***************************************************************************************************
 * @file  Entity.cpp
 * @brief Implementation of the Entity class
 **************************************************************************************************/

#include "scene_graph/Entity.hpp"

#include "imgui.h"

Entity::Entity(const std::string& name) : name(name), parent(nullptr) { }

Entity::~Entity() {
    for(const Entity* child : children) { delete child; }
}

void Entity::update_transform_and_children() {
    if(transform.is_local_model_dirty()) {
        if(parent != nullptr) {
            transform.update_global_model(parent->transform.get_global_model());
        } else {
            transform.update_global_model();
        }
    }

    for(Entity* child : children) {
        child->update_transform_and_children();
    }
}

void Entity::draw_drawables(const mat4& view_projection_matrix) {
    if(is_drawable()) { static_cast<DrawableEntity*>(this)->draw(view_projection_matrix); }
    for(Entity* child : children) { child->draw_drawables(view_projection_matrix); }
}

bool Entity::is_drawable() const {
    return false;
}

void Entity::add_to_object_editor() {
    ImGui::Text("Selected Entity: '%s'", name.c_str());

    bool is_dirty = ImGui::InputFloat3("Local Position", &transform.get_local_position_reference().x);
    is_dirty = is_dirty || ImGui::InputFloat3("Local Orientation", &transform.get_local_orientation_reference().x);
    is_dirty = is_dirty || ImGui::InputFloat3("Local Scale", &transform.get_local_scale_reference().x);

    if(is_dirty) { transform.set_local_model_to_dirty(); }
}

DrawableEntity::DrawableEntity(const std::string& name, Shader* shader)
    : Entity(name), shader(shader), is_hidden(false) { }

void DrawableEntity::update_uniforms(const mat4& view_projection_matrix) {
    int u_mvp_location = shader->get_uniform_location("u_mvp");
    if(u_mvp_location != -1) {
        Shader::set_uniform(u_mvp_location, view_projection_matrix * transform.get_global_model());
    }

    const mat4& global_model = transform.get_global_model_reference();
    shader->set_uniform_if_exists("u_model", global_model);

    int u_normals_model_matrix_location = shader->get_uniform_location("u_normals_model_matrix");
    if(u_normals_model_matrix_location != -1) {
        Shader::set_uniform(u_normals_model_matrix_location, transpose_inverse(global_model));
    }
}

bool DrawableEntity::is_drawable() const {
    return true;
}

void DrawableEntity::add_to_object_editor() {
    ImGui::Checkbox("Is Object Hidden", &is_hidden);
    Entity::add_to_object_editor();
}

ModelEntity::ModelEntity(const std::string& name, Shader* shader, const std::filesystem::path& path)
    : DrawableEntity(name, shader), model(path) { }

void ModelEntity::draw(const mat4& view_projection_matrix) {
    if(is_hidden) { return; }

    if(shader != nullptr) {
        shader->use();
        update_uniforms(view_projection_matrix);
        model.draw(*shader);
    } else {
        std::cout << "[WARNING] ModelEntity '" << name << "' with nullptr shader.\n";
    }
}

TriangleMeshEntity::TriangleMeshEntity(const std::string& name, Shader* shader)
    : DrawableEntity(name, shader) { }

void TriangleMeshEntity::draw(const mat4& view_projection_matrix) {
    if(is_hidden) { return; }

    if(shader != nullptr) {
        shader->use();
        update_uniforms(view_projection_matrix);
        mesh.draw(*shader);
    } else {
        std::cout << "[WARNING] TriangleMeshEntity '" << name << "' with nullptr shader.\n";
    }
}

FlatShadedMeshEntity::FlatShadedMeshEntity(const std::string& name, Shader* shader, const vec3& color)
    : TriangleMeshEntity(name, shader), color(color) { }

void FlatShadedMeshEntity::update_uniforms(const mat4& view_projection_matrix) {
    TriangleMeshEntity::update_uniforms(view_projection_matrix);
    shader->set_uniform_if_exists("u_color", color);
}

void FlatShadedMeshEntity::add_to_object_editor() {
    DrawableEntity::add_to_object_editor();
    ImGui::ColorEdit3("Object color", &color.x);
}
