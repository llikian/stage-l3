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

Texture& AssetManager::add_texture(const std::filesystem::path& path, bool flip_vertically, bool srgb) {
    AssetManager& asset_manager = get();
    auto iterator = asset_manager.textures.find(path);

    if(iterator == asset_manager.textures.end()) {
        Texture texture;
        texture.create(path, flip_vertically, srgb);
        return asset_manager.textures.emplace(path.string(), texture).first->second;
    }

    return iterator->second;
}

Texture& AssetManager::add_texture(const std::string& name, const Texture& texture) {
    return get().textures.emplace(name, texture).first->second;
}

Texture& AssetManager::add_texture(const std::string& name, const vec3& color) {
    Texture texture;
    texture.create(color);
    return get().textures.emplace(name, texture).first->second;
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

Shader* AssetManager::get_shader_ptr(const std::string& shader_name) {
    AssetManager& asset_manager = get();

    auto iterator = asset_manager.shaders.find(shader_name);
    return iterator == asset_manager.shaders.end() ? nullptr : &iterator->second;
}

Texture* AssetManager::get_texture_ptr(const std::string& texture_name_or_path) {
    AssetManager& asset_manager = get();

    auto iterator = asset_manager.textures.find(texture_name_or_path);
    return iterator == asset_manager.textures.end() ? nullptr : &iterator->second;
}

Model* AssetManager::get_model_ptr(const std::string& model_name) {
    AssetManager& asset_manager = get();

    auto iterator = asset_manager.models.find(model_name);
    return iterator == asset_manager.models.end() ? nullptr : &iterator->second;
}

Mesh* AssetManager::get_mesh_ptr(const std::string& mesh_name) {
    AssetManager& asset_manager = get();

    auto iterator = asset_manager.meshes.find(mesh_name);
    return iterator == asset_manager.meshes.end() ? nullptr : &iterator->second;
}

bool AssetManager::has_shader(const std::string& shader_name) {
    return get().shaders.contains(shader_name);
}

bool AssetManager::has_texture(const std::string& texture_name_or_path) {
    return get().textures.contains(texture_name_or_path);
}

bool AssetManager::has_model(const std::string& model_name) {
    return get().models.contains(model_name);
}

bool AssetManager::has_mesh(const std::string& mesh_name) {
    return get().meshes.contains(mesh_name);
}

Shader& AssetManager::get_relevant_shader_from_mesh(const Mesh& mesh) {
    AssetManager& asset_manager = get();

    switch(mesh.get_primitive()) {
        case Primitive::POINTS:
            return asset_manager.shaders["point mesh"];
        case Primitive::LINES:
            return asset_manager.shaders[mesh.has_attribute(ATTRIBUTE_COLOR) ? "line mesh" : "flat"];
        case Primitive::TRIANGLES:
            return asset_manager.shaders[mesh.has_attribute(ATTRIBUTE_NORMAL) ? "blinn-phong" : "flat"];
        default:
            return asset_manager.shaders["flat"];
    }
}

AssetManager::AssetManager() { }

AssetManager::~AssetManager() {
    for(Shader& shader : shaders | std::views::values) { shader.free(); }
    for(Texture& texture : textures | std::views::values) { texture.free(); }
}
