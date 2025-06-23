/***************************************************************************************************
 * @file  Entity.cpp
 * @brief Implementation of the Entity class
 **************************************************************************************************/

#include "scene_graph/Entity.hpp"

Entity::Entity(const std::string& name) : name(name), parent(nullptr) { }

Entity::~Entity() {
    for(const Entity* child : children) { delete child; }
}

void Entity::update_transform_and_children() {
    if(transform.is_local_model_dirty()) {
        if(parent != nullptr) {
            transform.compute_global_model(parent->transform.get_global_model());
        } else {
            transform.compute_global_model();
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

DrawableEntity::DrawableEntity(const std::string& name, Shader* shader) : Entity(name), shader(shader) { }

void DrawableEntity::update_uniforms(const mat4& view_projection_matrix) const {
    int u_mvp_location = shader->get_uniform_location("u_mvp");
    if(u_mvp_location != -1) {
        Shader::set_uniform(u_mvp_location, view_projection_matrix * transform.get_global_model());
    }

    shader->set_uniform_if_exists("u_model", transform.get_global_model_reference());

    int u_normals_model_matrix_location = shader->get_uniform_location("u_normals_model_matrix");
    if(u_normals_model_matrix_location != -1) {
        Shader::set_uniform(u_normals_model_matrix_location,
                            transpose_inverse(transform.get_global_model_reference()));
    }
}

bool DrawableEntity::is_drawable() const {
    return true;
}

ModelEntity::ModelEntity(const std::string& name, Shader* shader, const std::filesystem::path& path)
    : DrawableEntity(name, shader), model(path) { }

void ModelEntity::draw(const mat4& view_projection_matrix) {
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
    if(shader != nullptr) {
        shader->use();
        update_uniforms(view_projection_matrix);
        mesh.draw(*shader);
    } else {
        std::cout << "[WARNING] TriangleMeshEntity '" << name << "' with nullptr shader.\n";
    }
}
