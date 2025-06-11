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
#include "Material.hpp"

/**
 * @class Model
 * @brief
 */
class Model {
public:
    explicit Model(const std::filesystem::path& obj_file_path, bool verbose = false);

    void parse_obj_file(std::ifstream& file, bool verbose);

    void draw(Shader& shader);
private:
    std::vector<TriangleMesh> meshes;
    std::vector<Material> materials;
};