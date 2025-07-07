/***************************************************************************************************
 * @file  transforms.hpp
 * @brief Declaration of functions for 3D transforms
 **************************************************************************************************/

#pragma once

#include "mat4.hpp"
#include "quaternion.hpp"
#include "vec3.hpp"

/**
 * @brief Calculates the scaling matrix that scales by the same factor in all 3 directions.
 * @param factor The scaling factor.
 * @return The scaling matrix.
 */
mat4 scale(float factor);

/**
 * @brief Calculates the scaling matrix that scales by a specific factor in each direction.
 * @param x The scaling factor in the x direction.
 * @param y The scaling factor in the y direction.
 * @param z The scaling factor in the z direction.
 * @return The scaling matrix.
 */
mat4 scale(float x, float y, float z);

/**
 * @brief Calculates the scaling matrix that scales by a specific factor in each direction.
 * @param factors The scaling factors.
 * @return The scaling matrix.
 */
mat4 scale(const vec3& factors);

/**
 * @brief Calculates the scaling matrix that only scales in the x direction.
 * @param factor The scaling factor.
 * @return The scaling matrix.
 */
mat4 scale_x(float factor);

/**
 * @brief Calculates the scaling matrix that only scales in the y direction.
 * @param factor The scaling factor.
 * @return The scaling matrix.
 */
mat4 scale_y(float factor);

/**
 * @brief Calculates the scaling matrix that only scales in the z direction.
 * @param factor The scaling factor.
 * @return The scaling matrix.
 */
mat4 scale_z(float factor);

/**
 * @brief Calculates the translation matrix that displaces by a specific vector.
 * @param vector The translation vector.
 * @return The translation matrix.
 */
mat4 translate(const vec3& vector);

/**
 * @brief Calculates the translation matrix that displaces by a specific amount in each direction.
 * @param x The displacement in the x direction.
 * @param y The displacement in the y direction.
 * @param z The displacement in the z direction.
 * @return The translation matrix.
 */
mat4 translate(float x, float y, float z);

/**
 * @brief Calculates the translation matrix that only displaces in the x direction.
 * @param scalar The displacement amount.
 * @return The translation matrix.
 */
mat4 translate_x(float scalar);

/**
 * @brief Calculates the translation matrix that only displaces in the y direction.
 * @param scalar The displacement amount.
 * @return The translation matrix.
 */
mat4 translate_y(float scalar);

/**
 * @brief Calculates the translation matrix that only displaces in the z direction.
 * @param scalar The displacement amount.
 * @return The translation matrix.
 */
mat4 translate_z(float scalar);

/**
 * @brief Calculates the rotation matrix that rotates around an axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @param axis The rotation axis.
 * @return The rotation matrix.
 */
mat4 rotate(float angle, vec3 axis);

/**
 * @brief Calculates the rotation matrix that rotates around the x axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @return The rotation matrix.
 */
mat4 rotate_x(float angle);

/**
 * @brief Calculates the rotation matrix that rotates around the y axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @return The rotation matrix.
 */
mat4 rotate_y(float angle);

/**
 * @brief Calculates the rotation matrix that rotates around the z axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @return The rotation matrix.
 */
mat4 rotate_z(float angle);

/**
 * @brief Calculates the TRS matrix such that TRS=T*Ry*Rx*Rz*S, with T being a translation matrix,
 * Rx, Ry, Rz being rotation matrices around their respective axis and S being a scale matrix.
 * @param translation The value of the translation vector.
 * @param rotation The values of the rotation angles.
 * @param scale The values of the scaling factors.
 * @return The matrix that scales then rotates then translates a point according to the given
 * parameters.
 */
mat4 TRS_matrix(const vec3& translation, const vec3& rotation, const vec3& scale);

/**
 * @brief Calculates the TRS matrix such that TRS=T*Rq*S, with T being a translation matrix, Rq
 * being the rotation matrix derived from a quaternion S being a scale matrix.
 * @param translation The value of the translation vector.
 * @param rotation The rotation quaternion. Assumed to be a unit quaternion.
 * @param scale The values of the scaling factors.
 * @return The matrix that scales then rotates then translates a point according to the given
 * parameters.
 */
mat4 TRS_matrix(const vec3& translation, const quaternion& rotation, const vec3& scale);

/**
 * @brief Calculates the 'look at' matrix. That allows to simulate a camera.
 * @param eye The position of the camera.
 * @param target The target of the camera.
 * @param up The up vector of the camera.
 * @return The 'look at' matrix.
 */
mat4 look_at(const vec3& eye, const vec3& target, const vec3& up);

/**
 * @brief Calculates the perspective matrix.
 * @param fov The field of vue value. Sets how large the viewspace is. For a realistic view it is
 * usually set to 45 degrees.
 * @param aspect The aspect ration. Is calculated by dividing the window's width by its height.
 * @param near, far The near and far plane. Usually set to respectively 0.1 and 100, all vertices
 * between the two planes are renderered, those outside won't.
 * @return The perspective matrix.
 */
mat4 perspective(float fov, float aspect, float near, float far);