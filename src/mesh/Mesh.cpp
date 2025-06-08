/***************************************************************************************************
 * @file  Mesh.cpp
 * @brief Implementation of the Mesh class
 **************************************************************************************************/

#include "mesh/Mesh.hpp"

#include <glad/glad.h>

Mesh::Mesh() : bound(false), VAO(0), VBO(0) { }

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
