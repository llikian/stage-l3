/***************************************************************************************************
 * @file  vector4.hpp
 * @brief Declaration of the vector4 class
 **************************************************************************************************/

#pragma once

#include <iostream>

#include "vector3.hpp"

/**
 * @class vector4
 * @brief Holds 4 values of the same type.
 * @tparam Type The type of the vector's components.
 */
template <typename Type>
struct vector4 {
    /**
     * @brief Constructs a vector4 with all components set to 0 (or default initialized in the case of a class).
     */
    vector4() : x(), y(), z(), w() { }

    /**
     * @brief Constructs a vector4 with a specific value for each component.
     * @param x The value of the x component.
     * @param y The value of the y component.
     * @param z The value of the z component.
     * @param w The value of the w component.
     */
    vector4(Type x, Type y, Type z, Type w) : x(x), y(y), z(z), w(w) { }

    /**
     * @brief Constructs a vector4 with its first 2 components specified by a vector2 and its last 2
     * by explicit values.
     * @param xy The value of the x and y components.
     * @param z The value of the z component.
     * @param w The value of the w component.
     */
    vector4(const vector2<Type>& xy, float z, float w) : x(xy.x), y(xy.y), z(z), w(w) { }

    /**
     * @brief Constructs a vector4 with its first 2 components specified by a vector2 and its last 2
     * by another vector2.
     * @param xy The value of the x and y components.
     * @param zw The value of the z and w components.
     */
    vector4(const vector2<Type>& xy, const vector2<Type> zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }

    /**
     * @brief Constructs a vector4 with its first 3 components specified by a vector3 and its last
     * one by an explicit value.
     * @param xyz The value of the x, y and z components
     * @param w The value of the w component.
     */
    vector4(const vector3<Type>& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }

    /**
     * @brief Constructs a vector4 with the same value for each component.
     * @param value The value of each component.
     */
    explicit vector4(Type value) : x(value), y(value), z(value), w(value) { }

    /**
     * @brief Adds another vector4's components to the current instance's components.
     * @param vec The vector4 to add.
     * @return A reference to this instance.
     */
    vector4& operator +=(const vector4& vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        w += vec.w;

        return *this;
    }

    /**
     * @brief Subtracts the current instance's components by another vector4's components.
     * @param vec The vector4 to subtract by.
     * @return A reference to this instance.
     */
    vector4& operator -=(const vector4& vec) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        w -= vec.w;

        return *this;
    }

    /**
     * @brief Multiplies the current instance's components by another vector4's components.
     * @param vec The vector4 to multiply by.
     * @return A reference to this instance.
     */
    vector4& operator *=(const vector4& vec) {
        x *= vec.x;
        y *= vec.y;
        z *= vec.z;
        w *= vec.w;

        return *this;
    }

    /**
     * @brief Divides the current instance's components by another vector4's components.
     * @param vec The vector4 to divide by.
     * @return A reference to this instance.
     */
    vector4& operator /=(const vector4& vec) {
        x /= vec.x;
        y /= vec.y;
        z /= vec.z;
        w /= vec.w;

        return *this;
    }

    /**
     * @brief Adds a value to all of the current instance's components.
     * @param value The value to add.
     * @return A reference to this instance.
     */
    vector4& operator +=(Type value) {
        x += value;
        y += value;
        z += value;
        w += value;

        return *this;
    }

    /**
     * @brief Subtracts all of the current instance's components by a value.
     * @param value The value to subtract by.
     * @return A reference to this instance.
     */
    vector4& operator -=(Type value) {
        x -= value;
        y -= value;
        z -= value;
        w -= value;

        return *this;
    }

    /**
     * @brief Multiplies all of the current instance's components by a value.
     * @param value The value to multiply by.
     * @return A reference to this instance.
     */
    vector4& operator *=(Type value) {
        x *= value;
        y *= value;
        z *= value;
        w *= value;

        return *this;
    }

    /**
     * @brief Divides all of the current instance's components by a value.
     * @param value The value to divide by.
     * @return A reference to this instance.
     */
    vector4& operator /=(Type value) {
        x /= value;
        y /= value;
        z /= value;
        w /= value;

        return *this;
    }

    /**
     * @brief Tests if this vector4 is equal to an other one.
     * @param other The vector4 to compare with.
     * @return Whether the two vector4 are equal.
     */
    bool operator ==(const vector4& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    /**
     * @brief Tests if this vector4 is different than an other one.
     * @param other The vector4 to compare with.
     * @return Whether the two vector4 are different.
     */
    bool operator !=(const vector4& other) const {
        return x != other.x || y != other.y || z != other.z || w != other.w;
    }

    Type x; ///< The x component of the vector4.
    Type y; ///< The y component of the vector4.
    Type z; ///< The z component of the vector4.
    Type w; ///< The w component of the vector4.
};

