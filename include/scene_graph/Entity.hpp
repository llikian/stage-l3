/***************************************************************************************************
 * @file  Entity.hpp
 * @brief Declaration of the Entity class
 **************************************************************************************************/

#pragma once

#include <list>
#include "Transform.hpp"
#include "mesh/Model.hpp"
#include "mesh/Terrain.hpp"

enum EntityType {
    ENTITY_TYPE_DEFAULT,
    ENTITY_TYPE_DRAWABLE,
    ENTITY_TYPE_MODEL,
    ENTITY_TYPE_TRIANGLE_MESH,
    ENTITY_TYPE_FLAT_SHADED_MESH,
    ENTITY_TYPE_TERRAIN,
};

/**
 * @class Entity
 * @brief An entity in the scene graph, implements a tree-like classure with an std::list of pointers
 * to its children entities. Handles the propagation of its local transform to its children.
 */
class Entity {
public:
    /**
     * @brief Creates an entity with a specified name.
     * @param name The name of the entity, should be unique.
     */
    explicit Entity(const std::string& name);

    /**
     * @brief Frees the children.
     */
    virtual ~Entity();

    /**
     * @brief Add a child to the entity.
     * @tparam EntityClass The type of entity to add.
     * @tparam Args The types of the arguments of the new child's conclassor.
     * @param child_name The name of the child entity.
     * @param args The arguments of the new child's conclassor.
     * @return The new child's pointer.
     */
    template <typename EntityClass, typename... Args>
    EntityClass* add_child(const std::string& child_name, const Args&... args) {
        EntityClass* child = new EntityClass(child_name, args...);
        children.push_back(child);
        child->parent = this;
        return child;
    }

    /**
     * @brief Changes the visibility for this entity and all its children.
     * @param is_hidden Whether this entity and its children are hidden.
     */
    void set_visibility(bool is_hidden);

    /**
     * @brief Toggles the visibility for this entity and sets all of its children's visibility to
     * the same value.
     */
    void toggle_visibility();

    /**
     * @brief Check if the local model was modified, updates it if it was and do the same thing for
     * all children.
     */
    void update_transform_and_children();

    /**
     * @brief Update the local model of this entity and all its children.
     */
    void force_update_transform_and_children();

    /**
     * @brief Check if the entity is drawable, draws it if it is and do the same thing for all children.
     * @param view_projection_matrix 
     */
    void draw_drawables(const mat4& view_projection_matrix);

    /**
     * @brief Returns whether this entity is drawable.
     * @return false.
     */
    virtual bool is_drawable() const;

    /**
     * @brief Add this entity to the object editor. Allows to modify these fields in the entity:\n
     * - The transform's local position\n
     * - The transform's local orientation\n
     * - The transform's local scale\n
     * - Whether the entity is hidden
     */
    virtual void add_to_object_editor();

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_DEFAULT.
     */
    virtual constexpr EntityType get_type() const { return ENTITY_TYPE_DEFAULT; }

    std::string name;            ///< The entity's name.
    std::list<Entity*> children; ///< The entity's children
    Entity* parent;              ///< The entity's parent.
    Transform transform;         ///< The entity's transform.

protected:
    bool is_hidden; ///< Whether the mesh is hidden: it should not be drawn.
};

/**
 * @class DrawableEntity
 * @brief Pure virtual class that represents an entity that can be drawn.
 */
class DrawableEntity : public Entity {
public:
    /**
     * @brief Creates an entity with a certain name and a pointer to the shader that will be used
     * when rendering.
     * @param name The name of the entity.
     * @param shader A pointer to the shader used when rendering.
     */
    DrawableEntity(const std::string& name, const Shader* shader);

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

    const Shader* shader; ///< A pointer to the shader used when rendering.
};

/**
 * @class ModelEntity
 * @brief A drawable entity that holds a model.
 */
class ModelEntity : public DrawableEntity {
public:
    /**
     * @brief Creates an entity with a certain name, a pointer to the shader that will be used
     * when rendering and the path to the model to render.
     * @param name The name of the entity.
     * @param shader A pointer to the shader used when rendering.
     * @param path The path to the model to render.
     */
    ModelEntity(const std::string& name, const Shader* shader, const std::filesystem::path& path);

    /**
     * @brief Updates uniforms then draws the model.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    void draw(const mat4& view_projection_matrix) override;

    /**
     * @brief Add this entity to the object editor. Allows to modify these fields in the entity:\n
     * - The transform's local position\n
     * - The transform's local orientation\n
     * - The transform's local scale\n
     * - Whether the entity is hidden\n
     * - The model's materials: ambient, diffuse and specular colors, specular exponent
     */
    void add_to_object_editor() override;

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_MODEL.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_MODEL; }

    Model model; ///< The model to render.
};

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

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_TRIANGLE_MESH.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_TRIANGLE_MESH; }

    TriangleMesh mesh; ///< The mesh to render.
};

/**
 * @class FlatShadedMeshEntity
 * @brief A drawable that holds a mesh and a color in order to render it with flat shading.
 */
class FlatShadedMeshEntity : public TriangleMeshEntity {
public:
    /**
     * @brief Creates an entity with a certain name, a pointer to the shader that will be used
     * when rendering and the color of the mesh.
     * @param name The name of the entity.
     * @param shader A pointer to the shader used when rendering.
     * @param color The color the mesh should be rendered in.
     */
    FlatShadedMeshEntity(const std::string& name, const Shader* shader, const vec3& color = vec3(1.0f));

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

    vec3 color; ///< The color the mesh should be rendered in.
};

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

    /**
     * @brief Returns the type of the entity.
     * @return ENTITY_TYPE_TRIANGLE_MESH.
     */
    constexpr EntityType get_type() const override { return ENTITY_TYPE_TERRAIN; }

    Terrain terrain; ///< The terrain.
};
