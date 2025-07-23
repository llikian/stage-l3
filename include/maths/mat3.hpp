/***************************************************************************************************
 * @file  mat3.hpp
 * @brief Declaration of the mat3 struct
 **************************************************************************************************/

#pragma once

#include <ostream>
#include "vec3.hpp"

struct mat4; // Forward-Definition of mat4

/**
 * @struct mat3
 * @brief Represents a 3 by 3 matrix.
 */
struct mat3 {
public:
    /**
     * @brief Constructs a mat3 with all components equal to 0.
     */
    mat3();

    /**
     * @brief Constructs a mat3 with a specific value for each component.
     * @param v00, v01, v02 The values of the first three components of the first row.
     * @param v10, v11, v12 The values of the first three components of the second row.
     * @param v20, v21, v22 The values of the first three components of the third row.
     */
    mat3(float v00, float v01, float v02,
         float v10, float v11, float v12,
         float v20, float v21, float v22);

    /**
     * @brief Constructs a mat3 which is the identity matrix multiplied by a scalar.
     * Each value of the diagonal is equal to the scalar and the rest is zeros.
     * @param scalar The value for the components on the diagonal.
     */
    explicit mat3(float scalar);

    /**
     * @brief Accesses an element of the mat3.
     * @param row The row's index.
     * @param column The column's index.
     * @return A reference to the element.
     */
    float& operator ()(int row, int column);

    /**
     * @brief Accesses an element of the mat3.
     * @param row The row's index.
     * @param column The column's index.
     * @return A const reference to the element.
     */
    const float& operator ()(int row, int column) const;

private:
    float values[3][3]; ///< The values of the mat3.
};

/**
 * @brief Calculates the inverse of a mat3.
 * @param mat The mat3 we want the inverse of.
 * @return The inverse of the mat3. If no inverse exists, simply returns the input mat3.
 */
mat3 inverse(const mat3& mat);

/**
 * @brief Calculates the transpose of the inverse of a mat3.
 * @param mat The mat3 we want the transpose of the inverse of.
 * @return The transpose of the inverse of the mat3. If no inverse exists, simply returns the input mat3.
 */
mat3 transpose_inverse(const mat3& mat);

/**
 * @brief Calculates the transpose of the inverse of a mat4's upper left 3x3 matrix.
 * @param mat The mat4 we want the transpose of the inverse of.
 * @return The transpose of the inverse of the mat3. If no inverse exists, simply returns the input mat3.
 */
mat3 transpose_inverse(const mat4& mat);

/**
 * @brief Writes the components of the given mat3 to the output stream in the format:\n
 * ( v00 ; v01 ; v02 )\n
 * ( v10 ; v11 ; v12 )\n
 * ( v20 ; v21 ; v22 )\n
 * @param stream The output stream to write to.
 * @param mat The mat3 to write to the stream.
 * @return A reference to the output stream after writing the mat3.
 */
std::ostream& operator <<(std::ostream& stream, const mat3& mat);

/**
 * @brief Adds the second mat3's components to the first's.
 * @param left The left operand. Will store the result.
 * @param right The right operand.
 */
void operator +=(mat3& left, const mat3& right);

/**
 * @brief Subtracts the second mat3's components from the first's.
 * @param left The left operand. Will store the result.
 * @param right The right operand.
 */
void operator -=(mat3& left, const mat3& right);

/**
 * @brief Adds a scalar to all of a mat3's components.
 * @param mat The mat3.
 * @param scalar The scalar.
 */
void operator +=(mat3& mat, float scalar);

/**
 * @brief Subtracts all of a mat3's components by a scalar.
 * @param mat The mat3.
 * @param scalar The scalar.
 */
void operator -=(mat3& mat, float scalar);

/**
 * @brief Multiplies a scalar to all of a mat3's components.
 * @param mat The mat3.
 * @param scalar The scalar.
 */
void operator *=(mat3& mat, float scalar);

/**
 * @brief Divides all of a mat3's components by a scalar.
 * @param mat The mat3.
 * @param scalar The scalar.
 */
void operator /=(mat3& mat, float scalar);

/** @brief Adds a mat3's components to another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise sum of the two mat3.
 */
mat3 operator +(const mat3& left, const mat3& right);

/** @brief Subtracts a mat3's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise subtraction of the first mat3 by the second.
 */
mat3 operator -(const mat3& left, const mat3& right);

/** @brief Multiplies a mat3 by another.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The product of the two mat3.
 */
mat3 operator *(const mat3& left, const mat3& right);

/** @brief Adds a scalar to each of a mat3's components.
 *  @param mat The mat3.
 *  @param scalar The scalar.
 *  @return The component-wise sum of a mat3 by a scalar.
 */
mat3 operator +(const mat3& mat, float scalar);

/** @brief Subtracts each of a mat3's components by a scalar.
 *  @param mat The mat3.
 *  @param scalar The scalar.
 *  @return The component-wise subtraction of a mat3 by a scalar.
 */
mat3 operator -(const mat3& mat, float scalar);

/** @brief Multiplies each of a mat3's components by a scalar.
 *  @param mat The mat3.
 *  @param scalar The scalar.
 *  @return The component-wise product of a mat3 by a scalar.
 */
mat3 operator *(const mat3& mat, float scalar);

/** @brief Multiplies each of a mat3's components by a scalar.
 *  @param scalar The scalar.
 *  @param mat The mat3.
 *  @return The component-wise product of a mat3 by a scalar.
 */
mat3 operator *(float scalar, const mat3& mat);

/** @brief Divides each of a mat3's components by a scalar.
 *  @param mat The mat3.
 *  @param scalar The scalar.
 *  @return The component-wise division of a mat3 by a scalar.
 */
mat3 operator /(const mat3& mat, float scalar);

/**
 * @brief Multiplies a mat3 by a 3-component vector interpreted as the 3x1 column mat3 (vec.x, vec.y, vec.z).
 * @param mat The mat3.
 * @param vec The vector.
 * @return The result of mat * (vec.x, vec.y, vec.z).
 */
vec3 operator*(const mat3& mat, const vec3& vec);
