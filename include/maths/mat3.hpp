/***************************************************************************************************
 * @file  mat3.hpp
 * @brief Declaration of the mat3 struct
 **************************************************************************************************/

#pragma once

#include <ostream>

struct mat4;

/**
 * @struct mat3
 * @brief Represents a 4 by 4 matrix.
 */
struct mat3 {
public:
    /**
     * @brief Constructs a matrix with all components equal to 0.
     */
    mat3();

    /**
     * @brief Constructs a matrix with a specific value for each component.
     * @param v00, v01, v02 The values of the first three components of the first row.
     * @param v10, v11, v12 The values of the first three components of the second row.
     * @param v20, v21, v22 The values of the first three components of the third row.
     */
    mat3(float v00, float v01, float v02,
         float v10, float v11, float v12,
         float v20, float v21, float v22);

    explicit mat3(const mat4& mat3);

    /**
     * @brief Constructs a matrix which is the identity matrix multiplied by a scalar.
     * Each value of the diagonal is equal to the scalar and the rest is zeros.
     * @param scalar The value for the components on the diagonal.
     */
    explicit mat3(float scalar);

    /**
     * @brief Accesses an element of the matrix.
     * @param row The row's index.
     * @param column The column's index.
     * @return A reference to the element.
     */
    float& operator ()(int row, int column);

    /**
     * @brief Accesses an element of the matrix.
     * @param row The row's index.
     * @param column The column's index.
     * @return A const reference to the element.
     */
    const float& operator ()(int row, int column) const;

private:
    float values[3][3]; ///< The values of the matrix.
};

/**
 * @brief Calculates the inverse of a matrix.
 * @param mat The matrix we want the inverse of.
 * @return The inverse of the matrix. If no inverse exists, simply returns the input matrix.
 */
mat3 inverse(const mat3& mat);

/**
 * @brief Calculates the transpose of the inverse of a matrix.
 * @param mat The matrix we want the transpose of the inverse of.
 * @return The transpose of the inverse of the matrix. If no inverse exists, simply returns the input matrix.
 */
mat3 transpose_inverse(const mat3& mat);

/**
 * @brief Calculates the transpose of the inverse of a 4x4 matrix's upper left 3x3 matrix.
 * @param mat The matrix we want the transpose of the inverse of.
 * @return The transpose of the inverse of the matrix. If no inverse exists, simply returns the input matrix.
 */
mat3 transpose_inverse(const mat4& mat);

/**
 * @brief Writes the components of the given matrix to the output stream in the format:\n
 * ( v00 ; v01 ; v02 )\n
 * ( v10 ; v11 ; v12 )\n
 * ( v20 ; v21 ; v22 )\n
 * @param stream The output stream to write to.
 * @param mat The matrix to write to the stream.
 * @return A reference to the output stream after writing the matrix.
 */
std::ostream& operator <<(std::ostream& stream, const mat3& mat);

/**
 * @brief Adds the second matrix's components to the first's.
 * @param left The left operand. Will store the result.
 * @param right The right operand.
 */
void operator +=(mat3& left, const mat3& right);

/**
 * @brief Subtracts the second matrix's components from the first's.
 * @param left The left operand. Will store the result.
 * @param right The right operand.
 */
void operator -=(mat3& left, const mat3& right);

/**
 * @brief Adds a scalar to all of a matrix's components.
 * @param mat The matrix.
 * @param scalar The scalar.
 */
void operator +=(mat3& mat, float scalar);

/**
 * @brief Subtracts all of a matrix's components by a scalar.
 * @param mat The matrix.
 * @param scalar The scalar.
 */
void operator -=(mat3& mat, float scalar);

/**
 * @brief Multiplies a scalar to all of a matrix's components.
 * @param mat The matrix.
 * @param scalar The scalar.
 */
void operator *=(mat3& mat, float scalar);

/**
 * @brief Divides all of a matrix's components by a scalar.
 * @param mat The matrix.
 * @param scalar The scalar.
 */
void operator /=(mat3& mat, float scalar);

/** @brief Adds a matrix's components to another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise sum of the two matrix.
 */
mat3 operator +(const mat3& left, const mat3& right);

/** @brief Subtracts a matrix's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise subtraction of the first matrix by the second.
 */
mat3 operator -(const mat3& left, const mat3& right);

/** @brief Multiplies a matrix by another.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The product of the two matrix.
 */
mat3 operator *(const mat3& left, const mat3& right);

/** @brief Adds a scalar to each of a matrix's components.
 *  @param mat The matrix.
 *  @param scalar The scalar.
 *  @return The component-wise sum of a matrix by a scalar.
 */
mat3 operator +(const mat3& mat, float scalar);

/** @brief Subtracts each of a matrix's components by a scalar.
 *  @param mat The matrix.
 *  @param scalar The scalar.
 *  @return The component-wise subtraction of a matrix by a scalar.
 */
mat3 operator -(const mat3& mat, float scalar);

/** @brief Multiplies each of a matrix's components by a scalar.
 *  @param mat The matrix.
 *  @param scalar The scalar.
 *  @return The component-wise product of a matrix by a scalar.
 */
mat3 operator *(const mat3& mat, float scalar);

/** @brief Multiplies each of a matrix's components by a scalar.
 *  @param scalar The scalar.
 *  @param mat The matrix.
 *  @return The component-wise product of a matrix by a scalar.
 */
mat3 operator *(float scalar, const mat3& mat);

/** @brief Divides each of a matrix's components by a scalar.
 *  @param mat The matrix.
 *  @param scalar The scalar.
 *  @return The component-wise division of a matrix by a scalar.
 */
mat3 operator /(const mat3& mat, float scalar);