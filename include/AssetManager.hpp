/***************************************************************************************************
 * @file  AssetManager.hpp
 * @brief Declaration of the AssetManager class
 **************************************************************************************************/

#pragma once

#include <functional>

#include "mesh/LineMesh.hpp"
#include "mesh/Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

/**
 * @class AssetManager
 * @brief
 */
class AssetManager {
public:
    ~AssetManager();

    static inline AssetManager& get_instance() {
        static AssetManager asset_manager;
        return asset_manager;
    }

    /**
     * @brief Add a shader to the unordered_map.
     * @param name The name of the shader (key).
     * @param paths_list The paths to each of the different shaders (parameter to the value's constructor).
     */
    static Shader& add_shader(const std::string& name, const std::initializer_list<std::filesystem::path>& paths_list);
    static Texture& add_texture(const std::filesystem::path& path);
    static Model& add_model(const std::string& name, const std::filesystem::path& path);
    static TriangleMesh& add_triangle_mesh(const std::string& name);

    template <typename MeshFunc, typename... Args>
    static TriangleMesh& add_triangle_mesh(const std::string& name, MeshFunc&& create_mesh, Args&&... args) {
        TriangleMesh& mesh = get_instance().triangle_meshes.emplace(name, TriangleMesh()).first->second;
        std::invoke(std::forward<MeshFunc>(create_mesh), mesh, std::forward<Args>(args)...);
        return mesh;
    }

    static LineMesh& add_line_mesh(const std::string& name);

    template <typename MeshFunc, typename... Args>
    static LineMesh& add_line_mesh(const std::string& name, MeshFunc&& create_mesh, Args&&... args) {
        LineMesh& mesh = get_instance().line_meshes.emplace(name, LineMesh()).first->second;
        std::invoke(std::forward<MeshFunc>(create_mesh), mesh, std::forward<Args>(args)...);
        return mesh;
    }

    static Shader& get_shader(const std::string& shader_name);
    static Texture& get_texture(const std::string& texture_path);
    static Model& get_model(const std::string& model_name);
    static TriangleMesh& get_triangle_mesh(const std::string& mesh_name);
    static LineMesh& get_line_mesh(const std::string& mesh_name);

private:
    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, Texture> textures;
    std::unordered_map<std::string, Model> models;
    std::unordered_map<std::string, TriangleMesh> triangle_meshes;
    std::unordered_map<std::string, LineMesh> line_meshes;
};
