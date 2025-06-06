/***************************************************************************************************
 * @file  vec2.cpp
 * @brief Implementation of the vec2 struct
 **************************************************************************************************/

#include "maths/vec2.hpp"

#include <random>

vec2::vec2() : x(0.0f), y(0.0f) { }

vec2::vec2(float x, float y) : x(x), y(y) { }

vec2::vec2(float scalar) : x(scalar), y(scalar) { }

vec2& vec2::operator +=(const vec2& vec) {
    x += vec.x;
    y += vec.y;

    return *this;
}

vec2& vec2::operator -=(const vec2& vec) {
    x -= vec.x;
    y -= vec.y;

    return *this;
}

vec2& vec2::operator *=(const vec2& vec) {
    x *= vec.x;
    y *= vec.y;

    return *this;
}

vec2& vec2::operator /=(const vec2& vec) {
    x /= vec.x;
    y /= vec.y;

    return *this;
}

vec2& vec2::operator +=(float scalar) {
    x += scalar;
    y += scalar;

    return *this;
}

vec2& vec2::operator -=(float scalar) {
    x -= scalar;
    y -= scalar;

    return *this;
}

vec2& vec2::operator *=(float scalar) {
    x *= scalar;
    y *= scalar;

    return *this;
}

vec2& vec2::operator /=(float scalar) {
    x /= scalar;
    y /= scalar;

    return *this;
}

std::ostream& operator <<(std::ostream& stream, const vec2& vec) {
    stream << "( " << vec.x << " ; " << vec.y << " )";
    return stream;
}

std::istream& operator >>(std::istream& stream, vec2& vec) {
    stream >> vec.x >> vec.y;
    return stream;
}

vec2 operator +(const vec2& left, const vec2& right) {
    return vec2(
        left.x + right.x,
        left.y + right.y
    );
}

vec2 operator -(const vec2& left, const vec2& right) {
    return vec2(
        left.x - right.x,
        left.y - right.y
    );
}

vec2 operator *(const vec2& left, const vec2& right) {
    return vec2(
        left.x * right.x,
        left.y * right.y
    );
}

vec2 operator /(const vec2& left, const vec2& right) {
    return vec2(
        left.x / right.x,
        left.y / right.y
    );
}

vec2 operator +(const vec2& vec, float scalar) {
    return vec2(
        vec.x + scalar,
        vec.y + scalar
    );
}

vec2 operator -(const vec2& vec, float scalar) {
    return vec2(
        vec.x - scalar,
        vec.y - scalar
    );
}

vec2 operator *(const vec2& vec, float scalar) {
    return vec2(
        vec.x * scalar,
        vec.y * scalar
    );
}

vec2 operator *(float scalar, const vec2& vec) {
    return vec2(
        scalar * vec.x,
        scalar * vec.y
    );
}

vec2 operator /(const vec2& vec, float scalar) {
    return vec2(
        vec.x / scalar,
        vec.y / scalar
    );
}

bool operator ==(const vec2& left, const vec2& right) {
    return left.x == right.x && left.y == right.y;
}

bool operator !=(const vec2& left, const vec2& right) {
    return left.x != right.x || left.y != right.y;
}
