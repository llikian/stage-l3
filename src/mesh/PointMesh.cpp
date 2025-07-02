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
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertices.size());
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
}

unsigned int PointMesh::get_primitive() const {
    return GL_POINTS;
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

void PointMesh::clear() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    vertices.clear();
}

void PointMesh::apply_model_matrix(const mat4& model) {
    for(Vertex& vertex : vertices) {
        vec3 vec = vertex.position;
        vertex.position.x = model(0, 0) * vec.x + model(0, 1) * vec.y + model(0, 2) * vec.z + model(0, 3);
        vertex.position.y = model(1, 0) * vec.x + model(1, 1) * vec.y + model(1, 2) * vec.z + model(1, 3);
        vertex.position.z = model(2, 0) * vec.x + model(2, 1) * vec.y + model(2, 2) * vec.z + model(2, 3);
    }

    bind_buffers();
}

void PointMesh::add_vertex(const Vertex& vertex) {
    vertices.push_back(vertex);
}

void PointMesh::add_vertex(const vec3& position, const vec3& color, float size) {
    vertices.emplace_back(position, color, size);
}
