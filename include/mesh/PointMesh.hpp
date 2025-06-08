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
class PointMesh : public Mesh {
public:
    struct Vertex {
        vec3 position;
        vec3 color;
        float size;
    };

    PointMesh();
    explicit PointMesh(const std::vector<Vertex>& vertices);

    void draw() override;

    uint getPrimitive() override;

    void addVertex(const Vertex& vertex);
    void addVertex(const vec3& position, const vec3& color = vec3(1.0f), float size = 1.0f);

private:
    void bindBuffers() override;

    std::vector<Vertex> vertices;
};
