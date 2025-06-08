/***************************************************************************************************
 * @file  LineMesh.cpp
 * @brief Implementation of the LineMesh class
 **************************************************************************************************/

#include "mesh/LineMesh.hpp"

#include <glad/glad.h>

LineMesh::LineMesh() : EBO(0) { }

LineMesh::LineMesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices)
    : EBO(0), vertices(vertices), indices(indices) {
    LineMesh::bindBuffers();
}

LineMesh::LineMesh(const std::vector<Vertex>& vertices)
    : EBO(0), vertices(vertices) {
    LineMesh::bindBuffers();
}

LineMesh::~LineMesh() {
    if(!indices.empty()) { glDeleteBuffers(1, &EBO); }
}

void LineMesh::draw() {
    if(!bound) { bindBuffers(); }

    glBindVertexArray(VAO);

    if(indices.empty()) {
        glDrawArrays(GL_LINES, 0, vertices.size());
    } else {
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

uint LineMesh::getPrimitive() {
    return GL_LINES;
}

void LineMesh::addVertex(const Vertex& vertex) {
    vertices.push_back(vertex);
}

void LineMesh::addVertex(const vec3& position, const vec3& color) {
    vertices.emplace_back(position, color);
}

void LineMesh::addLine(uint start, uint end) {
    indices.push_back(start);
    indices.push_back(end);
}

void LineMesh::bindBuffers() {
    /* VAO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* VBO */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    /* Positions */
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    /* Colors */
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec3)));
    glEnableVertexAttribArray(1);

    /* Thickness */
    glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(vec3)));
    glEnableVertexAttribArray(2);

    /* Indices & EBO */
    if(!indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
    }

    bound = true;
}
