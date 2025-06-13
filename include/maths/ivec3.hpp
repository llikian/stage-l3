/***************************************************************************************************
 * @file  ivec3.hpp
 * @brief Declaration of the ivec3 class
 **************************************************************************************************/

#pragma once

/**
 * @class ivec3
 * @brief
 */
struct ivec3 {
    ivec3();
    explicit ivec3(int value);
    ivec3(int x, int y, int z);

    bool operator==(const ivec3& other) const;

    int x;
    int y;
    int z;
};
