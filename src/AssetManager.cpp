/***************************************************************************************************
 * @file  AssetManager.cpp
 * @brief Implementation of the AssetManager class
 **************************************************************************************************/

#include "AssetManager.hpp"

#include <ranges>

AssetManager::~AssetManager() {
    for(Shader& shader : shaders | std::views::values) { shader.free(); }
    for(Texture& texture : textures | std::views::values) { texture.free(); }
}

Shader& AssetManager::add_shader(const std::string& name,
                                 const std::initializer_list<std::filesystem::path>& paths_list) {
    return get_instance().shaders.emplace(std::piecewise_construct,
                                          std::forward_as_tuple(name),
                                          std::forward_as_tuple(paths_list, name))
                         .first->second;
}

Texture& AssetManager::add_texture(const std::filesystem::path& path) {
    AssetManager& asset_manager = get_instance();
    auto iterator = asset_manager.textures.find(path);

    return iterator == asset_manager.textures.end()
               ? asset_manager.textures.emplace(path.string(), Texture(path)).first->second
               : iterator->second;
}

Model& AssetManager::add_model(const std::string& name, const std::filesystem::path& path) {
    return get_instance().models.emplace(std::piecewise_construct,
                                         std::forward_as_tuple(name),
                                         std::forward_as_tuple(path))
                         .first->second;
}

TriangleMesh& AssetManager::add_triangle_mesh(const std::string& name) {
    return get_instance().triangle_meshes.emplace(name, TriangleMesh()).first->second;
}

LineMesh& AssetManager::add_line_mesh(const std::string& name) {
    return get_instance().line_meshes.emplace(name, LineMesh()).first->second;
}

Shader& AssetManager::get_shader(const std::string& shader_name) {
    AssetManager& asset_manager = get_instance();

    auto iterator = asset_manager.shaders.find(shader_name);
    if(iterator == asset_manager.shaders.end()) {
        throw std::runtime_error("Couldn't find shader '" + shader_name + "' in asset manager");
    }

    return iterator->second;
}

Texture& AssetManager::get_texture(const std::string& texture_path) {
    AssetManager& asset_manager = get_instance();

    auto iterator = asset_manager.textures.find(texture_path);
    if(iterator == asset_manager.textures.end()) {
        throw std::runtime_error("Couldn't find texture '" + texture_path + "' in asset manager");
    }

    return iterator->second;
}

Model& AssetManager::get_model(const std::string& model_name) {
    AssetManager& asset_manager = get_instance();

    auto iterator = asset_manager.models.find(model_name);
    if(iterator == asset_manager.models.end()) {
        throw std::runtime_error("Couldn't find model '" + model_name + "' in asset manager");
    }

    return iterator->second;
}

TriangleMesh& AssetManager::get_triangle_mesh(const std::string& mesh_name) {
    AssetManager& asset_manager = get_instance();

    auto iterator = asset_manager.triangle_meshes.find(mesh_name);
    if(iterator == asset_manager.triangle_meshes.end()) {
        throw std::runtime_error("Couldn't find triangle mesh '" + mesh_name + "' in asset manager");
    }

    return iterator->second;
}

LineMesh& AssetManager::get_line_mesh(const std::string& mesh_name) {
    AssetManager& asset_manager = get_instance();

    auto iterator = asset_manager.line_meshes.find(mesh_name);
    if(iterator == asset_manager.line_meshes.end()) {
        throw std::runtime_error("Couldn't find line meshe '" + mesh_name + "' in asset manager");
    }

    return iterator->second;
}
