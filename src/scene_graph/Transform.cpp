/***************************************************************************************************
 * @file  Transform.cpp
 * @brief Implementation of the Transform class
 **************************************************************************************************/

#include "scene_graph/Transform.hpp"

Transform::Transform()
    : local_position(0.0f),
      local_rotation(0.0f),
      local_scale(1.0f),
      is_dirty(true),
      global_model(1.0f) { }

void Transform::set_local_position(const vec3& position) {
    local_position = position;
    is_dirty = true;
}

void Transform::set_local_rotation(const vec3& rotation) {
    local_rotation = rotation;
    is_dirty = true;
}

void Transform::set_local_scale(const vec3& scale) {
    local_scale = scale;
    is_dirty = true;
}

void Transform::set_local_model_to_dirty() {
    is_dirty = true;
}

vec3 Transform::get_local_position() const {
    return local_position;
}

vec3& Transform::get_local_position_reference() {
    return local_position;
}

vec3 Transform::get_local_rotation() const {
    return local_rotation;
}

vec3& Transform::get_local_rotation_reference() {
    return local_rotation;
}

vec3 Transform::get_local_scale() const {
    return local_scale;
}

vec3& Transform::get_local_scale_reference() {
    return local_scale;
}

mat4 Transform::get_local_model() const {
    return translate(local_position)
          .rotate_y(local_rotation.y)
          .rotate_x(local_rotation.x)
          .rotate_z(local_rotation.z)
          .scale(local_scale);
}

mat4 Transform::get_global_model() const {
    return global_model;
}

const mat4& Transform::get_global_model_reference() const {
    return global_model;
}

bool Transform::is_local_model_dirty() const {
    return is_dirty;
}

void Transform::compute_global_model() {
    global_model = get_local_model();
    is_dirty = false;
}

void Transform::compute_global_model(const mat4& parent_global_model) {
    global_model = parent_global_model;
    global_model.translate(local_position)
                .rotate_y(local_rotation.y)
                .rotate_x(local_rotation.x)
                .rotate_z(local_rotation.z)
                .scale(local_scale);
    is_dirty = false;
}
