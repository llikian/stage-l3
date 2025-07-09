/***************************************************************************************************
 * @file  AssetManager.cpp
 * @brief Implementation of the AssetManager class
 **************************************************************************************************/

#include "AssetManager.hpp"

#include <ranges>
#include "mesh/primitives.hpp"

Shader& AssetManager::add_shader(const std::string& name,
                                 const std::initializer_list<std::filesystem::path>& paths_list) {
    return get().shaders.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(name),
                                 std::forward_as_tuple(paths_list, name))
                .first->second;
}

Texture& AssetManager::add_texture(const std::filesystem::path& path) {
    AssetManager& asset_manager = get();
    auto iterator = asset_manager.textures.find(path);

    return iterator == asset_manager.textures.end()
               ? asset_manager.textures.emplace(path.string(), Texture(path)).first->second
               : iterator->second;
}

Texture& AssetManager::add_texture(const std::string& name, const vec3& color) {
    return get().textures.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(name),
                                 std::forward_as_tuple(color))
                .first->second;
}

Model& AssetManager::add_model(const std::string& name, const std::filesystem::path& path) {
    return get().models.emplace(std::piecewise_construct,
                                std::forward_as_tuple(name),
                                std::forward_as_tuple(path))
                .first->second;
}

Mesh& AssetManager::add_mesh(const std::string& name) {
    return get().meshes.emplace(name, Mesh()).first->second;
}

Shader& AssetManager::get_shader(const std::string& shader_name) {
    AssetManager& asset_manager = get();

    auto iterator = asset_manager.shaders.find(shader_name);
    if(iterator == asset_manager.shaders.end()) {
        throw std::runtime_error("Couldn't find shader '" + shader_name + "' in asset manager");
    }

    return iterator->second;
}

Texture& AssetManager::get_texture(const std::string& texture_name_or_path) {
    AssetManager& asset_manager = get();

    auto iterator = asset_manager.textures.find(texture_name_or_path);
    if(iterator == asset_manager.textures.end()) {
        throw std::runtime_error("Couldn't find texture '" + texture_name_or_path + "' in asset manager");
    }

    return iterator->second;
}

Model& AssetManager::get_model(const std::string& model_name) {
    AssetManager& asset_manager = get();

    auto iterator = asset_manager.models.find(model_name);
    if(iterator == asset_manager.models.end()) {
        throw std::runtime_error("Couldn't find model '" + model_name + "' in asset manager");
    }

    return iterator->second;
}

Mesh& AssetManager::get_mesh(const std::string& mesh_name) {
    AssetManager& asset_manager = get();

    auto iterator = asset_manager.meshes.find(mesh_name);
    if(iterator == asset_manager.meshes.end()) {
        throw std::runtime_error("Couldn't find triangle mesh '" + mesh_name + "' in asset manager");
    }

    return iterator->second;
}

Shader& AssetManager::get_relevant_shader_from_mesh(const Mesh& mesh) {
    AssetManager& asset_manager = get();

    switch(mesh.get_primitive()) {
        case Primitive::POINTS:
            return asset_manager.shaders["point mesh"];
        case Primitive::LINES:
            return asset_manager.shaders[mesh.has_attribute(Attribute::COLOR) ? "line mesh" : "flat"];
        case Primitive::TRIANGLES:
            return asset_manager.shaders[mesh.has_attribute(Attribute::NORMAL) ? "blinn-phong" : "flat"];
        default:
            return asset_manager.shaders["flat"];
    }
}

AssetManager::AssetManager() { }

AssetManager::~AssetManager() {
    for(Shader& shader : shaders | std::views::values) { shader.free(); }
    for(Texture& texture : textures | std::views::values) { texture.free(); }
}
