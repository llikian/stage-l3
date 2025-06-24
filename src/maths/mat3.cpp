/***************************************************************************************************
 * @file  mat3.cpp
 * @brief Implementation of the mat3 struct
 **************************************************************************************************/

#include "maths/mat3.hpp"

#include "maths/mat4.hpp"

mat3::mat3()
    : values{
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f }
    } { }

mat3::mat3(float v00, float v01, float v02,
           float v10, float v11, float v12,
           float v20, float v21, float v22)
    : values{
        { v00, v10, v20 },
        { v01, v11, v21 },
        { v02, v12, v22 }
    } { }

mat3::mat3(float scalar)
    : values{
        { scalar, 0.0f, 0.0f },
        { 0.0f, scalar, 0.0f },
        { 0.0f, 0.0f, scalar }
    } { }

float& mat3::operator ()(int row, int column) {
    return values[column][row];
}

const float& mat3::operator ()(int row, int column) const {
    return values[column][row];
}

mat3 inverse(const mat3& mat) {
    float det = mat(0, 0) * (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1))
                - mat(0, 1) * (mat(1, 0) * mat(2, 2) - mat(1, 2) * mat(2, 0))
                + mat(0, 2) * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0));

    if(det == 0.0f) { return mat; }

    float inv = 1.0f / det;

    return mat3(
        inv * (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1)),
        inv * (mat(0, 2) * mat(2, 1) - mat(0, 1) * mat(2, 2)),
        inv * (mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)),

        inv * (mat(1, 2) * mat(2, 0) - mat(1, 0) * mat(2, 2)),
        inv * (mat(0, 0) * mat(2, 2) - mat(0, 2) * mat(2, 0)),
        inv * (mat(0, 2) * mat(1, 0) - mat(0, 0) * mat(1, 2)),

        inv * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0)),
        inv * (mat(0, 1) * mat(2, 0) - mat(0, 0) * mat(2, 1)),
        inv * (mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0))
    );
}

mat3 transpose_inverse(const mat3& mat) {
    float det = mat(0, 0) * (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1))
                - mat(0, 1) * (mat(1, 0) * mat(2, 2) - mat(1, 2) * mat(2, 0))
                + mat(0, 2) * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0));

    if(det == 0.0f) { return mat; }

    float inv = 1.0f / det;

    return mat3(
        inv * (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1)),
        inv * (mat(1, 2) * mat(2, 0) - mat(1, 0) * mat(2, 2)),
        inv * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0)),

        inv * (mat(0, 2) * mat(2, 1) - mat(0, 1) * mat(2, 2)),
        inv * (mat(0, 0) * mat(2, 2) - mat(0, 2) * mat(2, 0)),
        inv * (mat(0, 1) * mat(2, 0) - mat(0, 0) * mat(2, 1)),

        inv * (mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)),
        inv * (mat(0, 2) * mat(1, 0) - mat(0, 0) * mat(1, 2)),
        inv * (mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0))
    );
}

mat3 transpose_inverse(const mat4& mat) {
    float det = mat(0, 0) * (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1))
                - mat(0, 1) * (mat(1, 0) * mat(2, 2) - mat(1, 2) * mat(2, 0))
                + mat(0, 2) * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0));

    if(det == 0.0f) {
        mat3 res;
        for(int i = 0 ; i < 3 ; ++i) {
            for(int j = 0 ; j < 3 ; ++j) {
                res(i, j) = mat(i, j);
            }
        }
        return res;
    }

    float inv = 1.0f / det;

    return mat3(
        inv * (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1)),
        inv * (mat(1, 2) * mat(2, 0) - mat(1, 0) * mat(2, 2)),
        inv * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0)),

        inv * (mat(0, 2) * mat(2, 1) - mat(0, 1) * mat(2, 2)),
        inv * (mat(0, 0) * mat(2, 2) - mat(0, 2) * mat(2, 0)),
        inv * (mat(0, 1) * mat(2, 0) - mat(0, 0) * mat(2, 1)),

        inv * (mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)),
        inv * (mat(0, 2) * mat(1, 0) - mat(0, 0) * mat(1, 2)),
        inv * (mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0))
    );
}

std::ostream& operator <<(std::ostream& stream, const mat3& mat) {
    for(int i = 0 ; i < 3 ; ++i) {
        stream << "( ";

        for(int j = 0 ; j < 2 ; ++j) {
            stream << ' ' << mat(i, j) << " ; ";
        }

        stream << mat(i, 2) << " )\n";
    }
    return stream;
}

void operator +=(mat3& left, const mat3& right) {
    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            left(i, j) += right(i, j);
        }
    }
}

void operator -=(mat3& left, const mat3& right) {
    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            left(i, j) -= right(i, j);
        }
    }
}

void operator +=(mat3& mat, float scalar) {
    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            mat(i, j) += scalar;
        }
    }
}

void operator -=(mat3& mat, float scalar) {
    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            mat(i, j) -= scalar;
        }
    }
}

void operator *=(mat3& mat, float scalar) {
    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            mat(i, j) *= scalar;
        }
    }
}

void operator /=(mat3& mat, float scalar) {
    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            mat(i, j) /= scalar;
        }
    }
}

mat3 operator +(const mat3& left, const mat3& right) {
    mat3 result;

    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            result(i, j) = left(i, j) + right(i, j);
        }
    }

    return result;
}

mat3 operator -(const mat3& left, const mat3& right) {
    mat3 result;

    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            result(i, j) = left(i, j) - right(i, j);
        }
    }

    return result;
}

mat3 operator *(const mat3& left, const mat3& right) {
    mat3 result;

    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            for(int k = 0 ; k < 3 ; ++k) {
                result(i, j) += left(i, k) * right(k, j);
            }
        }
    }

    return result;
}

mat3 operator +(const mat3& mat, float scalar) {
    mat3 result;

    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            result(i, j) = mat(i, j) + scalar;
        }
    }

    return result;
}

mat3 operator -(const mat3& mat, float scalar) {
    mat3 result;

    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            result(i, j) = mat(i, j) - scalar;
        }
    }

    return result;
}

mat3 operator *(const mat3& mat, float scalar) {
    mat3 result;

    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            result(i, j) = mat(i, j) * scalar;
        }
    }

    return result;
}

mat3 operator *(float scalar, const mat3& mat) {
    mat3 result;

    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            result(i, j) = scalar * mat(i, j);
        }
    }

    return result;
}

mat3 operator /(const mat3& mat, float scalar) {
    mat3 result;

    for(int i = 0 ; i < 3 ; ++i) {
        for(int j = 0 ; j < 3 ; ++j) {
            result(i, j) = mat(i, j) / scalar;
        }
    }

    return result;
}

vec3 operator*(const mat3& mat, const vec3& vec) {
    return vec3(
        mat(0, 0) * vec.x + mat(0, 1) * vec.y + mat(0, 2) * vec.z,
        mat(1, 0) * vec.x + mat(1, 1) * vec.y + mat(1, 2) * vec.z,
        mat(2, 0) * vec.x + mat(2, 1) * vec.y + mat(2, 2) * vec.z
    );
}
