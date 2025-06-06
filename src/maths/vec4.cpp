/***************************************************************************************************
 * @file  vec4.cpp
 * @brief Implementation of the vec4 struct
 **************************************************************************************************/

#include "maths/vec4.hpp"

#include <random>

vec4::vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }

vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

vec4::vec4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) { }

vec4& vec4::operator +=(const vec4& vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    w += vec.w;

    return *this;
}

vec4& vec4::operator -=(const vec4& vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    w -= vec.w;

    return *this;
}

vec4& vec4::operator *=(const vec4& vec) {
    x *= vec.x;
    y *= vec.y;
    z *= vec.z;
    w *= vec.w;

    return *this;
}

vec4& vec4::operator /=(const vec4& vec) {
    x /= vec.x;
    y /= vec.y;
    z /= vec.z;
    w /= vec.w;

    return *this;
}

vec4& vec4::operator +=(float scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    w += scalar;

    return *this;
}

vec4& vec4::operator -=(float scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    w -= scalar;

    return *this;
}

vec4& vec4::operator *=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;

    return *this;
}

vec4& vec4::operator /=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;

    return *this;
}

std::ostream& operator <<(std::ostream& stream, const vec4& vec) {
    stream << "( " << vec.x << " ; " << vec.y << " ; " << vec.z << " ; " << vec.w << " )";
    return stream;
}

std::istream& operator >>(std::istream& stream, vec4& vec) {
    stream >> vec.x >> vec.y >> vec.z >> vec.w;
    return stream;
}

vec4 operator +(const vec4& left, const vec4& right) {
    return vec4(
        left.x + right.x,
        left.y + right.y,
        left.z + right.z,
        left.w + right.w
    );
}

vec4 operator -(const vec4& left, const vec4& right) {
    return vec4(
        left.x - right.x,
        left.y - right.y,
        left.z - right.z,
        left.w - right.w
    );
}

vec4 operator *(const vec4& left, const vec4& right) {
    return vec4(
        left.x * right.x,
        left.y * right.y,
        left.z * right.z,
        left.w * right.w
    );
}

vec4 operator /(const vec4& left, const vec4& right) {
    return vec4(
        left.x / right.x,
        left.y / right.y,
        left.z / right.z,
        left.w / right.w
    );
}

vec4 operator +(const vec4& vec, float scalar) {
    return vec4(
        vec.x + scalar,
        vec.y + scalar,
        vec.z + scalar,
        vec.w + scalar
    );
}

vec4 operator -(const vec4& vec, float scalar) {
    return vec4(
        vec.x - scalar,
        vec.y - scalar,
        vec.z - scalar,
        vec.w - scalar
    );
}

vec4 operator *(const vec4& vec, float scalar) {
    return vec4(
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar,
        vec.w * scalar
    );
}

vec4 operator *(float scalar, const vec4& vec) {
    return vec4(
        scalar * vec.x,
        scalar * vec.y,
        scalar * vec.z,
        scalar * vec.w
    );
}

vec4 operator /(const vec4& vec, float scalar) {
    return vec4(
        vec.x / scalar,
        vec.y / scalar,
        vec.z / scalar,
        vec.w / scalar
    );
}

bool operator ==(const vec4& left, const vec4& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
}

bool operator !=(const vec4& left, const vec4& right) {
    return left.x != right.x || left.y != right.y || left.z != right.z || left.w != right.w;
}
