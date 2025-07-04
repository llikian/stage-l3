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

    ~DrawableEntity() override;

    /**
     * @brief Updates uniforms then draws the entity.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    virtual void draw(const mat4& view_projection_matrix) = 0;

    /**
     * @brief Updates these uniforms if they exist in the shader:\n
     * - u_mvp\n
     * - u_model\n
     * - u_normals_model_matrix
     * @param view_projection_matrix
     */
    virtual void update_uniforms(const mat4& view_projection_matrix);

    virtual void create_aabb() = 0;

    /**
     * @brief Returns whether this entity is drawable.
     * @return true.
     */
    bool is_drawable() const override;

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_DRAWABLE.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_DRAWABLE; }

    const Shader& shader; ///< A pointer to the shader used when rendering.
    AABB* aabb;

    static inline unsigned int total_drawable_entities = 0;
    static inline unsigned int total_not_hidden_entities = 0;
    static inline unsigned int total_drawn_entities = 0;
};
