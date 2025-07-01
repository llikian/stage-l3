/***************************************************************************************************
 * @file  TerrainEntity.hpp
 * @brief Declaration of the TerrainEntity class
 **************************************************************************************************/

#pragma once

#include "DrawableEntity.hpp"
#include "mesh/Terrain.hpp"

/**
 * @class TerrainEntity
 * @brief A drawable entity that holds a terrain.
 */
class TerrainEntity : public DrawableEntity {
public:
    /**
     * @brief Creates an entity with a certain name and a pointer to the shader that will be used
     * when rendering and the terrain's chunk size and amount of chunks on a line.
     * @param name The name of the entity.
     * @param shader The shader used when rendering.
     * @param chunk_size How large a chunk of the terrain is.
     * @param chunks_on_line The amount of chunks on one line of the terrain.
     */
    TerrainEntity(const std::string& name, const Shader& shader, float chunk_size, unsigned int chunks_on_line);

    /**
     * @brief Updates uniforms then draws the mesh.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    void draw(const mat4& view_projection_matrix) override;

    /**
     * @brief Add this entity to the object editor. Allows to modify these fields in the entity:\n
     * - Whether the entity is hidden\n
     * - The terrain's // TODO : update method and doc
     */
    void add_to_object_editor() override;

    void create_aabb() override;

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_TRIANGLE_MESH.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_TERRAIN; }

    Terrain terrain; ///< The terrain.
};