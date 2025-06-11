/***************************************************************************************************
 * @file  Model.hpp
 * @brief Declaration of the Model class
 **************************************************************************************************/

#pragma once

#include <filesystem>
#include <fstream>
#include <vector>
#include "Shader.hpp"
#include "TriangleMesh.hpp"

/**
 * @class Model
 * @brief
 */
class Model {
public:
    explicit Model(const std::filesystem::path& obj_file_path, const mat4& model = mat4(1.0f), bool verbose = false);

    void parse_obj_file(std::ifstream& file, bool verbose);

    void draw(Shader& shader);

    mat4 model;
private:
    std::vector<TriangleMesh> meshes;
};