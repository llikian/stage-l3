/***************************************************************************************************
 * @file  vector3.hpp
 * @brief Declaration of the vector3 struct
 **************************************************************************************************/

#pragma once

#include <iostream>
#include "vector2.hpp"

/**
 * @class vector3
 * @brief Holds 3 values of the same type.
 * @tparam Type The type of the vector's components.
 */
template <typename Type>
struct vector3 {
    /**
     * @brief Constructs a vector3 with all components set to 0 (or default initialized in the case
     * of a class).
     */
    vector3() : x(), y(), z() { }

    /**
     * @brief Constructs a vector3 with a specific value for each component.
     * @param x The value of the x component.
     * @param y The value of the y component.
     * @param z The value of the z component.
     */
    vector3(Type x, Type y, Type z) : x(x), y(y), z(z) { }

    /**
     * @brief Constructs a vector3 with its first 2 components specified by a vector2 and its last
     * one by an explicit value.
     * @param xy The value of the x and y components
     * @param z The value of the z component.
     */
    vector3(const vector2<Type>& xy, float z) : x(xy.x), y(xy.y), z(z) { }

    /**
     * @brief Constructs a vector3 with its components specified by a vector4's first 3 components.
     * @param xyzw The value of the xyz (and the ignored w) components.
     */
    explicit vector3(const vector4<Type>& xyzw) : x(xyzw.x), y(xyzw.y), z(xyzw.z) { }

    /**
     * @brief Constructs a vector3 with the same value for each component.
     * @param value The value of each component.
     */
    explicit vector3(Type value) : x(value), y(value), z(value) { }

    /**
     * @brief Adds another vector3's components to the current instance's components.
     * @param vec The vector3 to add.
     * @return A reference to this instance.
     */
    vector3& operator +=(const vector3& vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;

        return *this;
    }

    /**
     * @brief Subtracts the current instance's components by another vector3's components.
     * @param vec The vector3 to subtract by.
     * @return A reference to this instance.
     */
    vector3& operator -=(const vector3& vec) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;

