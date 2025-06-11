/***************************************************************************************************
 * @file  Mesh.hpp
 * @brief Declaration of the Mesh class
 **************************************************************************************************/

#pragma once

#include "Shader.hpp"

/**
 * @class Mesh
 * @brief Mother class for representing a 3D mesh that can be created and rendererd.
 */
class Mesh {
public:
    Mesh();
    virtual ~Mesh();

    virtual void draw(Shader& shader) = 0;
    virtual unsigned int getPrimitive() = 0;

protected:
    virtual void bindBuffers() = 0;

    bool bound;

    unsigned int VAO;
    unsigned int VBO;
};
