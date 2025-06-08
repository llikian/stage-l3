/***************************************************************************************************
 * @file  Mesh.hpp
 * @brief Declaration of the Mesh class
 **************************************************************************************************/

#pragma once

#include <sys/types.h>

/**
 * @class Mesh
 * @brief Mother class for representing a 3D mesh that can be created and rendererd.
 */
class Mesh {
public:
    Mesh();
    virtual ~Mesh();

    virtual void draw() = 0;
    virtual uint getPrimitive() = 0;

protected:
    virtual void bindBuffers() = 0;

    bool bound;

    uint VAO;
    uint VBO;
};
