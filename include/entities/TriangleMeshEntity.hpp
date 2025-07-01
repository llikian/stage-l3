/***************************************************************************************************
 * @file  TriangleMeshEntity.hpp
 * @brief Declaration of the TriangleMeshEntity class
 **************************************************************************************************/

#pragma once

#include "DrawableEntity.hpp"

/**
 * @class TriangleMeshEntity
 * @brief A drawable entity that holds a triangle mesh.
 */
class TriangleMeshEntity : public DrawableEntity {
public:
    /**
     * @brief Creates an entity with a certain name and a pointer to the shader that will be used
     * when rendering.
     * @param name The name of the entity.
     * @param shader A pointer to the shader used when rendering.
     */
    TriangleMeshEntity(const std::string& name, const Shader* shader);

    /**
     * @brief Updates uniforms then draws the mesh.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    void draw(const mat4& view_projection_matrix) override;

    /**
     * @brief Add this entity to the object editor. Allows to modify these fields in the entity:\n
     * - The transform's local position\n
     * - The transform's local orientation\n
     * - The transform's local scale\n
     * - Whether the entity is hidden\n
     * - The mesh's material: ambient, diffuse and specular colors, specular exponent
     */
    void add_to_object_editor() override;

    void create_aabb() override;

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_TRIANGLE_MESH.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_TRIANGLE_MESH; }

    TriangleMesh mesh; ///< The mesh to render.
};