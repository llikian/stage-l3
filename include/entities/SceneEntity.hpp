/***************************************************************************************************
 * @file  SceneEntity.hpp
 * @brief Declaration of the SceneEntity class
 **************************************************************************************************/

#pragma once

#include "DrawableEntity.hpp"
#include "mesh/Scene.hpp"

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

    /**
     * @brief Recursively draws this entity and its children if they're drawable.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     * @param frustum The view frustum.
     */
    void draw(const mat4& view_projection_matrix, const Frustum& frustum) const override;

    /**
     * @brief Draws the scene.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    void draw(const mat4& view_projection_matrix) const;

private:
    Scene scene;
};
