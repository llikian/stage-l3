/***************************************************************************************************
 * @file  Camera.cpp
 * @brief Implementation of the Camera class
 **************************************************************************************************/

#include "Camera.hpp"

#include <cmath>
#include <numbers>
#include "maths/geometry.hpp"
#include "maths/transforms.hpp"
#include "maths/trigonometry.hpp"

#define PIf (std::numbers::pi_v<float>)

Camera::Camera(const vec3& position, float fov, float aspect_ratio, float near_distance, float far_distance)
    : sensitivity(0.1f), movement_speed(100.0f),
      position(position),
      pitch(0.0f), yaw(-PIf / 2.0f),
      fov(fov), near_distance(near_distance), far_distance(far_distance),
      view_matrix(1.0f), projection_matrix(perspective(fov, aspect_ratio, near_distance, far_distance)) {
    update_vectors_and_view_matrix();
}

Camera::Camera(const vec3& position,
               const vec3& target,
               float fov,
               float aspect_ratio,
               float near_distance,
               float far_distance)
    : sensitivity(0.1f), movement_speed(100.0f),
      position(position),
      fov(fov), near_distance(near_distance), far_distance(far_distance),
      view_matrix(1.0f), projection_matrix(perspective(fov, aspect_ratio, near_distance, far_distance)) {
    look_at_point(target);
}

vec3 Camera::get_position() const {
    return position;
}

vec3 Camera::get_direction() const {
    return direction;
}

vec3 Camera::get_right_vector() const {
    return right;
}

vec3 Camera::get_up_vector() const {
    return up;
}

float Camera::get_fov() const {
    return fov;
}

float Camera::get_near_distance() const {
    return near_distance;
}

float Camera::get_far_distance() const {
    return far_distance;
}

const mat4& Camera::get_view_matrix() const {
    return view_matrix;
}

const mat4& Camera::get_projection_matrix() const {
    return projection_matrix;
}

void Camera::look_around(float pitch_offset, float yaw_offset) {
    static const float MAX_TILT_ANGLE = degrees_to_radians(80.0f);

    pitch = std::clamp(pitch - sensitivity * degrees_to_radians(pitch_offset), -MAX_TILT_ANGLE, MAX_TILT_ANGLE);
    yaw += sensitivity * degrees_to_radians(yaw_offset);

    update_vectors_and_view_matrix();
}

void Camera::move_around(MovementDirection movement_direction, float delta) {
    switch(movement_direction) {
        case MovementDirection::FORWARD:
            position += movement_speed * delta * direction;
            break;
        case MovementDirection::BACKWARD:
            position -= movement_speed * delta * direction;
            break;
        case MovementDirection::LEFT:
            position -= movement_speed * delta * right;
            break;
        case MovementDirection::RIGHT:
            position += movement_speed * delta * right;
            break;
        case MovementDirection::UPWARD:
            position += movement_speed * delta * WORLD_UP;
            break;
        case MovementDirection::DOWNWARD:
            position -= movement_speed * delta * WORLD_UP;
            break;
        default:
            break;
    }

    view_matrix(0, 3) = -dot(position, right);
    view_matrix(1, 3) = -dot(position, up);
    view_matrix(2, 3) = dot(position, direction);
}

void Camera::update_projection_matrix(float aspect_ratio) {
    projection_matrix(0, 0) = 1.0f / (aspect_ratio * std::tan(0.5f * fov));
}

void Camera::look_at_point(const vec3& target) {
    vec3 dir = normalize(target - position);
    pitch = asinf(dir.y);
    yaw = atan2f(dir.z, dir.x);
    update_vectors_and_view_matrix();
}

mat4 Camera::get_view_projection_matrix() const {
    return projection_matrix * view_matrix;
}

mat4 Camera::get_rotation_matrix() const {
    return mat4(
        right.x, up.x, -direction.x,
        right.y, up.y, -direction.y,
        right.z, up.z, -direction.z
    );
}

mat4 Camera::get_model_matrix() const {
    return mat4(
        right.x, up.x, -direction.x, position.x,
        right.y, up.y, -direction.y, position.y,
        right.z, up.z, -direction.z, position.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

void Camera::set_position(const vec3& position) {
    this->position = position;

    view_matrix(0, 3) = -dot(position, right);
    view_matrix(1, 3) = -dot(position, up);
    view_matrix(2, 3) = dot(position, direction);
}

void Camera::update_vectors_and_view_matrix() {
    direction.x = std::cos(pitch) * std::cos(yaw);
    direction.y = std::sin(pitch);
    direction.z = std::cos(pitch) * std::sin(yaw);

    right = normalize(cross(direction, WORLD_UP));
    up = normalize(cross(right, direction));

    view_matrix(0, 0) = right.x;
    view_matrix(0, 1) = right.y;
    view_matrix(0, 2) = right.z;
    view_matrix(0, 3) = -dot(position, right);

    view_matrix(1, 0) = up.x;
    view_matrix(1, 1) = up.y;
    view_matrix(1, 2) = up.z;
    view_matrix(1, 3) = -dot(position, up);

    view_matrix(2, 0) = -direction.x;
    view_matrix(2, 1) = -direction.y;
    view_matrix(2, 2) = -direction.z;
    view_matrix(2, 3) = dot(position, direction);
}
