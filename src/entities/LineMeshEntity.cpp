/***************************************************************************************************
 * @file  LineMeshEntity.cpp
 * @brief Implementation of the LineMeshEntity class
 **************************************************************************************************/

#include "entities/LineMeshEntity.hpp"

LineMeshEntity::LineMeshEntity(const std::string& name, const Shader* shader)
    : DrawableEntity(name, shader) { }

void LineMeshEntity::draw(const mat4& view_projection_matrix) {
    if(shader != nullptr) {
        shader->use();
        update_uniforms(view_projection_matrix);
        mesh.draw(*shader);
    } else {
        std::cout << "[WARNING] LineMeshEntity '" << name << "' with nullptr shader.\n";
    }
}

void LineMeshEntity::create_aabb() {
    vec3 min(std::numeric_limits<float>::max());
    vec3 max(std::numeric_limits<float>::lowest());
    mesh.get_min_max_axis_aligned_coordinates(min, max);
    bounding_volume = new AABB(min, max);
}