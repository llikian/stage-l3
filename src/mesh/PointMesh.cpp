/***************************************************************************************************
 * @file  PointMesh.cpp
 * @brief Implementation of the PointMesh class
 **************************************************************************************************/

#include "mesh/PointMesh.hpp"

#include <glad/glad.h>

PointMesh::PointMesh() { }

PointMesh::PointMesh(const std::vector<Vertex>& vertices)
    : vertices(vertices) {
    PointMesh::bindBuffers();
}

void PointMesh::addVertex(const vec3& position, const vec3& color, float size) {
    vertices.emplace_back(position, color, size);
}

void PointMesh::draw() {
    if(!bound) { bindBuffers(); }
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertices.size());
}

uint PointMesh::getPrimitive() {
    return GL_POINTS;
}

void PointMesh::addVertex(const Vertex& vertex) {
    vertices.push_back(vertex);
}

void PointMesh::bindBuffers() {
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
