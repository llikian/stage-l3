/***************************************************************************************************
 * @file  mat4.cpp
 * @brief Implementation of the mat4 struct
 **************************************************************************************************/

#include "maths/mat4.hpp"

#include <cmath>
#include "maths/geometry.hpp"
#include "maths/trigonometry.hpp"

mat4::mat4()
    : values{
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f }
    } { }

mat4::mat4(float v00, float v01, float v02, float v03,
           float v10, float v11, float v12, float v13,
           float v20, float v21, float v22, float v23,
           float v30, float v31, float v32, float v33)
    : values{
        { v00, v10, v20, v30 },
        { v01, v11, v21, v31 },
        { v02, v12, v22, v32 },
        { v03, v13, v23, v33 }
    } { }

mat4::mat4(float v00, float v01, float v02,
           float v10, float v11, float v12,
           float v20, float v21, float v22)
    : values{
        { v00, v10, v20, 0.0f },
        { v01, v11, v21, 0.0f },
        { v02, v12, v22, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    } { }

mat4::mat4(float scalar)
    : values{
        { scalar, 0.0f, 0.0f, 0.0f },
        { 0.0f, scalar, 0.0f, 0.0f },
        { 0.0f, 0.0f, scalar, 0.0f },
        { 0.0f, 0.0f, 0.0f, scalar }
    } { }

mat4& mat4::scale(float factor) {
    for(int i = 0 ; i < 3 ; ++i) {
        values[0][i] *= factor;
        values[1][i] *= factor;
        values[2][i] *= factor;
    }

    return *this;
}

mat4& mat4::scale(float x, float y, float z) {
    for(int i = 0 ; i < 3 ; ++i) {
        values[0][i] *= x;
        values[1][i] *= y;
        values[2][i] *= z;
    }

    return *this;
}

mat4& mat4::scale(const vec3& factors) {
    for(int i = 0 ; i < 3 ; ++i) {
        values[0][i] *= factors.x;
        values[1][i] *= factors.y;
        values[2][i] *= factors.z;
    }

    return *this;
}

mat4& mat4::scale_x(float factor) {
    for(int i = 0 ; i < 3 ; ++i) {
        values[0][i] *= factor;
    }

    return *this;
}

mat4& mat4::scale_y(float factor) {
    for(int i = 0 ; i < 3 ; ++i) {
        values[1][i] *= factor;
    }

    return *this;
}

mat4& mat4::scale_z(float factor) {
    for(int i = 0 ; i < 3 ; ++i) {
        values[2][i] *= factor;
    }

    return *this;
}

mat4& mat4::translate(const vec3& vector) {
    values[3][0] += values[0][0] * vector.x + values[1][0] * vector.y + values[2][0] * vector.z;
    values[3][1] += values[0][1] * vector.x + values[1][1] * vector.y + values[2][1] * vector.z;
    values[3][2] += values[0][2] * vector.x + values[1][2] * vector.y + values[2][2] * vector.z;
    values[3][3] += values[0][3] * vector.x + values[1][3] * vector.y + values[2][3] * vector.z;

    return *this;
}

mat4& mat4::translate(float x, float y, float z) {
    values[3][0] += values[0][0] * x + values[1][0] * y + values[2][0] * z;
    values[3][1] += values[0][1] * x + values[1][1] * y + values[2][1] * z;
    values[3][2] += values[0][2] * x + values[1][2] * y + values[2][2] * z;
    values[3][3] += values[0][3] * x + values[1][3] * y + values[2][3] * z;

    return *this;
}

mat4& mat4::translate_x(float scalar) {
    values[3][0] += values[0][0] * scalar;
    values[3][1] += values[0][1] * scalar;
    values[3][2] += values[0][2] * scalar;
    values[3][3] += values[0][3] * scalar;

    return *this;
}

mat4& mat4::translate_y(float scalar) {
    values[3][0] += values[1][0] * scalar;
    values[3][1] += values[1][1] * scalar;
    values[3][2] += values[1][2] * scalar;
    values[3][3] += values[1][3] * scalar;

    return *this;
}

mat4& mat4::translate_z(float scalar) {
    values[3][0] += values[2][0] * scalar;
    values[3][1] += values[2][1] * scalar;
    values[3][2] += values[2][2] * scalar;
    values[3][3] += values[2][3] * scalar;

    return *this;
}

mat4& mat4::rotate_x(float angle) {
    angle = degrees_to_radians(angle);

    const float cosine = std::cos(angle);
    const float sine = std::sin(angle);

    float column[4]{ values[1][0], values[1][1], values[1][2], values[1][3] };

    values[1][0] = cosine * column[0] + sine * values[2][0];
    values[1][1] = cosine * column[1] + sine * values[2][1];
    values[1][2] = cosine * column[2] + sine * values[2][2];
    values[1][3] = cosine * column[3] + sine * values[2][3];

    values[2][0] = -sine * column[0] + cosine * values[2][0];
    values[2][1] = -sine * column[1] + cosine * values[2][1];
    values[2][2] = -sine * column[2] + cosine * values[2][2];
    values[2][3] = -sine * column[3] + cosine * values[2][3];

    return *this;
}

mat4& mat4::rotate_y(float angle) {
    angle = degrees_to_radians(angle);

    const float cosine = std::cos(angle);
    const float sine = std::sin(angle);

    float column[4]{ values[0][0], values[0][1], values[0][2], values[0][3] };

    values[0][0] = cosine * column[0] - sine * values[2][0];
    values[0][1] = cosine * column[1] - sine * values[2][1];
    values[0][2] = cosine * column[2] - sine * values[2][2];
    values[0][3] = cosine * column[3] - sine * values[2][3];

    values[2][0] = sine * column[0] + cosine * values[2][0];
    values[2][1] = sine * column[1] + cosine * values[2][1];
    values[2][2] = sine * column[2] + cosine * values[2][2];
    values[2][3] = sine * column[3] + cosine * values[2][3];

    return *this;
}

mat4& mat4::rotate_z(float angle) {
    angle = degrees_to_radians(angle);

    const float cosine = std::cos(angle);
    const float sine = std::sin(angle);

    float column[4]{ values[0][0], values[0][1], values[0][2], values[0][3] };

    values[0][0] = cosine * column[0] + sine * values[1][0];
    values[0][1] = cosine * column[1] + sine * values[1][1];
    values[0][2] = cosine * column[2] + sine * values[1][2];
    values[0][3] = cosine * column[3] + sine * values[1][3];

    values[1][0] = -sine * column[0] + cosine * values[1][0];
    values[1][1] = -sine * column[1] + cosine * values[1][1];
    values[1][2] = -sine * column[2] + cosine * values[1][2];
    values[1][3] = -sine * column[3] + cosine * values[1][3];

    return *this;
}

std::ostream& operator <<(std::ostream& stream, const mat4& mat) {
    for(int i = 0 ; i < 4 ; ++i) {
        stream << "( ";

        for(int j = 0 ; j < 3 ; ++j) {
            stream << ' ' << mat(i, j) << " ; ";
        }

        stream << mat(i, 3) << " )\n";
    }
    return stream;
}

void operator +=(mat4& left, const mat4& right) {
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            left(i, j) += right(i, j);
        }
    }
}

