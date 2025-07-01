/***************************************************************************************************
 * @file  Terrain.hpp
 * @brief Declaration of the Terrain class
 **************************************************************************************************/

#pragma once

#include "culling/Frustum.hpp"
#include "TriangleMesh.hpp"

/**
 * @class Terrain
 * @brief
 */
class Terrain {
public:
    Terrain(const Shader& shader, float chunk_size, unsigned int chunks_on_line);
    ~Terrain();

    void draw(const mat4& view_projection) const;
    void draw(const Frustum& frustum, const mat4& view_projection) const;

private:
    unsigned int get_index(unsigned int x, unsigned int y) const;

    const Shader& shader;

    std::vector<vec2> positions;
    std::vector<unsigned int> indices;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    float chunk_size;
    unsigned int chunks_on_line;
};