/**
 * @brief Writes the components of the given vector4 to the output stream in the format
 * "( x ; y ; z ; w )".
 * @param stream The output stream to write to.
 * @param vec The vector4 to write to the stream.
 * @return A reference to the output stream after writing the vector4.
 */
template <typename Type>
std::ostream& operator <<(std::ostream& stream, const vector4<Type>& vec) {
    stream << "( " << vec.x << " ; " << vec.y << " ; " << vec.z << " ; " << vec.w << " )";
    return stream;
}

/**
 * @brief Reads four values from the input stream and assigns them to the x, y, z and w components
 * of the given vector4.
 * @param stream The input stream to read from.
 * @param vec The vector4 to assign the read values to.
 * @return A reference to the input stream after reading the values and assigning them to vector4.
 */
template <typename Type>
std::istream& operator >>(std::istream& stream, vector4<Type>& vec) {
    stream >> vec.x >> vec.y >> vec.z >> vec.w;
    return stream;
}

/** @brief Adds a vector4's components to another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise sum of the two vector4.
 */
template <typename Type>
vector4<Type> operator +(const vector4<Type>& left, const vector4<Type>& right) {
    return vector4<Type>(
        left.x + right.x,
        left.y + right.y,
        left.z + right.z,
        left.w + right.w
    );
}

/** @brief Subtracts a vector4's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise subtraction of the first vector4 by the second.
 */
template <typename Type>
vector4<Type> operator -(const vector4<Type>& left, const vector4<Type>& right) {
    return vector4<Type>(
        left.x - right.x,
        left.y - right.y,
        left.z - right.z,
        left.w - right.w
    );
}

/** @brief Multiplies a vector4's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise product of the two vector4.
 */
template <typename Type>
vector4<Type> operator *(const vector4<Type>& left, const vector4<Type>& right) {
    return vector4<Type>(
        left.x * right.x,
        left.y * right.y,
        left.z * right.z,
        left.w * right.w
    );
}

/** @brief Divides a vector4's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise division of the first vector4 by the second.
 */
template <typename Type>
vector4<Type> operator /(const vector4<Type>& left, const vector4<Type>& right) {
    return vector4<Type>(
        left.x / right.x,
        left.y / right.y,
        left.z / right.z,
        left.w / right.w
    );
}

/** @brief Adds a value to each of a vector4's components.
 *  @param vec The vector4.
 *  @param value The value.
 *  @return The component-wise sum of a vector4 by a value.
 */
template <typename Type>
vector4<Type> operator +(const vector4<Type>& vec, Type value) {
    return vector4<Type>(
        vec.x + value,
        vec.y + value,
        vec.z + value,
        vec.w + value
    );
}

/** @brief Subtracts each of a vector4's components by a value.
 *  @param vec The vector4.
 *  @param value The value.
 *  @return The component-wise subtraction of a vector4 by a value.
 */
template <typename Type>
vector4<Type> operator -(const vector4<Type>& vec, Type value) {
    return vector4<Type>(
        vec.x - value,
        vec.y - value,
        vec.z - value,
        vec.w - value
    );
}

/** @brief Multiplies each of a vector4's components by a value.
 *  @param vec The vector4.
 *  @param value The value.
 *  @return The component-wise product of a vector4 by a value.
 */
template <typename Type>
vector4<Type> operator *(const vector4<Type>& vec, Type value) {
    return vector4<Type>(
        vec.x * value,
        vec.y * value,
        vec.z * value,
        vec.w * value
    );
}

/** @brief Multiplies each of a vector4's components by a value.
 *  @param value The value.
 *  @param vec The vector4.
 *  @return The component-wise product of a vector4 by a value.
 */
template <typename Type>
vector4<Type> operator *(Type value, const vector4<Type>& vec) {
    return vector4<Type>(
        value * vec.x,
        value * vec.y,
        value * vec.z,
        value * vec.w
    );
}

/** @brief Divides each of a vector4's components by a value.
 *  @param vec The vector4.
 *  @param value The value.
 *  @return The component-wise division of a vector4 by a value.
 */
template <typename Type>
vector4<Type> operator /(const vector4<Type>& vec, Type value) {
    return vector4<Type>(
        vec.x / value,
        vec.y / value,
        vec.z / value,
        vec.w / value
    );
}

/**
 * @brief Multiplies all of a vector4's components by -1.
 *  @param vec The vector4.
 *  @return The component-wise product of a vector4 by -1.
 */
template <typename Type>
vector4<Type> operator -(const vector4<Type>& vec) {
    return vector4(-vec.x, -vec.y, -vec.z, -vec.w);
}
