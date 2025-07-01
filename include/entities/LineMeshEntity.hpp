/***************************************************************************************************
 * @file  LineMeshEntity.hpp
 * @brief Declaration of the LineMeshEntity class
 **************************************************************************************************/

#pragma once

#include "DrawableEntity.hpp"
#include "mesh/LineMesh.hpp"

/**
 * @class LineMeshEntity
 * @brief A drawable entity that holds a line mesh.
 */
class LineMeshEntity : public DrawableEntity {
public:
    /**
     * @brief Creates an entity with a certain name and a pointer to the shader that will be used
     * when rendering.
     * @param name The name of the entity.
     * @param shader A pointer to the shader used when rendering.
     */
    LineMeshEntity(const std::string& name, const Shader* shader);

    /**
     * @brief Updates uniforms then draws the mesh.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    void draw(const mat4& view_projection_matrix) override;

    void create_aabb() override;

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_LINE_MESH.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_LINE_MESH; }

    LineMesh mesh; ///< The mesh to render.
};