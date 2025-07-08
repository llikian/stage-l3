/***************************************************************************************************
 * @file  Model.hpp
 * @brief Declaration of the Model class
 **************************************************************************************************/

#pragma once

#include <filesystem>
#include <vector>
#include "Material.hpp"
#include "maths/vec3.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

/**
 * @class Model
 * @brief Create 3D models by reading data from various file formats.
 */
class Model {
public:
    friend class ModelEntity;

    /**
     * @brief Creates a mesh by reading the file at a certain path. Currently supports only .obj files.
     * @param path The path to the file.
     */
    explicit Model(const std::filesystem::path& path);

    /**
     * @brief Performs a draw call for each of the model's meshes with a certain shader.
     * @param shader The shader to perform the draw calls with.
     */
    void draw(const Shader& shader);

    /**
     * @brief Applies a model matrix to each mesh in the model.
     * @param model The model matrix to apply.
     */
    void apply_model_matrix(const mat4& model);

    void get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const;

private:
    /**
     * @brief Parse a .obj file and reads all of its data into the model's buffers.
     * @param path The path to the .obj file.
     */
    void parse_obj_file(const std::filesystem::path& path);

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
     */
    void add_mesh(const std::vector<vec3>& positions,
                  std::vector<vec3>& normals,
                  const std::vector<vec2>& tex_coords,
                  std::vector<llvec3>& vertex_indices);

    std::vector<Mesh> meshes;  ///< The meshes composing the model.
    std::vector<Material> materials; ///< The model's materials.
};
