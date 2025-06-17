/***************************************************************************************************
 * @file  Camera.hpp
 * @brief Declaration of the Camera class
 **************************************************************************************************/

#pragma once

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
 * @brief Represents a first person that allows to fly and look around the 3D space.
 */
class Camera {
public:
    /**
     * @brief Creates a camera at a specific position looking towards the -z axis.
     * @param position The position of the camera.
     */
    explicit Camera(const vec3& position);

    /**
     * @brief Updates the camera's direction depending on how the mouse moved since the last frame.
     * @param pitch_offset The offset to the camera's tilt angle (in degrees).
     * @param yaw_offset The offset to the camera's orientation around the y axis (in degrees).
     */
    void look_around(float pitch_offset, float yaw_offset);

    /**
     * Moves the camera in a certain direction.
     * @param movement_direction The direction to move towards.
     * @param delta How much time passed since the last frame.
     */
    void move_around(MovementDirection movement_direction, float delta);

    /**
     * @return A reference to the camera's view matrix.
     */
    const mat4& get_view_matrix() const;

    /**
     * @return The camera's position.
     */
    vec3 get_position() const;

    /**
     * @return The direction is looking towards.
     */
    vec3 get_direction() const;

    /**
     * @return The direction pointing right of where the camera is looking.
     */
    vec3 get_right_vector() const;

    /**
     * @return The direction pointing up of where the camera is looking.
     */
    vec3 get_up_vector() const;

private:
    /**
     * @brief Recalculates the direction, right and up vectors depending on the camera's pitch and
     * yaw and updates the view matrix.
     */
    void update_vectors_and_view_matrix();

    vec3 position; ///< The camera's position.
    float pitch;   ///< The camera's pitch angle (in radians), the camera's tilt, the "up/down" angle.
    float yaw;     ///< The camera's yaw angle (in radians), the angle around the y axis, the "left/right angle."

    vec3 direction; ///< The direction is looking towards.
    vec3 right;     ///< The direction pointing right of where the camera is looking.
    vec3 up;        ///< The direction pointing up of where the camera is looking.

    mat4 view_matrix; ///< The camera's view matrix. Used every frame in the mvp matrix calculation.

    const float CAMERA_SENSITIVITY{ 0.1f }; ///< The sensivity when looking around.
    const float MOVEMENT_SPEED{ 10.0f };    ///< The sensitivity of the movement.

    const vec3 WORLD_UP{ 0.0f, 1.0f, 0.0f }; ///< Where "up" is.
};
