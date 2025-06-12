/***************************************************************************************************
 * @file  Model.hpp
 * @brief Declaration of the Model class
 **************************************************************************************************/

#pragma once

#include <filesystem>
#include <fstream>
#include <vector>
#include "Material.hpp"
#include "Shader.hpp"
#include "TriangleMesh.hpp"
#include "maths/ivec3.hpp"

/**
 * @class Model
 * @brief
 */
class Model {
public:
    explicit Model(const std::filesystem::path& path, const mat4& model = mat4(1.0f), bool verbose = false);

    void parse_obj_file(const std::filesystem::path& path, bool verbose);
    void parse_mtl_file(const std::filesystem::path& path);

    void handle_object(std::vector<vec3>& positions,
                       std::vector<vec3>& normals,
                       std::vector<vec2>& tex_coords,
                       std::vector<ivec3>& vertex_indices);

    void draw(Shader& shader);

    mat4 model;

private:
    std::vector<TriangleMesh> meshes;
    std::unordered_map<std::string, Material> materials;
};
