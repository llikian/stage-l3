/***************************************************************************************************
 * @file  DrawableEntity.cpp
 * @brief Implementation of the DrawableEntity class
 **************************************************************************************************/

#include "entities/DrawableEntity.hpp"

DrawableEntity::DrawableEntity(const std::string& name, const Shader& shader)
    : Entity(name), shader(shader), aabb(nullptr) { }

DrawableEntity::~DrawableEntity() {
    delete aabb;
}

void DrawableEntity::update_uniforms(const mat4& view_projection_matrix) {
    const mat4& global_model = transform.get_global_model_reference();
    shader.set_uniform_if_exists("u_model", global_model);

    int u_mvp_location = shader.get_uniform_location("u_mvp");
    if(u_mvp_location != -1) {
        Shader::set_uniform(u_mvp_location, view_projection_matrix * global_model);
    }

    int u_normals_model_matrix_location = shader.get_uniform_location("u_normals_model_matrix");
    if(u_normals_model_matrix_location != -1) {
        Shader::set_uniform(u_normals_model_matrix_location, transpose_inverse(global_model));
    }
}

bool DrawableEntity::is_drawable() const {
    return true;
}
