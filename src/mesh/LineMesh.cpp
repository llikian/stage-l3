/***************************************************************************************************
 * @file  LineMesh.cpp
 * @brief Implementation of the LineMesh class
 **************************************************************************************************/

#include "mesh/LineMesh.hpp"

#include <glad/glad.h>

LineMesh::LineMesh() : EBO(0) { }

LineMesh::LineMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : EBO(0), vertices(vertices), indices(indices) {
    LineMesh::bind_buffers();
}

LineMesh::LineMesh(const std::vector<Vertex>& vertices)
    : EBO(0), vertices(vertices) {
    LineMesh::bind_buffers();
}

LineMesh::~LineMesh() {
    if(!indices.empty()) { glDeleteBuffers(1, &EBO); }
}

void LineMesh::draw(const Shader& shader) {
    shader.use();
    glBindVertexArray(VAO);

    if(indices.empty()) {
        glDrawArrays(GL_LINES, 0, vertices.size());
    } else {
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

void LineMesh::bind_buffers() {
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

    /* Indices & EBO */
    if(!indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
    }
}

unsigned int LineMesh::get_primitive() const {
    return GL_LINES;
}

size_t LineMesh::get_vertices_amount() const {
    return vertices.size();
}

void LineMesh::get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const {
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

size_t LineMesh::get_indices_amount() const {
    return indices.size();
}

void LineMesh::clear() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if(!indices.empty()) { glDeleteBuffers(1, &EBO); }
    vertices.clear();
    indices.clear();
}

void LineMesh::delete_buffers() {
    Mesh::delete_buffers();
    if(!indices.empty()) { glDeleteBuffers(1, &EBO); }
}

void LineMesh::add_vertex(const Vertex& vertex) {
    vertices.push_back(vertex);
}

void LineMesh::add_vertex(const vec3& position, const vec3& color) {
    vertices.emplace_back(position, color);
}

void LineMesh::add_line(unsigned int start, unsigned int end) {
    indices.push_back(start);
    indices.push_back(end);
}
