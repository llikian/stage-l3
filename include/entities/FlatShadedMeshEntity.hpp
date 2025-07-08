/***************************************************************************************************
 * @file  FlatShadedMeshEntity.hpp
 * @brief Declaration of the FlatShadedMeshEntity class
 **************************************************************************************************/

#pragma once

#include "MeshEntity.hpp"

/**
 * @class FlatShadedMeshEntity
 * @brief A drawable that holds a mesh and a color in order to render it with flat shading.
 */
class FlatShadedMeshEntity : public MeshEntity {
public:
    /**
     * @brief Creates an entity with a certain name, the shader that will be used when rendering and
     * the color of the mesh.
     * @param name The name of the entity.
     * @param shader The shader used when rendering.
     * @param mesh The mesh to render.
     * @param color The color the mesh should be rendered in.
     */
    FlatShadedMeshEntity(const std::string& name,
                         const Shader& shader,
                         BetterMesh& mesh,
                         const vec4& color = vec4(1.0f));

    /**
     * @brief Updates these uniforms if they exist in the shader:\n
     * - u_mvp\n
     * - u_model\n
     * - u_normals_model_matrix\n
     * - u_color
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    void update_uniforms(const mat4& view_projection_matrix) override;

    /**
     * @brief Add this entity to the object editor. Allows to modify these fields in the entity:\n
     * - The transform's local position\n
     * - The transform's local orientation\n
     * - The transform's local scale\n
     * - The mesh's color
     */
    void add_to_object_editor() override;

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_FLAT_SHADED_MESH.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_FLAT_SHADED_MESH; }

    vec4 color; ///< The color the mesh should be rendered in.
};
