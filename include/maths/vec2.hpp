/***************************************************************************************************
 * @file  vec2.hpp
 * @brief Declaration of the vec2 struct
 **************************************************************************************************/

#pragma once

#include <iostream>

/**
 * @struct vec2
 * @brief Represents a 2 dimensional vector.
 */
struct vec2 {
    /**
     * @brief Constructs a vec2 with all components equal to 0.
     */
    vec2();

    /**
     * @brief Constructs a vec2 with a specific value for each component.
     * @param x The value of the x component.
     * @param y The value of the y component.
     */
    vec2(float x, float y);

    /**
     * @brief Constructs a vec2 with the same value for each component.
     * @param scalar The value of each component.
     */
    explicit vec2(float scalar);

    /**
     * @brief Adds another vec2's components to the current instance's components.
     * @param vec The vec2 to add.
     * @return A reference to this instance.
     */
    vec2& operator +=(const vec2& vec);

    /**
     * @brief Subtracts the current instance's components by another vec2's components.
     * @param vec The vec2 to subtract by.
     * @return A reference to this instance.
     */
    vec2& operator -=(const vec2& vec);

    /**
     * @brief Multiplies the current instance's components by another vec2's components.
     * @param vec The vec2 to multiply by.
     * @return A reference to this instance.
     */
    vec2& operator *=(const vec2& vec);

    /**
     * @brief Divides the current instance's components by another vec2's components.
     * @param vec The vec2 to divide by.
     * @return A reference to this instance.
     */
    vec2& operator /=(const vec2& vec);

    /**
     * @brief Adds a scalar to all of the current instance's components.
     * @param scalar The scalar to add.
     * @return A reference to this instance.
     */
    vec2& operator +=(float scalar);

    /**
     * @brief Subtracts all of the current instance's components by a scalar.
     * @param scalar The scalar to subtract by.
     * @return A reference to this instance.
     */
    vec2& operator -=(float scalar);

    /**
     * @brief Multiplies all of the current instance's components by a scalar.
     * @param scalar The scalar to multiply by.
     * @return A reference to this instance.
     */
    vec2& operator *=(float scalar);

    /**
     * @brief Divides all of the current instance's components by a scalar.
     * @param scalar The scalar to divide by.
     * @return A reference to this instance.
     */
    vec2& operator /=(float scalar);

    float x; ///< The x component of the vec2.
    float y; ///< The y component of the vec2.
};

/**
 * @brief Writes the components of the given vec2 to the output stream in the format "( x ; y )".
 * @param stream The output stream to write to.
 * @param vec The vec2 to write to the stream.
 * @return A reference to the output stream after writing the vec2.
 */
std::ostream& operator <<(std::ostream& stream, const vec2& vec);

/**
 * @brief Reads two values from the input stream and assigns them to the x and y components
 * of the given vec2.
 * @param stream The input stream to read from.
 * @param vec The vec2 to assign the read values to.
 * @return A reference to the input stream after reading the values and assigning them to vec2.
 */
std::istream& operator >>(std::istream& stream, vec2& vec);

/** @brief Adds a vec2's components to another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise sum of the two vec2.
 */
vec2 operator +(const vec2& left, const vec2& right);

/** @brief Subtracts a vec2's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise subtraction of the first vec2 by the second.
 */
vec2 operator -(const vec2& left, const vec2& right);

/** @brief Multiplies a vec2's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise product of the two vec2.
 */
vec2 operator *(const vec2& left, const vec2& right);

/** @brief Divides a vec2's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise division of the first vec2 by the second.
 */
vec2 operator /(const vec2& left, const vec2& right);

/** @brief Adds a scalar to each of a vec2's components.
 *  @param vec The vec2.
 *  @param scalar The scalar.
 *  @return The component-wise sum of a vec2 by a scalar.
 */
vec2 operator +(const vec2& vec, float scalar);

/** @brief Subtracts each of a vec2's components by a scalar.
 *  @param vec The vec2.
 *  @param scalar The scalar.
 *  @return The component-wise subtraction of a vec2 by a scalar.
 */
vec2 operator -(const vec2& vec, float scalar);

/** @brief Multiplies each of a vec2's components by a scalar.
 *  @param vec The vec2.
 *  @param scalar The scalar.
 *  @return The component-wise product of a vec2 by a scalar.
 */
vec2 operator *(const vec2& vec, float scalar);

/** @brief Multiplies each of a vec2's components by a scalar.
 *  @param scalar The scalar.
 *  @param vec The vec2.
 *  @return The component-wise product of a vec2 by a scalar.
 */
vec2 operator *(float scalar, const vec2& vec);

/** @brief Divides each of a vec2's components by a scalar.
 *  @param vec The vec2.
 *  @param scalar The scalar.
 *  @return The component-wise division of a vec2 by a scalar.
 */
vec2 operator /(const vec2& vec, float scalar);

/**
 * @brief Tests whether two vec2 are equal.
 * @param left The left operand.
 * @param right The right operand.
 * @return Whether the two vec2 are equal.
 */
bool operator ==(const vec2& left, const vec2& right);

/**
 * @brief Tests whether two vec2 are different.
 * @param left The left operand.
 * @param right The right operand.
 * @return Whether the two vec2 are different.
 */
bool operator !=(const vec2& left, const vec2& right);