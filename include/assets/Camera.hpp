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
    friend struct Frustum;

    /**
     * @brief Creates a camera at a specific position looking towards the -z axis.
     * @param position The position of the camera.
     * @param fov The camera's fov.
     * @param near_distance Distance of the near plane.
     * @param far_distance Distance of the far plane.
     */
    Camera(const vec3& position, float fov, float near_distance, float far_distance);

    /**
     * @brief Creates a camera at a specific position looking towards a specific point.
     * @param position The position of the camera.
     * @param target The point the camera is looking at.
     * @param fov The camera's fov.
     * @param near_distance Distance of the near plane.
     * @param far_distance Distance of the far plane.
     */
    Camera(const vec3& position, const vec3& target, float fov, float near_distance, float far_distance);

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

    /**
     * @return The camera's fov.
     */
    float get_fov() const;

    /**
     * @return Distance of the near plane.
     */
    float get_near_distance() const;

    /**
     * @return Distance of the far plane.
     */
    float get_far_distance() const;

    /**
     * @return A reference to the camera's view matrix.
     */
    const mat4& get_view_matrix() const;

    /**
     * @return A reference to the projection matrix.
     */
    const mat4& get_projection_matrix() const;

    /**
     * @brief Calculates the view-projection matrix.
     * @return The projection matrix multiplied by the view matrix.
     */
    mat4 get_view_projection_matrix() const;

    /**
     * @brief Calculates the camera's rotation matrix.
     * @return A rotation matrix that rotates towards where the camera is looking.
     */
    mat4 get_rotation_matrix() const;

    /**
     * @brief Calculates the camera's model matrix. This matrix is also the inverse of the camera's
     * view matrix.
     * @return A transformation matrix that rotates towards where the camera is looking and translates
     * it to the camera's position.
     */
    mat4 get_model_matrix() const;

    /**
     * @brief Calculates the inverse of the projection matrix.
     * @return The inverse of the projection matrix.
     */
    mat4 get_inverse_projection_matrix() const;

    /**
     * @brief Calculates the inverse of the view-projectino matrix.
     * @return The inverse of the view matrix multiplied with the inverse of the projection matrix:\n
     * (PV)^(-1) = V^(-1) * P^(-1).
     */
    mat4 get_inverse_view_projection_matrix() const;

    /**
     * @brief Sets the camera's position to a certain point.
     * @param position The camera's new position.
     */
    void set_position(const vec3& position);

    /**
     * @brief Updates the camera's direction depending on how the mouse moved since the last frame.
     * @param pitch_offset The offset to the camera's tilt angle (in degrees).
     * @param yaw_offset The offset to the camera's orientation around the y axis (in degrees).
     */
    void look_around(float pitch_offset, float yaw_offset);

    /**
     * @brief Moves the camera in a certain direction.
     * @param movement_direction The direction to move towards.
     */
    void move_around(MovementDirection movement_direction);

    /**
     * @brief Updates the projection matrix with according to the window's new aspect ratio.
     */
    void update_projection_matrix();

    /**
     * @brief Rotates the camera to make it look towards a certain point.
     * @param target The point the camera should look towards.
     */
    void look_at_point(const vec3& target);

    float sensitivity;    ///< The sensivity when looking around.
    float movement_speed; ///< The sensitivity of the movement.

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

    // TODO: Remove const keyword and add ways to modify these.
    const float fov;           ///< The camera's fov.
    const float near_distance; ///< Distance of the near plane.
    const float far_distance;  ///< Distance of the far plane.

    mat4 view_matrix;       ///< The camera's view matrix. Used every frame in the mvp matrix calculation.
    mat4 projection_matrix; ///< The projection matrix. Used every frame in the mvp matrix calculation.

    const vec3 WORLD_UP{ 0.0f, 1.0f, 0.0f }; ///< Where "up" is.
};
