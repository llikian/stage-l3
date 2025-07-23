/***************************************************************************************************
 * @file  Transform.cpp
 * @brief Implementation of the Transform class
 **************************************************************************************************/

#include "maths/Transform.hpp"

#include <cmath>

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

void Transform::set_local_orientation_euler(const vec3& angles) {
    local_orientation = euler_to_quaternion(angles);
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

void Transform::set_local_model(const float model[16]) {
    local_position.x = model[12];
    local_position.y = model[13];
    local_position.z = model[14];

    vec3 right(model[0], model[1], model[2]);
    vec3 up(model[4], model[5], model[6]);
    vec3 front(model[8], model[9], model[10]);

    local_scale.x = length(right);
    local_scale.y = length(up);
    local_scale.z = length(front);

    right /= local_scale.x;
    up /= local_scale.y;
    front /= local_scale.z;

    float trace = right.x + up.y + front.z;
    if(trace > 0.0f) {
        float s = std::sqrt(trace + 1.0f) * 2.0f;
        local_orientation.x = (up.z - front.y) / s;
        local_orientation.y = (front.x - right.z) / s;
        local_orientation.z = (right.y - up.x) / s;
        local_orientation.w = 0.25f * s;
    } else if((right.x > up.y) && (right.x > front.z)) {
        float s = std::sqrt(1.0f + right.x - up.y - front.z) * 2.0f;
        local_orientation.x = 0.25f * s;
        local_orientation.y = (up.x + right.y) / s;
        local_orientation.z = (front.x + right.z) / s;
        local_orientation.w = (up.z - front.y) / s;
    } else if(up.y > front.z) {
        float s = std::sqrt(1.0f + up.y - right.x - front.z) * 2.0f;
        local_orientation.x = (up.x + right.y) / s;
        local_orientation.y = 0.25f * s;
        local_orientation.z = (front.y + up.z) / s;
        local_orientation.w = (front.x - right.z) / s;
    } else {
        float s = std::sqrt(1.0f + front.z - right.x - up.y) * 2.0f;
        local_orientation.x = (front.x + right.z) / s;
        local_orientation.y = (front.y + up.z) / s;
        local_orientation.z = 0.25f * s;
        local_orientation.w = (right.y - up.x) / s;
    }

    local_orientation.normalize();

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
