/***************************************************************************************************
* @file  TriangleMesh.hpp
 * @brief Declaration of the TriangleMesh class
 **************************************************************************************************/

#pragma once

#include <vector>
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
    TriangleMesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices);
    explicit TriangleMesh(const std::vector<Vertex>& vertices);
    ~TriangleMesh() override;

    void draw() override;

    uint getPrimitive() override;

    void addVertex(const Vertex& vertex);
    void addVertex(const vec3& position, const vec3& normal, const vec2& texCoords);

    void addIndex(uint index);
    void addTriangle(uint top, uint left, uint right);
    void addFace(uint topL, uint bottomL, uint bottomR, uint topR);

private:
    void bindBuffers() override;

    uint EBO;

    std::vector<Vertex> vertices;
    std::vector<uint> indices;
};
