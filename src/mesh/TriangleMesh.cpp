/***************************************************************************************************
 * @file  TriangleMesh.cpp
 * @brief Implementation of the TriangleMesh class
 **************************************************************************************************/

#include "mesh/TriangleMesh.hpp"

#include <glad/glad.h>

TriangleMesh::TriangleMesh() : EBO(0) { }

TriangleMesh::TriangleMesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices)
    : EBO(0), vertices(vertices), indices(indices) {
    TriangleMesh::bindBuffers();
}

TriangleMesh::TriangleMesh(const std::vector<Vertex>& vertices)
    : EBO(0), vertices(vertices) {
    TriangleMesh::bindBuffers();
}

TriangleMesh::~TriangleMesh() {
    if(!indices.empty()) { glDeleteBuffers(1, &EBO); }
}

void TriangleMesh::draw() {
    if(!bound) { bindBuffers(); }

    glBindVertexArray(VAO);

    if(indices.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    } else {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

uint TriangleMesh::getPrimitive() {
    return GL_TRIANGLES;
}

void TriangleMesh::addVertex(const Vertex& vertex) {
    vertices.push_back(vertex);
}

void TriangleMesh::addVertex(const vec3& position, const vec3& normal, const vec2& texCoords) {
    vertices.emplace_back(position, normal, texCoords);
}

void TriangleMesh::addIndex(uint index) {
    indices.push_back(index);
}

void TriangleMesh::addTriangle(uint top, uint left, uint right) {
    indices.push_back(top);
    indices.push_back(left);
    indices.push_back(right);
}

void TriangleMesh::addFace(uint topL, uint bottomL, uint bottomR, uint topR) {
    indices.push_back(topL);
    indices.push_back(bottomL);
    indices.push_back(bottomR);

    indices.push_back(topL);
    indices.push_back(bottomR);
    indices.push_back(topR);
}

void TriangleMesh::bindBuffers() {
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

    /* Normals */
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec3)));
    glEnableVertexAttribArray(1);

    /* Texture Coordinates */
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(vec3)));
    glEnableVertexAttribArray(2);

    /* Indices & EBO */
    if(!indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
    }

    bound = true;
}