void operator -=(mat4& left, const mat4& right) {
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            left(i, j) -= right(i, j);
        }
    }
}

void operator +=(mat4& mat, float scalar) {
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            mat(i, j) += scalar;
        }
    }
}

void operator -=(mat4& mat, float scalar) {
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            mat(i, j) -= scalar;
        }
    }
}

void operator *=(mat4& mat, float scalar) {
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            mat(i, j) *= scalar;
        }
    }
}

void operator /=(mat4& mat, float scalar) {
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            mat(i, j) /= scalar;
        }
    }
}

mat4 operator +(const mat4& left, const mat4& right) {
    mat4 result;

    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            result(i, j) = left(i, j) + right(i, j);
        }
    }

    return result;
}

mat4 operator -(const mat4& left, const mat4& right) {
    mat4 result;

    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            result(i, j) = left(i, j) - right(i, j);
        }
    }

    return result;
}

mat4 operator *(const mat4& left, const mat4& right) {
    // mat4 result;
    //
    // for(int i = 0 ; i < 4 ; ++i) {
    //     for(int j = 0 ; j < 4 ; ++j) {
    //         for(int k = 0 ; k < 4 ; ++k) {
    //             result(i, j) += left(i, k) * right(k, j);
    //         }
    //     }
    // }
    //
    // return result;

    return mat4(
        left(0, 0) * right(0, 0) + left(0, 1) * right(1, 0) + left(0, 2) * right(2, 0) + left(0, 3) * right(3, 0),
        left(0, 0) * right(0, 1) + left(0, 1) * right(1, 1) + left(0, 2) * right(2, 1) + left(0, 3) * right(3, 1),
        left(0, 0) * right(0, 2) + left(0, 1) * right(1, 2) + left(0, 2) * right(2, 2) + left(0, 3) * right(3, 2),
        left(0, 0) * right(0, 3) + left(0, 1) * right(1, 3) + left(0, 2) * right(2, 3) + left(0, 3) * right(3, 3),
        left(1, 0) * right(0, 0) + left(1, 1) * right(1, 0) + left(1, 2) * right(2, 0) + left(1, 3) * right(3, 0),
        left(1, 0) * right(0, 1) + left(1, 1) * right(1, 1) + left(1, 2) * right(2, 1) + left(1, 3) * right(3, 1),
        left(1, 0) * right(0, 2) + left(1, 1) * right(1, 2) + left(1, 2) * right(2, 2) + left(1, 3) * right(3, 2),
        left(1, 0) * right(0, 3) + left(1, 1) * right(1, 3) + left(1, 2) * right(2, 3) + left(1, 3) * right(3, 3),
        left(2, 0) * right(0, 0) + left(2, 1) * right(1, 0) + left(2, 2) * right(2, 0) + left(2, 3) * right(3, 0),
        left(2, 0) * right(0, 1) + left(2, 1) * right(1, 1) + left(2, 2) * right(2, 1) + left(2, 3) * right(3, 1),
        left(2, 0) * right(0, 2) + left(2, 1) * right(1, 2) + left(2, 2) * right(2, 2) + left(2, 3) * right(3, 2),
        left(2, 0) * right(0, 3) + left(2, 1) * right(1, 3) + left(2, 2) * right(2, 3) + left(2, 3) * right(3, 3),
        left(3, 0) * right(0, 0) + left(3, 1) * right(1, 0) + left(3, 2) * right(2, 0) + left(3, 3) * right(3, 0),
        left(3, 0) * right(0, 1) + left(3, 1) * right(1, 1) + left(3, 2) * right(2, 1) + left(3, 3) * right(3, 1),
        left(3, 0) * right(0, 2) + left(3, 1) * right(1, 2) + left(3, 2) * right(2, 2) + left(3, 3) * right(3, 2),
        left(3, 0) * right(0, 3) + left(3, 1) * right(1, 3) + left(3, 2) * right(2, 3) + left(3, 3) * right(3, 3)
    );
}

