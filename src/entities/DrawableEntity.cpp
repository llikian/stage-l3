/***************************************************************************************************
 * @file  DrawableEntity.cpp
 * @brief Implementation of the DrawableEntity class
 **************************************************************************************************/

#include "entities/DrawableEntity.hpp"

#include "AssetManager.hpp"
#include "debug.hpp"

DrawableEntity::DrawableEntity(const std::string& name, const Shader& shader)
    : Entity(name), shader(shader), aabb(nullptr) { }

DrawableEntity::~DrawableEntity() {
    delete aabb;
}

void DrawableEntity::draw(const mat4& view_projection_matrix, const Frustum& frustum) const {
    total_drawable_entities++;

    if(is_visible) {
        total_not_hidden_entities++;

        if(aabb == nullptr || aabb->is_in_frustum(frustum.view_projection * transform.get_global_model())) {
            total_drawn_entities++;
            draw(view_projection_matrix);

#ifdef DEBUG_SHOW_BOUNDING_BOXES
            if(aabb != nullptr) {
                const Shader& bounding_volume_shader = AssetManager::get_shader("flat");
                bounding_volume_shader.use();
                bounding_volume_shader.set_uniform("u_mvp", view_projection_matrix
                                                            * aabb->get_global_model_matrix(transform));
                bounding_volume_shader.set_uniform("u_color", vec4(1.0f, 0.0f, 0.0f, 1.0f));
                glLineWidth(3.0f);
                AssetManager::get_mesh("wireframe cube").draw();
                glLineWidth(1.0f);
            }
#endif
        }
    }

    for(Entity* child : children) { child->draw(view_projection_matrix, frustum); }
}

void DrawableEntity::update_uniforms(const mat4& view_projection_matrix) const {
    const mat4& global_model = transform.get_global_model_const_reference();
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