        return *this;
    }

    /**
     * @brief Multiplies the current instance's components by another vector3's components.
     * @param vec The vector3 to multiply by.
     * @return A reference to this instance.
     */
    vector3& operator *=(const vector3& vec) {
        x *= vec.x;
        y *= vec.y;
        z *= vec.z;

        return *this;
    }

    /**
     * @brief Divides the current instance's components by another vector3's components.
     * @param vec The vector3 to divide by.
     * @return A reference to this instance.
     */
    vector3& operator /=(const vector3& vec) {
        x /= vec.x;
        y /= vec.y;
        z /= vec.z;

        return *this;
    }

    /**
     * @brief Adds a value to all of the current instance's components.
     * @param value The value to add.
     * @return A reference to this instance.
     */
    vector3& operator +=(Type value) {
        x += value;
        y += value;
        z += value;

        return *this;
    }

    /**
     * @brief Subtracts all of the current instance's components by a value.
     * @param value The value to subtract by.
     * @return A reference to this instance.
     */
    vector3& operator -=(Type value) {
        x -= value;
        y -= value;
        z -= value;

        return *this;
    }

    /**
     * @brief Multiplies all of the current instance's components by a value.
     * @param value The value to multiply by.
     * @return A reference to this instance.
     */
    vector3& operator *=(Type value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    /**
     * @brief Divides all of the current instance's components by a value.
     * @param value The value to divide by.
     * @return A reference to this instance.
     */
    vector3& operator /=(Type value) {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }

    /**
     * @brief Tests if this vector3 is equal to an other one.
     * @param other The vector3 to compare with.
     * @return Whether the two vector3 are equal.
     */
    bool operator ==(const vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    /**
     * @brief Tests if this vector3 is different than an other one.
     * @param other The vector3 to compare with.
     * @return Whether the two vector3 are different.
     */
    bool operator !=(const vector3& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    Type x; ///< The x component of the vector3.
    Type y; ///< The y component of the vector3.
    Type z; ///< The z component of the vector3.
};

/**
 * @brief Writes the components of the given vector3 to the output stream in the format
 * "( x ; y ; z )".
 * @param stream The output stream to write to.
 * @param vec The vector3 to write to the stream.
 * @return A reference to the output stream after writing the vector3.
 */
template <typename Type>
std::ostream& operator <<(std::ostream& stream, const vector3<Type>& vec) {
    stream << "( " << vec.x << " ; " << vec.y << " ; " << vec.z << " )";
    return stream;
}

/**
 * @brief Reads four values from the input stream and assigns them to the x, y and z components of
 * the given vector3.
 * @param stream The input stream to read from.
 * @param vec The vector3 to assign the read values to.
 * @return A reference to the input stream after reading the values and assigning them to vector3.
 */
template <typename Type>
std::istream& operator >>(std::istream& stream, vector3<Type>& vec) {
    stream >> vec.x >> vec.y >> vec.z;
    return stream;
}

/** @brief Adds a vector3's components to another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise sum of the two vector3.
 */
template <typename Type>
vector3<Type> operator +(const vector3<Type>& left, const vector3<Type>& right) {
    return vector3<Type>(
        left.x + right.x,
        left.y + right.y,
        left.z + right.z
    );
}

/** @brief Subtracts a vector3's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise subtraction of the first vector3 by the second.
 */
template <typename Type>
vector3<Type> operator -(const vector3<Type>& left, const vector3<Type>& right) {
    return vector3<Type>(
        left.x - right.x,
        left.y - right.y,
        left.z - right.z
    );
}

/** @brief Multiplies a vector3's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise product of the two vector3.
 */
template <typename Type>
vector3<Type> operator *(const vector3<Type>& left, const vector3<Type>& right) {
    return vector3<Type>(
        left.x * right.x,
        left.y * right.y,
        left.z * right.z
    );
}

/** @brief Divides a vector3's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise division of the first vector3 by the second.
 */
template <typename Type>
vector3<Type> operator /(const vector3<Type>& left, const vector3<Type>& right) {
    return vector3<Type>(
        left.x / right.x,
        left.y / right.y,
        left.z / right.z
    );
}

/** @brief Adds a value to each of a vector3's components.
 *  @param vec The vector3.
 *  @param value The value.
 *  @return The component-wise sum of a vector3 by a value.
 */
template <typename Type>
vector3<Type> operator +(const vector3<Type>& vec, Type value) {
    return vector3<Type>(
        vec.x + value,
        vec.y + value,
        vec.z + value
    );
}

/** @brief Subtracts each of a vector3's components by a value.
 *  @param vec The vector3.
 *  @param value The value.
 *  @return The component-wise subtraction of a vector3 by a value.
 */
template <typename Type>
vector3<Type> operator -(const vector3<Type>& vec, Type value) {
    return vector3<Type>(
        vec.x - value,
        vec.y - value,
        vec.z - value
    );
}

/** @brief Multiplies each of a vector3's components by a value.
 *  @param vec The vector3.
 *  @param value The value.
 *  @return The component-wise product of a vector3 by a value.
 */
template <typename Type>
vector3<Type> operator *(const vector3<Type>& vec, Type value) {
    return vector3<Type>(
        vec.x * value,
        vec.y * value,
        vec.z * value
    );
}

/** @brief Multiplies each of a vector3's components by a value.
 *  @param value The value.
 *  @param vec The vector3.
 *  @return The component-wise product of a vector3 by a value.
 */
template <typename Type>
vector3<Type> operator *(Type value, const vector3<Type>& vec) {
    return vector3<Type>(
        value * vec.x,
        value * vec.y,
        value * vec.z
    );
}

/** @brief Divides each of a vector3's components by a value.
 *  @param vec The vector3.
 *  @param value The value.
 *  @return The component-wise division of a vector3 by a value.
 */
template <typename Type>
vector3<Type> operator /(const vector3<Type>& vec, Type value) {
    return vector3<Type>(
        vec.x / value,
        vec.y / value,
        vec.z / value
    );
}

/**
 * @brief Multiplies all of a vector3's components by -1.
 *  @param vec The vector3.
 *  @return The component-wise product of a vector3 by -1.
 */
template <typename Type>
vector3<Type> operator -(const vector3<Type>& vec) {
    return vector3(-vec.x, -vec.y, -vec.z);
}
