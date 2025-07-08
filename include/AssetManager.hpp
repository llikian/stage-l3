/***************************************************************************************************
 * @file  AssetManager.hpp
 * @brief Declaration of the AssetManager class
 **************************************************************************************************/

#pragma once

#include <functional>

#include "mesh/Mesh.hpp"
#include "mesh/Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

/**
 * @class AssetManager
 * @brief
 */
class AssetManager {
public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    static inline AssetManager& get() {
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
    static Mesh& add_mesh(const std::string& name);

    template <typename MeshFunc, typename... Args>
    static Mesh& add_mesh(const std::string& name, MeshFunc&& create_mesh, Args&&... args) {
        Mesh& mesh = get().meshes.emplace(name, Mesh()).first->second;
        std::invoke(std::forward<MeshFunc>(create_mesh), mesh, std::forward<Args>(args)...);
        return mesh;
    }

    template <typename MeshFunc, typename... Args>
    static void add_two_meshes(const std::string& first,
                               const std::string& second,
                               MeshFunc&& create_mesh,
                               Args&&... args) {
        AssetManager& asset_manager = get();
        std::invoke(std::forward<MeshFunc>(create_mesh),
                    asset_manager.meshes.emplace(first, Mesh()).first->second,
                    asset_manager.meshes.emplace(second, Mesh()).first->second,
                    std::forward<Args>(args)...);
    }

    static Shader& get_shader(const std::string& shader_name);
    static Texture& get_texture(const std::string& texture_path);
    static Model& get_model(const std::string& model_name);
    static Mesh& get_mesh(const std::string& mesh_name);

private:
    AssetManager();
    ~AssetManager();

    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, Texture> textures;
    std::unordered_map<std::string, Model> models;
    std::unordered_map<std::string, Mesh> meshes;
};
