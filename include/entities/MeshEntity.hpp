/***************************************************************************************************
 * @file  MeshEntity.hpp
 * @brief Declaration of the MeshEntity class
 **************************************************************************************************/

#pragma once

#include "DrawableEntity.hpp"
#include "mesh/Material.hpp"
#include "mesh/Mesh.hpp"

/**
 * @class MeshEntity
 * @brief A drawable entity that holds a mesh.
 */
class MeshEntity : public DrawableEntity {
public:
    /**
     * @brief Creates an entity with a certain name, the shader that will be used when rendering and
     * the mesh to render.
     * @param name The name of the entity.
     * @param shader The shader used when rendering.
     * @param mesh The mesh to render.
     */
    MeshEntity(const std::string& name, const Shader& shader, Mesh& mesh);

    /**
     * @brief Updates uniforms then draws the mesh.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    void draw(const mat4& view_projection_matrix) override;

    /**
     * @brief Updates these uniforms if they exist in the shader:\n
     * - u_mvp\n
     * - u_model\n
     * - u_normals_model_matrix\n
     * - The material's uniforms
     * @param view_projection_matrix
     */
    void update_uniforms(const mat4& view_projection_matrix) override;

    /**
     * @brief Add this entity to the object editor. Allows to modify these fields in the entity:\n
     * - The transform's local position\n
     * - The transform's local orientation\n
     * - The transform's local scale\n
     * - Whether the entity is hidden\n
     * - The mesh's material: ambient, diffuse and specular colors, specular exponent
     */
    void add_to_object_editor() override;

    /**
     * @brief Creates an AABB bounding volume for the entity by computing the mesh's minimum and
     * maximum coordinates.
     */
    void create_aabb() override;

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_TRIANGLE_MESH.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_MESH; }

    Mesh& mesh; ///< The mesh to render.
    Material* material; ///< The mesh's material.
};
