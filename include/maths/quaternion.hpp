/***************************************************************************************************
 * @file  quaternion.hpp
 * @brief Declaration of the quaternion struct
 **************************************************************************************************/

#pragma once

#include "mat4.hpp"

/**
 * @class quaternion
 * @brief Represents a quaternion with each of its four coefficients (x, y, z, w) such that:\n
 * q = x*i + y*j + z*k + d.
 */
struct quaternion {
    /**
     * @brief Constructs a quaternion with all coefficients set to 0.
     */
    quaternion();

    /**
     * @brief Constructs a quaternion with a specific value for each coefficient.
     * @param x The value for the coefficient of the i imaginary unit.
     * @param y The value for the coefficient of the j imaginary unit.
     * @param z The value for the coefficient of the k imaginary unit.
     * @param w The value for the real part of the quaternion.
     */
    quaternion(float x, float y, float z, float w);

    /**
     * @brief Adds another quaternion to this one.
     * @param q The quaternion to add.
     * @return A reference to this quaternion.
     */
    quaternion& operator +=(const quaternion& q);

    /**
     * @brief Subtracts another quaternion from this one.
     * @param q The quaternion to subtract.
     * @return A reference to this quaternion.
     */
    quaternion& operator -=(const quaternion& q);

    /**
     * @brief Multiplies another quaternion with this one.
     * @param q The quaternion to multiply with.
     * @return A reference to this quaternion.
     */
    quaternion& operator *=(const quaternion& q);

    /**
     * @brief Divides another quaternion from this one.
     * @param q The quaternion to divide with.
     * @return A reference to this quaternion.
     */
    quaternion& operator /=(const quaternion& q);

    /**
     * @brief Multiplies this quaternion by a scalar.
     * @param scalar The scalar to multiply with.
     * @return A reference to this quaternion.
     */
    quaternion& operator *=(float scalar);
    /**
     * @brief Divides this quaternion by a scalar.
     * @param scalar The scalar to divide with.
     * @return A reference to this quaternion.
     */
    quaternion& operator /=(float scalar);

    /**
     * @brief Calculates the length (norm) of the quaternion.
     * @return The length of the quaternion.
     */
    float get_length() const;

    /**
     * @brief Calculates the conjugate of the quaternion.
     * @return The conjugate of the quaternion: q* = (-x, -y, -z, w).
     */
    quaternion get_conjugate() const;

    /**
     * @brief Calculates the inverse of the quaternion.
     * @return The inverse of the quaternion: q^(-1) = (q*) / norm² = (-x, -y, -z, w) / norm²
     */
    quaternion get_inverse() const;

    /**
     * @brief Calculates the matrix form of the quaternion.
     * @return The matrix form of the quaternion.
     */
    mat4 get_matrix() const;

    /**
     * @brief Normalizes the quaternion: divides it by its norm so that it has a norm of 1.
     */
    void normalize();

    float x; ///< The coefficient of the i imaginary unit.
    float y; ///< The coefficient of the j imaginary unit.
    float z; ///< The coefficient of the k imaginary unit.
    float w; ///< The real part of the quaternion.
};

/**
 * @brief Converts a rotation with euler angles to its equivalent with a quaternion.
 * @param angles Rotation angles in degrees around each axis.
 * @return The quaternion corresponding to the euler rotation.
 */
quaternion euler_to_quaternion(const vec3& angles);

/**
 * @brief Converts a rotation with a quaternion to its equivalent with euler angles.
 * @param q A unit quaternion representing a rotation.
 * @return The rotation angles in degrees around each axix.
 */
vec3 quaternion_to_euler(const quaternion& q);

/**
 * @brief Outputs a quaternion to an output stream with the following format: "xi + yj + zk + w".
 * @param stream The output stream to output to.
 * @param q The quaternion to output.
 * @return A reference to the output stream.
 */
std::ostream& operator <<(std::ostream& stream, const quaternion& q);

/**
 * @brief Reads four values from an output stream into a quaternion's coefficients.
 * @param stream The input stream to read input from.
 * @param q The quaternion to read into.
 * @return A reference to the input stream.
 */
std::istream& operator >>(std::istream& stream, quaternion& q);

/**
 * @brief Add two quaternions together.
 * @param q The first quaternion.
 * @param r The second quaternion.
 * @return The sum of the two quaternions.
 */
quaternion operator +(const quaternion& q, const quaternion& r);

/**
 * @brief Subtract a quaternion from another.
 * @param q The quaternion to subtract from.
 * @param r The quaternion to subtract with.
 * @return The subtraction of the first quaternion by the second.
 */
quaternion operator -(const quaternion& q, const quaternion& r);

/**
 * @brief Multiplies two quaternions together. Multiplication is not commutative for quaternions.
 * @param q The left quaternion.
 * @param r The right quaternion.
 * @return The product of the first quaternion with the second.
 */
quaternion operator *(const quaternion& q, const quaternion& r);

/**
 * @brief Divides a quaternion by another.
 * @param q The quaternion to divide from.
 * @param r The quaternion to divide with.
 * @return The division of the first quaternion by the second.
 */
quaternion operator /(const quaternion& q, const quaternion& r);

/**
 * @brief Multiplies a quaternion by a scalar.
 * @param q The quaternion to multiply by the scalar.
 * @param scalar The scalar to multiply the quaternion with.
 * @return The product of the quaternion by the scalar.
 */
quaternion operator *(const quaternion& q, float scalar);

/**
 * @brief Multiplies a quaternion by a scalar.
 * @param q The quaternion to multiply by the scalar.
 * @param scalar The scalar to multiply the quaternion with.
 * @return The product of the quaternion by the scalar.
 */
quaternion operator *(float scalar, const quaternion& q);

/**
 * @brief Divides a quaternion by a scalar.
 * @param q The quaternion to divide by the scalar.
 * @param scalar The scalar to divide the quaternion with.
 * @return The division of the quaternion by the scalar.
 */
quaternion operator /(const quaternion& q, float scalar);

/**
 * @brief Divides a scalar by a quaternion.
 * @param q The quaternion to divide the scalar with.
 * @param scalar The scalar to divide by the quaternion.
 * @return The division of the scalar by the quaternion.
 */
quaternion operator /(float scalar, const quaternion& q);
