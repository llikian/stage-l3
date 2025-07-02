/***************************************************************************************************
 * @file  LineMesh.hpp
 * @brief Declaration of the LineMesh class
 **************************************************************************************************/

#pragma once

#include <vector>
#include "maths/vec3.hpp"
#include "mesh/Mesh.hpp"

/**
 * @class LineMesh
 * @brief Represents a 3D mesh made out of lines that can be created and rendererd.
 */
class LineMesh : protected Mesh {
public:
    /**
     * @struct LineMesh::Vertex
     * @brief A structure holding a vertex's attributes.
     */
    struct Vertex {
        vec3 position;
        vec3 color;
    };

    /**
     * @brief Creates an empty mesh without initializing buffers.
     */
    LineMesh();

    /**
     * @brief Creates a mesh with the specified vertices and indices and initializes the buffers.
     */
    LineMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    /**
     * @brief Creates a mesh with the specified vertices and no indices and initializes the buffers.
     */
    explicit LineMesh(const std::vector<Vertex>& vertices);

    /**
     * @brief Frees buffers.
     */
    ~LineMesh() override;

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
     * @return The mesh's primitive: GL_LINES.
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
     * @return The amount of indices in the mesh.
     */
    size_t get_indices_amount() const;

    /**
     * @brief Delete OpenGL buffers and clears the vertices array and the indices array.
     */
    void clear() override;

    /**
     * @brief Delete OpenGL buffers.
     */
    void delete_buffers() override;

    /**
     * @brief Applies a model matrix to each vertex in the mesh.
     * @param model The model matrix to apply.
     */
    void apply_model_matrix(const mat4& model) override;

    /**
     * @brief Add a vertex to the mesh.
     * @param vertex The vertex.
     */
    void add_vertex(const Vertex& vertex);

    /**
     * @brief Create and add a vertex to the mesh.
     * @param position
     * @param color
     */
    void add_vertex(const vec3& position, const vec3& color = vec3(1.0f));

    /**
     * @brief Add a line's indices to the mesh.
     * @param start The index of the start of the line.
     * @param end The index of the end of the line.
     */
    void add_line(unsigned int start, unsigned int end);

private:
    unsigned int EBO; ///< The mesh's Element Buffer Object. Useful to avoid repeating vertices in memory.

    std::vector<Vertex> vertices;      ///< The mesh's vertices.
    std::vector<unsigned int> indices; ///< The mesh's indices.
};
