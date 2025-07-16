/***************************************************************************************************
 * @file  quaternion.cpp
 * @brief Implementation of the quaternion struct
 **************************************************************************************************/

#include "maths/quaternion.hpp"

#include <cmath>

#include "maths/constants.hpp"
#include "maths/trigonometry.hpp"
#include "maths/vec2.hpp"

quaternion::quaternion() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }

quaternion::quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

quaternion& quaternion::operator +=(const quaternion& q) {
    x += q.x;
    y += q.y;
    z += q.z;
    w += q.w;

    return *this;
}

quaternion& quaternion::operator -=(const quaternion& q) {
    x -= q.x;
    y -= q.y;
    z -= q.z;
    w -= q.w;

    return *this;;
}

quaternion& quaternion::operator *=(const quaternion& q) {
    x = y * q.z - z * q.y + x * q.w + w * q.x;
    y = z * q.x - x * q.z + y * q.w + w * q.y;
    z = x * q.y - y * q.x + z * q.w + w * q.z;
    w = w * q.w - x * q.x - y * q.y - z * q.z;

    return *this;
}

quaternion& quaternion::operator /=(const quaternion& q) {
    quaternion inv = q.get_inverse();

    x = y * inv.z - z * inv.y + x * inv.w + w * inv.x;
    y = z * inv.x - x * inv.z + y * inv.w + w * inv.y;
    z = x * inv.y - y * inv.x + z * inv.w + w * inv.z;
    w = w * inv.w - x * inv.x - y * inv.y - z * inv.z;

    return *this;
}

quaternion& quaternion::operator *=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;

    return *this;;
}

quaternion& quaternion::operator /=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;

    return *this;;
}

float quaternion::get_length() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

quaternion quaternion::get_conjugate() const {
    return quaternion(-x, -y, -z, w);
}

quaternion quaternion::get_inverse() const {
    float length = get_length();
    length = 1.0f / (length * length);
    return quaternion(-length * x, -length * y, -length * z, length * w);
}

mat4 quaternion::get_matrix() const {
    float length = get_length();
    float s = 2.0f / (length * length);
    return mat4(
        1.0f - s * (y * y + z * z), s * (x * y - w * z), s * (x * z + w * y),
        s * (x * y + w * z), 1.0f - s * (x * x + z * z), s * (y * z - w * x),
        s * (x * z - w * y), s * (y * z + w * x), 1.0f - s * (x * x + y * y)
    );
}

void quaternion::normalize() {
    float length = get_length();
    x /= length;
    y /= length;
    z /= length;
    w /= length;
}

quaternion euler_to_quaternion(const vec3& angles) {
    vec3 cosine, sine;

    if(angles.x == 0.0f) {
        cosine.x = 1.0f;
        sine.x = 0.0f;
    } else {
        float radians = degrees_to_radians(angles.x) / 2.0f;
        cosine.x = std::cos(radians);
        sine.x = std::sin(radians);
    }

    if(angles.y == 0.0f) {
        cosine.y = 1.0f;
        sine.y = 0.0f;
    } else {
        float radians = degrees_to_radians(angles.y) / 2.0f;
        cosine.y = std::cos(radians);
        sine.y = std::sin(radians);
    }

    if(angles.z == 0.0f) {
        cosine.z = 1.0f;
        sine.z = 0.0f;
    } else {
        float radians = degrees_to_radians(angles.z) / 2.0f;
        cosine.z = std::cos(radians);
        sine.z = std::sin(radians);
    }

    return quaternion(
        sine.x * cosine.y * cosine.z - cosine.x * sine.y * sine.z,
        cosine.x * sine.y * cosine.z + sine.x * cosine.y * sine.z,
        cosine.x * cosine.y * sine.z - sine.x * sine.y * cosine.z,
        cosine.x * cosine.y * cosine.z + sine.x * sine.y * sine.z
    );
}

vec3 quaternion_to_euler(const quaternion& q) {
    vec3 radians;

    radians.x = std::atan2(2.0f * (q.w * q.x + q.y * q.z),
                           1.0f - 2.0f * (q.x * q.x + q.y * q.y));
    radians.y = -PI_HALF_F + 2.0f * std::atan2(std::sqrt(1.0f + 2.0f * (q.w * q.y - q.x * q.z)),
                                               std::sqrt(1.0f - 2.0f * (q.w * q.y - q.x * q.z)));
    radians.z = std::atan2(2.0f * (q.w * q.z + q.x * q.y),
                           1.0f - 2.0f * (q.y * q.y + q.z * q.z));

    return vec3(radians_to_degrees(radians.x), radians_to_degrees(radians.y), radians_to_degrees(radians.z));
}

std::ostream& operator <<(std::ostream& stream, const quaternion& q) {
    std::cout << q.x << 'i'
        << ((q.y >= 0) ? " + " : " - ") << fabs(q.y) << 'j'
        << ((q.z >= 0) ? " + " : " - ") << fabs(q.y) << 'k'
        << ((q.w >= 0) ? " + " : " - ") << fabs(q.y);

    return stream;
}

std::istream& operator >>(std::istream& stream, quaternion& q) {
    return stream >> q.x >> q.y >> q.z >> q.w;
}

quaternion operator +(const quaternion& q, const quaternion& r) {
    return quaternion(q.x + r.x, q.y + r.y, q.z + r.z, q.w + r.w);
}

quaternion operator -(const quaternion& q, const quaternion& r) {
    return quaternion(q.x - r.x, q.y - r.y, q.z - r.z, q.w - r.w);
}

quaternion operator *(const quaternion& q, const quaternion& r) {
    return quaternion(
        q.y * r.z - q.z * r.y + q.x * r.w + q.w * r.x,
        q.z * r.x - q.x * r.z + q.y * r.w + q.w * r.y,
        q.x * r.y - q.y * r.x + q.z * r.w + q.w * r.z,
        q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z
    );
}

quaternion operator /(const quaternion& q, const quaternion& r) {
    return q * r.get_inverse();
}

quaternion operator *(const quaternion& q, float scalar) {
    return quaternion(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
}

quaternion operator *(float scalar, const quaternion& q) {
    return quaternion(scalar * q.x, scalar * q.y, scalar * q.z, scalar * q.w);
}

quaternion operator /(const quaternion& q, float scalar) {
    return quaternion(q.x / scalar, q.y / scalar, q.z / scalar, q.w / scalar);
}

quaternion operator /(float scalar, const quaternion& q) {
    return scalar * q.get_inverse();
}
