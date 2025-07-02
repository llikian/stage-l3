/***************************************************************************************************
 * @file  PointMesh.hpp
 * @brief Declaration of the PointMesh class
 **************************************************************************************************/

#pragma once

#include <vector>
#include "maths/vec3.hpp"
#include "mesh/Mesh.hpp"

/**
 * @class PointMesh
 * @brief Represents a 3D mesh made out of points that can be created and rendererd.
 */
class PointMesh : protected Mesh {
public:
    /**
     * @struct PointMesh::Vertex
     * @brief A structure holding a vertex's attributes.
     */
    struct Vertex {
        vec3 position;
        vec3 color;
        float size;
    };

    /**
     * @brief Creates an empty mesh without initializing buffers.
     */
    PointMesh();

    /**
     * @brief Creates a mesh with the specified vertices and initializes the buffers.
     */
    explicit PointMesh(const std::vector<Vertex>& vertices);

    /**
     * @brief Performs a draw call with a certain shader.
     * @param shader The shader to perform the draw call with.
     */
    void draw(const Shader& shader) override;

    /**
     * @brief Binds the OpenGL buffers.
     */
    void bind_buffers() override;

    /**
     * @return The mesh's primitive: GL_POINTS.
     */
    unsigned int get_primitive() const override;

    /**
     * @return The amount of vertices in the mesh.
     */
    size_t get_vertices_amount() const override;

    /**
     * @brief Calculates the minimum and maximum value for each coordinate for every point in the
     * mesh.
     * @param minimum The lowest x, y and z values of the points in the mesh.
     * @param maximum The highest x, y and z values of the points in the mesh.
     */
    void get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const override;

    /**
     * @brief Delete OpenGL buffers and clears the vertices array.
     */
    void clear() override;

    /**
     * @brief Add a vertex to the mesh.
     * @param vertex The vertex.
     */
    void add_vertex(const Vertex& vertex);

    /**
     * @brief Create and add a vertex to the mesh.
     * @param position The vertex's position.
     * @param color The vertex's color.
     * @param size The vertex's size.
     */
    void add_vertex(const vec3& position, const vec3& color = vec3(1.0f), float size = 1.0f);

private:
    std::vector<Vertex> vertices; ///< The mesh's vertices.
};
