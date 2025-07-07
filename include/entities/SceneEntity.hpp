/***************************************************************************************************
 * @file  SceneEntity.hpp
 * @brief Declaration of the SceneEntity class
 **************************************************************************************************/

#pragma once

#include <filesystem>
#include "Entity.hpp"

/**
 * @class SceneEntity
 * @brief
 */
class SceneEntity : public Entity {
public:
    /**
     * @brief Creates an entity from a GLTF file.
     * @param name The name of the entity.
     * @param path The path to a GLTF file.
     */
    SceneEntity(const std::string& name, const std::filesystem::path& path);

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_SCENE.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_SCENE; }

private:
    void load(const std::filesystem::path& path);
};
