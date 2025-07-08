/***************************************************************************************************
 * @file  Transform.cpp
 * @brief Implementation of the Transform class
 **************************************************************************************************/

#include "maths/Transform.hpp"

#include "maths/geometry.hpp"

Transform::Transform()
    : local_position(0.0f),
      local_orientation(0.0f, 0.0f, 0.0f, 1.0f),
      local_scale(1.0f),
      is_dirty(true),
      global_model(1.0f) { }

void Transform::set_local_position(const vec3& position) {
    local_position = position;
    is_dirty = true;
}

void Transform::set_local_position(float x, float y, float z) {
    local_position.x = x;
    local_position.y = y;
    local_position.z = z;
    is_dirty = true;
}

void Transform::set_local_orientation(const quaternion& orientation) {
    local_orientation = orientation;
    is_dirty = true;
}

void Transform::set_local_orientation(float x, float y, float z, float w) {
    local_orientation.x = x;
    local_orientation.y = y;
    local_orientation.z = z;
    local_orientation.w = w;
    is_dirty = true;
}

void Transform::set_local_scale(const vec3& scale) {
    local_scale = scale;
    is_dirty = true;
}

void Transform::set_local_scale(float scale) {
    local_scale.x = local_scale.y = local_scale.z = scale;
    is_dirty = true;
}

void Transform::set_local_scale(float x, float y, float z) {
    local_scale.x = x;
    local_scale.y = y;
    local_scale.z = z;
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

quaternion Transform::get_local_orientation() const {
    return local_orientation;
}

quaternion& Transform::get_local_orientation_reference() {
    return local_orientation;
}

vec3 Transform::get_local_scale() const {
    return local_scale;
}

vec3& Transform::get_local_scale_reference() {
    return local_scale;
}

mat4 Transform::compute_local_model() const {
    return TRS_matrix(local_position, local_orientation, local_scale);
}

mat4 Transform::get_global_model() const {
    return global_model;
}

const mat4& Transform::get_global_model_const_reference() const {
    return global_model;
}

vec3 Transform::get_global_position() const {
    return vec3(global_model(0, 3), global_model(1, 3), global_model(2, 3));
}

vec3 Transform::get_global_scale() const {
    return vec3(length(get_right_vector()), length(get_up_vector()), length(get_front_vector()));
}

vec3 Transform::get_front_vector() const {
    return vec3(-global_model(0, 2), -global_model(1, 2), -global_model(2, 2));
}

vec3 Transform::get_right_vector() const {
    return vec3(global_model(0, 0), global_model(1, 0), global_model(2, 0));
}

vec3 Transform::get_up_vector() const {
    return vec3(global_model(0, 1), global_model(1, 1), global_model(2, 1));
}

bool Transform::is_local_model_dirty() const {
    return is_dirty;
}

void Transform::update_global_model() {
    global_model = compute_local_model();
    is_dirty = false;
}

void Transform::update_global_model(const mat4& parent_global_model) {
    global_model = parent_global_model * compute_local_model();
    is_dirty = false;
}
