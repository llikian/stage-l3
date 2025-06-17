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
#include "maths/vec3.hpp"

/**
 * @class Model
 * @brief Create 3D models by reading data from various file formats.
 */
class Model {
public:
    /**
     * @brief Creates a mesh by reading the file at a certain path. Currently supports only .obj files.
     * @param path The path to the file.
     * @param model The model's model matrix.
     * @param verbose Whether the funtion will log information about what it's doing.
     */
    explicit Model(const std::filesystem::path& path, const mat4& model = mat4(1.0f), bool verbose = false);

    /**
     * @brief Performs a draw call for each of the model's meshes with a certain shader.
     * @param shader The shader to perform the draw calls with.
     */
    void draw(const Shader& shader);

    mat4 model_matrix; ///< The model's model matrix.

private:
    /**
     * @brief Parse a .obj file and reads all of its data into the model's buffers.
     * @param path The path to the .obj file.
     * @param verbose Whether the funtion will log information about what it's doing.
     */
    void parse_obj_file(const std::filesystem::path& path, bool verbose);

    /**
     * @brief Parse a .mtl file and reads all of the materials it describes.
     * @param path The path to the .mtl file.
     */
    void parse_mtl_file(const std::filesystem::path& path);

    /**
     * @brief Add a mesh to the model.
     * @param positions The positions indexed by the first component of each vertex index.
     * @param normals The normals indexed by the second component of each vertex index.
     * @param tex_coords The texture coordinates indexed by the thord component of each vertex index.
     * @param vertex_indices The vertex indices.
     * @param original_normals_amount The amount of normals in normals before any were added.
     */
    void add_mesh(const std::vector<vec3>& positions,
                  std::vector<vec3>& normals,
                  const std::vector<vec2>& tex_coords,
                  std::vector<ivec3>& vertex_indices,
                  size_t original_normals_amount);

    std::vector<TriangleMesh> meshes;                    ///< The meshes composing the model.
    std::unordered_map<std::string, Material> materials; ///< The model's materials.
};
