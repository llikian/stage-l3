/***************************************************************************************************
 * @file  DrawableEntity.hpp
 * @brief Declaration of the DrawableEntity class
 **************************************************************************************************/

#pragma once

#include "culling/AABB.hpp"
#include "Entity.hpp"
#include "Shader.hpp"

/**
 * @class DrawableEntity
 * @brief Pure virtual class that represents an entity that can be drawn.
 */
class DrawableEntity : public Entity {
public:
    /**
     * @brief Creates an entity with a certain name and a the shader that will be used when rendering.
     * @param name The name of the entity.
     * @param shader The shader used when rendering.
     */
    DrawableEntity(const std::string& name, const Shader& shader);

    /**
     * @brief Frees the aabb.
     */
    ~DrawableEntity() override;

    /**
     * @brief Recursively draws this entity and its children if they're drawable.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     * @param frustum The view frustum.
     */
    void draw(const mat4& view_projection_matrix, const Frustum& frustum) const override;

    /**
     * @brief Updates uniforms then draws the entity.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    virtual void draw(const mat4& view_projection_matrix) const = 0;

    /**
     * @brief Updates these uniforms if they exist in the shader:\n
     * - u_mvp\n
     * - u_model\n
     * - u_normals_model_matrix
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    virtual void update_uniforms(const mat4& view_projection_matrix) const;

    /**
     * @brief Creates the aabb for this entity, a bounding volume used for optimization.
     */
    virtual void create_aabb() = 0;

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_DRAWABLE.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_DRAWABLE; }

    const Shader& shader; ///< A pointer to the shader used when rendering.
    AABB* aabb;           ///< The bounding volume of the entity.

    static inline unsigned int total_drawable_entities = 0;
    static inline unsigned int total_not_hidden_entities = 0;
    static inline unsigned int total_drawn_entities = 0;
};
