/***************************************************************************************************
 * @file  vector2.hpp
 * @brief Declaration of the vector2 class
 **************************************************************************************************/

#pragma once

#include <iostream>

template <typename Type> struct vector3; // Forward Declaration of vector3 to avoid circular inclusion.
template <typename Type> struct vector4; // Forward Declaration of vector4 to avoid circular inclusion.

/**
 * @class vector2
 * @brief Holds 2 values of the same type.
 * @tparam Type The type of the vector's components.
 */
template <typename Type>
struct vector2 {
    /**
     * @brief Constructs a vector2 with all components set to 0 (or default initialized in the case of a class).
     */
    vector2() : x(), y() { }

    /**
     * @brief Constructs a vector2 with a specific value for each component.
     * @param x The value of the x component.
     * @param y The value of the y component.
     */
    vector2(Type x, Type y) : x(x), y(y) { }

    /**
     * @brief Constructs a vector2 with its components specified by a vector3's first 2 components.
     * @param xyz The value of the xy (and the ignored z) components.
     */
    vector2(const vector3<Type>& xyz) : x(xyz.x), y(xyz.y) { }

    /**
     * @brief Constructs a vector2 with its components specified by a vector4's first 2 components.
     * @param xyzw The value of the xy (and the ignored z and w) components.
     */
    vector2(const vector4<Type>& xyzw) : x(xyzw.x), y(xyzw.y) { }

    /**
     * @brief Constructs a vector2 with the same value for each component.
     * @param value The value of each component.
     */
    explicit vector2(Type value) : x(value), y(value) { }

    /**
     * @brief Adds another vector2's components to the current instance's components.
     * @param vec The vector2 to add.
     * @return A reference to this instance.
     */
    vector2& operator +=(const vector2& vec) {
        x += vec.x;
        y += vec.y;

        return *this;
    }

    /**
     * @brief Subtracts the current instance's components by another vector2's components.
     * @param vec The vector2 to subtract by.
     * @return A reference to this instance.
     */
    vector2& operator -=(const vector2& vec) {
        x -= vec.x;
        y -= vec.y;

        return *this;
    }

    /**
     * @brief Multiplies the current instance's components by another vector2's components.
     * @param vec The vector2 to multiply by.
     * @return A reference to this instance.
     */
    vector2& operator *=(const vector2& vec) {
        x *= vec.x;
        y *= vec.y;

        return *this;
    }

    /**
     * @brief Divides the current instance's components by another vector2's components.
     * @param vec The vector2 to divide by.
     * @return A reference to this instance.
     */
    vector2& operator /=(const vector2& vec) {
        x /= vec.x;
        y /= vec.y;

        return *this;
    }

    /**
     * @brief Adds a value to all of the current instance's components.
     * @param value The value to add.
     * @return A reference to this instance.
     */
    vector2& operator +=(Type value) {
        x += value;
        y += value;

        return *this;
    }

    /**
     * @brief Subtracts all of the current instance's components by a value.
     * @param value The value to subtract by.
     * @return A reference to this instance.
     */
    vector2& operator -=(Type value) {
        x -= value;
        y -= value;

        return *this;
    }

    /**
     * @brief Multiplies all of the current instance's components by a value.
     * @param value The value to multiply by.
     * @return A reference to this instance.
     */
    vector2& operator *=(Type value) {
        x *= value;
        y *= value;

        return *this;
    }

    /**
     * @brief Divides all of the current instance's components by a value.
     * @param value The value to divide by.
     * @return A reference to this instance.
     */
    vector2& operator /=(Type value) {
        x /= value;
        y /= value;

        return *this;
    }

    /**
     * @brief Tests if this vector2 is equal to an other one.
     * @param other The vector2 to compare with.
     * @return Whether the two vector2 are equal.
     */
    bool operator ==(const vector2& other) const {
        return x == other.x && y == other.y;
    }

    /**
     * @brief Tests if this vector2 is different than an other one.
     * @param other The vector2 to compare with.
     * @return Whether the two vector2 are different.
     */
    bool operator !=(const vector2& other) const {
        return x != other.x || y != other.y;
    }

    Type x; ///< The x component of the vector2.
    Type y; ///< The y component of the vector2.
};

