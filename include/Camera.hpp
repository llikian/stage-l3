/***************************************************************************************************
 * @file  Camera.hpp
 * @brief Declaration of the Camera class
 **************************************************************************************************/

#pragma once

#include "Shader.hpp"
#include "maths/mat4.hpp"
#include "maths/vec3.hpp"

enum class MovementDirection : unsigned char {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWNWARD
};

/**
 * @class Camera
 * @brief
 */
class Camera {
public:
    /**
     * @brief Creates a camera at a specific position looking towards the -z axis.
     * @param position The position of the camera.
     */
    explicit Camera(const vec3& position);

    void look_around(float pitch_offset, float yaw_offset, float delta);
    void move_around(MovementDirection movement_direction, float delta);

    const mat4& get_view_matrix() const;

private:
    vec3 position;
    float pitch;
    float yaw;

    vec3 direction;
    vec3 right;
    vec3 up;

    mat4 view_matrix;

    const vec3 WORLD_UP{ 0.0f, 1.0f, 0.0f };
};
