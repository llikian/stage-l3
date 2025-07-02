/***************************************************************************************************
 * @file  Mesh.cpp
 * @brief Implementation of the Mesh class
 **************************************************************************************************/

#include "mesh/Mesh.hpp"

#include <glad/glad.h>

Mesh::Mesh() : VAO(0), VBO(0) { }

Mesh::~Mesh() {
    Mesh::delete_buffers();
}

void Mesh::delete_buffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
