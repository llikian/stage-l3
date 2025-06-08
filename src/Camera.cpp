/***************************************************************************************************
 * @file  Camera.cpp
 * @brief Implementation of the Camera class
 **************************************************************************************************/

#include "Camera.hpp"

#include <cmath>
#include <numbers>
#include "maths/geometry.hpp"

#define PIf (std::numbers::pi_v<float>)

Camera::Camera(const vec3& position)
    : position(position),
      pitch(PIf / 2.0f), yaw(0.0f),
      direction(0.0f, 0.0f, -1.0f), right(-1.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f),
      view_matrix(right.x, right.y, right.z, -dot(position, right),
                  up.x, up.y, up.z, -dot(position, up),
                  direction.x, direction.y, direction.z, -dot(position, direction),
                  0.0f, 0.0f, 0.0f, 1.0f
      ) { }

void Camera::look_around(float pitch_offset, float yaw_offset, float delta) {
    float sensitivity = delta * 0.05f;

    pitch = std::clamp(pitch + sensitivity * pitch_offset, 0.0f, PIf);
    yaw = std::clamp(yaw + sensitivity * yaw_offset, 0.0f, 2.0f * PIf);

    direction.x = std::sin(pitch) * std::cos(yaw);
    direction.y = std::cos(pitch);
    direction.z = std::sin(pitch) * std::sin(yaw);
    view_matrix(2, 0) = direction.x;
    view_matrix(2, 1) = direction.y;
    view_matrix(2, 2) = direction.z;
    view_matrix(2, 3) = -dot(position, direction);

    right = cross(direction, WORLD_UP);
    view_matrix(0, 0) = right.x;
    view_matrix(0, 1) = right.y;
    view_matrix(0, 2) = right.z;
    view_matrix(0, 3) = -dot(position, right);

    up = cross(right, direction);
    view_matrix(1, 0) = up.x;
    view_matrix(1, 1) = up.y;
    view_matrix(1, 2) = up.z;
    view_matrix(1, 3) = -dot(position, up);
}

void Camera::move_around(MovementDirection movement_direction, float delta) {
    float sensitivity = delta * 0.05f;

    switch(movement_direction) {
        case MovementDirection::FORWARD:
            position += sensitivity * direction;
            break;
        case MovementDirection::BACKWARD:
            position -= sensitivity * direction;
            break;
        case MovementDirection::LEFT:
            position -= sensitivity * right;
            break;
        case MovementDirection::RIGHT:
            position += sensitivity * right;
            break;
        case MovementDirection::UPWARD:
            position += sensitivity * WORLD_UP;
            break;
        case MovementDirection::DOWNWARD:
            position += sensitivity * WORLD_UP;
            break;
        default:
            break;
    }

    view_matrix(2, 3) = -dot(position, direction);
    view_matrix(0, 3) = -dot(position, right);
    view_matrix(1, 3) = -dot(position, up);
}

const mat4& Camera::get_view_matrix() const {
    return view_matrix;
}
