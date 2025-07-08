/***************************************************************************************************
 * @file  ModelEntity.hpp
 * @brief Declaration of the ModelEntity class
 **************************************************************************************************/

#pragma once

#include "DrawableEntity.hpp"
#include "mesh/Model.hpp"

/**
 * @class ModelEntity
 * @brief A drawable entity that holds a model.
 */
class ModelEntity : public DrawableEntity {
public:
    /**
     * @brief Creates an entity with a certain name, the shader that will be used when rendering and
     * the model to render.
     * @param name The name of the entity.
     * @param shader The shader used when rendering.
     * @param model The model to render.
     */
    ModelEntity(const std::string& name, const Shader& shader, Model& model);

    /**
     * @brief Updates uniforms then draws the model.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    void draw(const mat4& view_projection_matrix) const override;

    /**
     * @brief Add this entity to the object editor. Allows to modify these fields in the entity:\n
     * - The transform's local position\n
     * - The transform's local orientation\n
     * - The transform's local scale\n
     * - Whether the entity is hidden\n
     * - The model's materials: ambient, diffuse and specular colors, specular exponent
     */
    void add_to_object_editor() override;

    void create_aabb() override;

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_MODEL.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_MODEL; }

    Model& model; ///< The model to render.
};