/**
 * @brief Writes the components of the given vector2 to the output stream in the format "( x ; y )".
 * @param stream The output stream to write to.
 * @param vec The vector2 to write to the stream.
 * @return A reference to the output stream after writing the vector2.
 */
template <typename Type>
std::ostream& operator <<(std::ostream& stream, const vector2<Type>& vec) {
    stream << "( " << vec.x << " ; " << vec.y << " )";
    return stream;
}

/**
 * @brief Reads four values from the input stream and assigns them to the x and y components of the
 * given vector2.
 * @param stream The input stream to read from.
 * @param vec The vector2 to assign the read values to.
 * @return A reference to the input stream after reading the values and assigning them to vector2.
 */
template <typename Type>
std::istream& operator >>(std::istream& stream, vector2<Type>& vec) {
    stream >> vec.x >> vec.y;
    return stream;
}

/** @brief Adds a vector2's components to another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise sum of the two vector2.
 */
template <typename Type>
vector2<Type> operator +(const vector2<Type>& left, const vector2<Type>& right) {
    return vector2<Type>(
        left.x + right.x,
        left.y + right.y
    );
}

/** @brief Subtracts a vector2's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise subtraction of the first vector2 by the second.
 */
template <typename Type>
vector2<Type> operator -(const vector2<Type>& left, const vector2<Type>& right) {
    return vector2<Type>(
        left.x - right.x,
        left.y - right.y
    );
}

/** @brief Multiplies a vector2's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise product of the two vector2.
 */
template <typename Type>
vector2<Type> operator *(const vector2<Type>& left, const vector2<Type>& right) {
    return vector2<Type>(
        left.x * right.x,
        left.y * right.y
    );
}

/** @brief Divides a vector2's components by another's.
 *  @param left The left operand.
 *  @param right The right operand.
 *  @return The component-wise division of the first vector2 by the second.
 */
template <typename Type>
vector2<Type> operator /(const vector2<Type>& left, const vector2<Type>& right) {
    return vector2<Type>(
        left.x / right.x,
        left.y / right.y
    );
}

/** @brief Adds a value to each of a vector2's components.
 *  @param vec The vector2.
 *  @param value The value.
 *  @return The component-wise sum of a vector2 by a value.
 */
template <typename Type>
vector2<Type> operator +(const vector2<Type>& vec, Type value) {
    return vector2<Type>(
        vec.x + value,
        vec.y + value
    );
}

/** @brief Subtracts each of a vector2's components by a value.
 *  @param vec The vector2.
 *  @param value The value.
 *  @return The component-wise subtraction of a vector2 by a value.
 */
template <typename Type>
vector2<Type> operator -(const vector2<Type>& vec, Type value) {
    return vector2<Type>(
        vec.x - value,
        vec.y - value
    );
}

/** @brief Multiplies each of a vector2's components by a value.
 *  @param vec The vector2.
 *  @param value The value.
 *  @return The component-wise product of a vector2 by a value.
 */
template <typename Type>
vector2<Type> operator *(const vector2<Type>& vec, Type value) {
    return vector2<Type>(
        vec.x * value,
        vec.y * value
    );
}

/** @brief Multiplies each of a vector2's components by a value.
 *  @param value The value.
 *  @param vec The vector2.
 *  @return The component-wise product of a vector2 by a value.
 */
template <typename Type>
vector2<Type> operator *(Type value, const vector2<Type>& vec) {
    return vector2<Type>(
        value * vec.x,
        value * vec.y
    );
}

/** @brief Divides each of a vector2's components by a value.
 *  @param vec The vector2.
 *  @param value The value.
 *  @return The component-wise division of a vector2 by a value.
 */
template <typename Type>
vector2<Type> operator /(const vector2<Type>& vec, Type value) {
    return vector2<Type>(
        vec.x / value,
        vec.y / value
    );
}

/**
 * @brief Multiplies all of a vector2's components by -1.
 *  @param vec The vector2.
 *  @return The component-wise product of a vector2 by -1.
 */
template <typename Type>
vector2<Type> operator -(const vector2<Type>& vec) {
    return vector2(-vec.x, -vec.y);
}