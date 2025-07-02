/***************************************************************************************************
 * @file  TriangleMesh.cpp
 * @brief Implementation of the TriangleMesh class
 **************************************************************************************************/

#include "mesh/TriangleMesh.hpp"

#include <glad/glad.h>

#include "maths/geometry.hpp"

TriangleMesh::TriangleMesh() : EBO(0), material(nullptr) { }

TriangleMesh::TriangleMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : EBO(0), vertices(vertices), indices(indices) {
    TriangleMesh::bind_buffers();
}

TriangleMesh::TriangleMesh(const std::vector<Vertex>& vertices)
    : EBO(0), vertices(vertices) {
    TriangleMesh::bind_buffers();
}

TriangleMesh::~TriangleMesh() {
    if(!indices.empty()) { glDeleteBuffers(1, &EBO); }
}

void TriangleMesh::draw(const Shader& shader) {
    shader.use();
    if(material != nullptr) { material->update_shader_uniforms(shader); }
    glBindVertexArray(VAO);

    if(indices.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    } else {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

void TriangleMesh::bind_buffers() {
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
}

unsigned int TriangleMesh::get_primitive() const {
    return GL_TRIANGLES;
}

size_t TriangleMesh::get_vertices_amount() const {
    return vertices.size();
}

void TriangleMesh::get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const {
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

void TriangleMesh::clear() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if(!indices.empty()) { glDeleteBuffers(1, &EBO); }
    vertices.clear();
    indices.clear();
}

void TriangleMesh::delete_buffers() {
    Mesh::delete_buffers();
    if(!indices.empty()) { glDeleteBuffers(1, &EBO); }
}

size_t TriangleMesh::get_indices_amount() const {
    return indices.size();
}

void TriangleMesh::add_vertex(const Vertex& vertex) {
    vertices.push_back(vertex);
}

void TriangleMesh::add_vertex(const vec3& position, const vec3& normal, const vec2& texCoords) {
    vertices.emplace_back(position, normal, texCoords);
}

void TriangleMesh::add_triangle(unsigned int top, unsigned int left, unsigned int right) {
    indices.push_back(top);
    indices.push_back(left);
    indices.push_back(right);
}

void TriangleMesh::add_face(unsigned int topL, unsigned int bottomL, unsigned int bottomR, unsigned int topR) {
    indices.push_back(topL);
    indices.push_back(bottomL);
    indices.push_back(bottomR);

    indices.push_back(topL);
    indices.push_back(bottomR);
    indices.push_back(topR);
}

void TriangleMesh::set_material(Material* material) {
    this->material = material;
}

void TriangleMesh::apply_model_matrix(const mat4& model) {
    mat3 normals_model = transpose_inverse(model);

    for(Vertex& vertex : vertices) {
        vec3 vec = vertex.position;
        vertex.position.x = model(0, 0) * vec.x + model(0, 1) * vec.y + model(0, 2) * vec.z + model(0, 3);
        vertex.position.y = model(1, 0) * vec.x + model(1, 1) * vec.y + model(1, 2) * vec.z + model(1, 3);
        vertex.position.z = model(2, 0) * vec.x + model(2, 1) * vec.y + model(2, 2) * vec.z + model(2, 3);

        vertex.normal = normalize(normals_model * vertex.normal);
    }
}
