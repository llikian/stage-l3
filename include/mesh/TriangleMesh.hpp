/***************************************************************************************************
* @file  TriangleMesh.hpp
 * @brief Declaration of the TriangleMesh class
 **************************************************************************************************/

#pragma once

#include <vector>
#include "Material.hpp"
#include "maths/vec2.hpp"
#include "maths/vec3.hpp"
#include "mesh/Mesh.hpp"

/**
 * @class TriangleMesh
 * @brief Represents a triangular 3D mesh that can be created and rendererd.
 */
class TriangleMesh : protected Mesh {
public:
    friend class TriangleMeshEntity;

    /**
     * @struct TriangleMesh::Vertex
     * @brief A structure holding a vertex's attributes.
     */
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 texCoords;
    };

    /**
     * @brief Creates an empty mesh without initializing buffers.
     */
    TriangleMesh();

    /**
     * @brief Creates a mesh with the specified vertices and indices and initializes the buffers.
     */
    TriangleMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    /**
     * @brief Creates a mesh with the specified vertices and no indices and initializes the buffers.
     */
    explicit TriangleMesh(const std::vector<Vertex>& vertices);

    /**
     * @brief Frees buffers.
     */
    ~TriangleMesh() override;

    /**
     * @brief Performs a draw call with a certain shader.
     * @param shader The shader to perform the draw call with.
     */
    void draw(const Shader& shader) override;

    /**
     * @return The mesh's primitive: GL_TRIANGLES.
     */
    unsigned int get_primitive() const override;

    /**
     * @brief Add a vertex to the mesh.
     * @param vertex The vertex.
     */
    void add_vertex(const Vertex& vertex);

    /**
     * @brief Create and add a vertex to the mesh.
     * @param position
     * @param normal
     * @param texCoords
     */
    void add_vertex(const vec3& position, const vec3& normal, const vec2& texCoords);

    /**
     * @brief Add a triangle's indices to the mesh.
     * @param top The index of the top vertex.
     * @param left The index of the left vertex.
     * @param right The index of the right vertex.
     */
    void add_triangle(unsigned int top, unsigned int left, unsigned int right);

    /**
     * @breif Add a rectangle's indices to the mesh
     * @param topL The index of the top left vertex.
     * @param bottomL The index of the bottom left vertex.
     * @param bottomR The index of the bottom right vertex.
     * @param topR The index of the top right vertex.
     */
    void add_face(unsigned int topL, unsigned int bottomL, unsigned int bottomR, unsigned int topR);

    /**
     * @brief Set the mesh's material.
     * @param material The material to use.
     */
    void set_material(Material* material);

    /**
     * @return The amount of vertices in the mesh.
     */
    size_t get_vertices_amount() const override;

    void get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const override;

    /**
     * @return The amount of indices in the mesh.
     */
    size_t get_indices_amount() const;

    /**
     * @brief Applies a model matrix to each vertex in the mesh.
     * @param model The model matrix to apply.
     */
    void apply_model_matrix(const mat4& model);

private:
    /**
     * @brief Binds the OpenGL buffers.
     */
    void bind_buffers() override;

    unsigned int EBO; ///< The mesh's Element Buffer Object. Useful to avoid repeating vertices in memory.

    std::vector<Vertex> vertices;      ///< The mesh's vertices.
    std::vector<unsigned int> indices; ///< The mesh's indices.

    Material* material; ///< The mesh's material.
};
