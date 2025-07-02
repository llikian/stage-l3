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
    virtual void draw(const Shader& shader) = 0;

    /**
     * @brief Binds the OpenGL buffers.
     */
    virtual void bind_buffers() = 0;

    /**
     * @return The mesh's primitive.
     */
    virtual unsigned int get_primitive() const = 0;

    /**
     * @return Get the amount of vertices in the mesh.
     */
    virtual size_t get_vertices_amount() const = 0;

    /**
     * @brief Calculates the minimum and maximum value for each coordinate for every point in the
     * mesh.
     * @param minimum The lowest x, y and z values of the points in the mesh.
     * @param maximum The highest x, y and z values of the points in the mesh.
     */
    virtual void get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const = 0;

    /**
     * @brief Delete OpenGL buffers and clears the vertices array (and if there is one, the indices
     * array).
     */
    virtual void clear() = 0;

    /**
     * @brief Delete OpenGL buffers.
     */
    virtual void delete_buffers();

protected:
    unsigned int VAO; ///< The mesh's Vertex Array Object.
    unsigned int VBO; ///< The mesh's Vertex Buffer Object.
};
