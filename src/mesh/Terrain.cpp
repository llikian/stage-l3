/***************************************************************************************************
 * @file  Terrain.cpp
 * @brief Implementation of the Terrain class
 **************************************************************************************************/

#include "mesh/Terrain.hpp"

#include <glad/glad.h>

Terrain::Terrain(const Shader& shader, float chunk_size, unsigned int chunks_on_line)
    : shader(shader), chunk_size(chunk_size), chunks_on_line(chunks_on_line) {
    float corner_coord = chunk_size * chunks_on_line / 2.0f;
    for(unsigned int i = 0 ; i <= chunks_on_line ; ++i) {
        for(unsigned int j = 0 ; j <= chunks_on_line ; ++j) {
            positions.emplace_back(-corner_coord + i * chunk_size, -corner_coord + j * chunk_size);
        }
    }

    for(unsigned int i = 0 ; i < chunks_on_line ; ++i) {
        for(unsigned int j = 0 ; j < chunks_on_line ; ++j) {
            indices.push_back(get_index(i, j));
            indices.push_back(get_index(i, j + 1));
            indices.push_back(get_index(i + 1, j + 1));
            indices.push_back(get_index(i + 1, j));
        }
    }

    /* VAO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* VBO */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec2), positions.data(), GL_STATIC_DRAW);

    /* Positions */
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(vec2), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    /* Indices & EBO */
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
}

Terrain::~Terrain() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Terrain::draw(const mat4& view_projection) const {
    shader.use();
    shader.set_uniform("u_view_projection", view_projection);
    shader.set_uniform("u_chunk_size", chunk_size);
    glBindVertexArray(VAO);
    glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Terrain::draw(const Frustum& frustum, const mat4& view_projection) const {
    shader.use();

    shader.set_uniform("u_view_projection", view_projection);
    shader.set_uniform("u_chunk_size", chunk_size);
    shader.set_uniform("u_frustum_view_projection_matrix", frustum.view_projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

unsigned int Terrain::get_index(unsigned int x, unsigned int y) const {
    return x + y * (chunks_on_line + 1);
}
