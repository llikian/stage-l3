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
class LineMesh : public Mesh {
public:
    struct Vertex {
        vec3 position;
        vec3 color;
    };

    LineMesh();
    LineMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    explicit LineMesh(const std::vector<Vertex>& vertices);
    ~LineMesh() override;

    void draw(Shader& shader) override;

    unsigned int getPrimitive() override;

    void addVertex(const Vertex& vertex);
    void addVertex(const vec3& position, const vec3& color = vec3(1.0f));

    void addIndex(unsigned int index);
    void addLine(unsigned int start, unsigned int end);

private:
    void bindBuffers() override;

    unsigned int EBO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
