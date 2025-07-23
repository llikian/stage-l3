/***************************************************************************************************
 * @file  transforms.cpp
 * @brief Implementation of the Image class
 **************************************************************************************************/

#include "maths/transforms.hpp"

#include <cmath>
#include "maths/geometry.hpp"
#include "maths/trigonometry.hpp"

mat4 scale(float factor) {
    return mat4(
        factor, 0.0f, 0.0f,
        0.0f, factor, 0.0f,
        0.0f, 0.0f, factor
    );
}

mat4 scale(float x, float y, float z) {
    return mat4(
        x, 0.0f, 0.0f,
        0.0f, y, 0.0f,
        0.0f, 0.0f, z
    );
}

mat4 scale(const vec3& factors) {
    return mat4(
        factors.x, 0.0f, 0.0f,
        0.0f, factors.y, 0.0f,
        0.0f, 0.0f, factors.z
    );
}

mat4 scale_x(float factor) {
    return mat4(
        factor, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    );
}

mat4 scale_y(float factor) {
    return mat4(
        1.0f, 0.0f, 0.0f,
        0.0f, factor, 0.0f,
        0.0f, 0.0f, 1.0f
    );
}

mat4 scale_z(float factor) {
    return mat4(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, factor
    );
}

mat4 translate(const vec3& vector) {
    return mat4(1.0f, 0.0f, 0.0f, vector.x,
                0.0f, 1.0f, 0.0f, vector.y,
                0.0f, 0.0f, 1.0f, vector.z,
                0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 translate(float x, float y, float z) {
    return mat4(1.0f, 0.0f, 0.0f, x,
                0.0f, 1.0f, 0.0f, y,
                0.0f, 0.0f, 1.0f, z,
                0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 translate_x(float scalar) {
    return mat4(1.0f, 0.0f, 0.0f, scalar,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 translate_y(float scalar) {
    return mat4(1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, scalar,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 translate_z(float scalar) {
    return mat4(1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, scalar,
                0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 rotate(float angle, vec3 axis) {
    angle = degrees_to_radians(angle);
    float cosine = std::cos(angle);
    float sine = std::sin(angle);

    if(float len = length(axis) ; len != 0.0f) { axis /= len; }
    vec3 temp = (1.0f - cosine) * axis;

    return mat4(
        axis.x * temp.x + cosine, axis.x * temp.y - axis.z * sine, axis.x * temp.z + axis.y * sine,
        axis.y * temp.x + axis.z * sine, axis.y * temp.y + cosine, axis.y * temp.z - axis.x * sine,
        axis.z * temp.x - axis.y * sine, axis.z * temp.y + axis.x * sine, axis.z * temp.z + cosine
    );
}

mat4 rotate_x(float angle) {
    angle = degrees_to_radians(angle);

    const float cosine = std::cos(angle);
    const float sine = std::sin(angle);

    return mat4(
        1.0f, 0.0f, 0.0f,
        0.0f, cosine, -sine,
        0.0f, sine, cosine
    );
}

mat4 rotate_y(float angle) {
    angle = degrees_to_radians(angle);

    const float cosine = std::cos(angle);
    const float sine = std::sin(angle);

    return mat4(
        cosine, 0.0f, sine,
        0.0f, 1.0f, 0.0f,
        -sine, 0.0f, cosine
    );
}

mat4 rotate_z(float angle) {
    angle = degrees_to_radians(angle);

    const float cosine = std::cos(angle);
    const float sine = std::sin(angle);

    return mat4(
        cosine, -sine, 0.0f,
        sine, cosine, 0.0f,
        0.0f, 0.0f, 1.0f
    );
}

mat4 TRS_matrix(const vec3& translation, const vec3& rotation, const vec3& scale) {
    vec3 cosine, sine;

    if(rotation.x == 0.0f) {
        cosine.x = 1.0f;
        sine.x = 0.0f;
    } else {
        float radians = degrees_to_radians(rotation.x);
        cosine.x = std::cos(radians);
        sine.x = std::sin(radians);
    }

    if(rotation.y == 0.0f) {
        cosine.y = 1.0f;
        sine.y = 0.0f;
    } else {
        float radians = degrees_to_radians(rotation.y);
        cosine.y = std::cos(radians);
        sine.y = std::sin(radians);
    }

    if(rotation.z == 0.0f) {
        cosine.z = 1.0f;
        sine.z = 0.0f;
    } else {
        float radians = degrees_to_radians(rotation.z);
        cosine.z = std::cos(radians);
        sine.z = std::sin(radians);
    }

    return mat4(
        scale.x * (cosine.y * cosine.z + sine.x * sine.y * sine.z),
        scale.y * (-cosine.y * sine.z + sine.x * sine.y * cosine.z),
        scale.z * cosine.x * sine.y,
        translation.x,

        scale.x * cosine.x * sine.z,
        scale.y * cosine.x * cosine.z,
        -scale.z * sine.x,
        translation.y,

        scale.x * (-sine.y * cosine.z + sine.x * cosine.y * sine.z),
        scale.y * (sine.y * sine.z + sine.x * cosine.y * cosine.z),
        scale.z * cosine.x * cosine.y,
        translation.z,

        0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat4 TRS_matrix(const vec3& translation, const quaternion& rotation, const vec3& scale) {
    return mat4(
        scale.x * (1.0f - 2.0f * (rotation.y * rotation.y + rotation.z * rotation.z)),
        scale.x * (2.0f * (rotation.x * rotation.y - rotation.w * rotation.z)),
        scale.x * (2.0f * (rotation.x * rotation.z + rotation.w * rotation.y)),
        translation.x,

        scale.y * (2.0f * (rotation.x * rotation.y + rotation.w * rotation.z)),
        scale.y * (1.0f - 2.0f * (rotation.x * rotation.x + rotation.z * rotation.z)),
        scale.y * (2.0f * (rotation.y * rotation.z - rotation.w * rotation.x)),
        translation.y,

        scale.z * (2.0f * (rotation.x * rotation.z - rotation.w * rotation.y)),
        scale.z * (2.0f * (rotation.y * rotation.z + rotation.w * rotation.x)),
        scale.z * (1.0f - 2.0f * (rotation.x * rotation.x + rotation.y * rotation.y)),
        translation.z,

        0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat4 look_at(const vec3& eye, const vec3& target, const vec3& up) {
    const vec3 FRONT = normalize(eye - target); // vec3 from the target to the camera
    const vec3 RIGHT = normalize(cross(up, FRONT));
    const vec3 UP = cross(FRONT, RIGHT);

    return mat4(
        RIGHT.x, RIGHT.y, RIGHT.z, -dot(eye, RIGHT),
        UP.x, UP.y, UP.z, -dot(eye, UP),
        FRONT.x, FRONT.y, FRONT.z, -dot(eye, FRONT),
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat4 perspective(float fov, float aspect, float near, float far) {
    return mat4(
        1.0f / (aspect * std::tan(0.5f * fov)), 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f / std::tan(0.5f * fov), 0.0f, 0.0f,
        0.0f, 0.0f, -(far + near) / (far - near), -(2.0f * far * near) / (far - near),
        0.0f, 0.0f, -1.0f, 0.0f
    );
}