mat4 operator +(const mat4& mat, float scalar) {
    mat4 result;

    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            result(i, j) = mat(i, j) + scalar;
        }
    }

    return result;
}

mat4 operator -(const mat4& mat, float scalar) {
    mat4 result;

    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            result(i, j) = mat(i, j) - scalar;
        }
    }

    return result;
}

mat4 operator *(const mat4& mat, float scalar) {
    mat4 result;

    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            result(i, j) = mat(i, j) * scalar;
        }
    }

    return result;
}

mat4 operator *(float scalar, const mat4& mat) {
    mat4 result;

    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            result(i, j) = scalar * mat(i, j);
        }
    }

    return result;
}

mat4 operator /(const mat4& mat, float scalar) {
    mat4 result;

    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            result(i, j) = mat(i, j) / scalar;
        }
    }

    return result;
}

vec3 operator*(const mat4& mat, const vec3& vec) {
    return vec3(
        mat(0, 0) * vec.x + mat(0, 1) * vec.y + mat(0, 2) * vec.z,
        mat(1, 0) * vec.x + mat(1, 1) * vec.y + mat(1, 2) * vec.z,
        mat(2, 0) * vec.x + mat(2, 1) * vec.y + mat(2, 2) * vec.z
    );
}

vec4 operator*(const mat4& mat, const vec4& vec) {
    return vec4(
        mat(0, 0) * vec.x + mat(0, 1) * vec.y + mat(0, 2) * vec.z + mat(0, 3) * vec.w,
        mat(1, 0) * vec.x + mat(1, 1) * vec.y + mat(1, 2) * vec.z + mat(1, 3) * vec.w,
        mat(2, 0) * vec.x + mat(2, 1) * vec.y + mat(2, 2) * vec.z + mat(2, 3) * vec.w,
        mat(3, 0) * vec.x + mat(3, 1) * vec.y + mat(3, 2) * vec.z + mat(3, 3) * vec.w
    );
}
