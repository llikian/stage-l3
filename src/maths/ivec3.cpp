/***************************************************************************************************
 * @file  ivec3.cpp
 * @brief Implementation of the ivec3 class
 **************************************************************************************************/

#include "maths/ivec3.hpp"

ivec3::ivec3(): x(0), y(0), z(0) { }

ivec3::ivec3(int value): x(value), y(value), z(value) { }

ivec3::ivec3(int x, int y, int z): x(x), y(y), z(z) { }

bool ivec3::operator==(const ivec3& other) const {
    return x == other.x && y == other.y && z == other.z;
}
