/***************************************************************************************************
 * @file  PointMesh.cpp
 * @brief Implementation of the PointMesh class
 **************************************************************************************************/

#include "mesh/PointMesh.hpp"

#include <glad/glad.h>

PointMesh::PointMesh() { }

PointMesh::PointMesh(const std::vector<Vertex>& vertices)
    : vertices(vertices) {
    PointMesh::bind_buffers();
}

void PointMesh::draw(const Shader& shader) {
    if(!bound) { bind_buffers(); }

    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertices.size());
}

unsigned int PointMesh::get_primitive() const {
    return GL_POINTS;
}

void PointMesh::add_vertex(const Vertex& vertex) {
    vertices.push_back(vertex);
}

void PointMesh::add_vertex(const vec3& position, const vec3& color, float size) {
    vertices.emplace_back(position, color, size);
}

size_t PointMesh::get_vertices_amount() const {
    return vertices.size();
}

void PointMesh::get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const {
    for(unsigned int i = 0 ; i < vertices.size() ; ++i) {
        const vec3& pos = vertices[i].position;
        minimum.x = std::min(minimum.x, pos.x);
        minimum.y = std::min(minimum.y, pos.y);
        minimum.z = std::min(minimum.z, pos.z);

        maximum.x = std::max(maximum.x, pos.x);
        maximum.y = std::max(maximum.y, pos.y);
        maximum.z = std::max(maximum.z, pos.z);
    }
}

void PointMesh::bind_buffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    /* Positions */
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    /* Colors */
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec3)));
    glEnableVertexAttribArray(1);

    /* Size */
    glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(vec3)));
    glEnableVertexAttribArray(2);

    bound = true;
}
