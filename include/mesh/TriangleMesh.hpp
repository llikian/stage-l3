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
class TriangleMesh : public Mesh {
public:
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 texCoords;
    };

    TriangleMesh();
    TriangleMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    explicit TriangleMesh(const std::vector<Vertex>& vertices);
    ~TriangleMesh() override;

    void draw(Shader& shader) override;

    unsigned int getPrimitive() override;

    void addVertex(const Vertex& vertex);
    void addVertex(const vec3& position, const vec3& normal, const vec2& texCoords);

    void addIndex(unsigned int index);
    void addTriangle(unsigned int top, unsigned int left, unsigned int right);
    void addFace(unsigned int topL, unsigned int bottomL, unsigned int bottomR, unsigned int topR);

    size_t get_vertices_amount() const;
    size_t get_indices_amount() const;

private:
    void bindBuffers() override;

    unsigned int EBO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Material> materials;
};
