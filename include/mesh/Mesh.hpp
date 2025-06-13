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
    /**
     * @brief Creates an empty mesh without initializing buffers.
     */
    Mesh();

    /**
     * @brief Frees buffers.
     */
    virtual ~Mesh();

    /**
     * @brief Performs a draw call with a certain shader.
     * @param shader The shader to perform the draw call with.
     */
    virtual void draw(Shader& shader) = 0;

    /**
     * @return The mesh's primitive.
     */
    virtual unsigned int get_primitive() const = 0;

    /**
     * @return Get the amount of vertices in the mesh.
     */
    virtual size_t get_vertices_amount() const = 0;

protected:
    /**
     * @brief Binds the OpenGL buffers.
     */
    virtual void bind_buffers() = 0;

    bool bound; ///< Whether the buffers were bound.

    unsigned int VAO; ///< The mesh's Vertex Array Object.
    unsigned int VBO; ///< The mesh's Vertex Buffer Object.
};
