/***************************************************************************************************
 * @file  Entity.hpp
 * @brief Declaration of the Entity class
 **************************************************************************************************/

#pragma once

#include <list>
#include "culling/Frustum.hpp"
#include "maths/Transform.hpp"

enum EntityType {
    ENTITY_TYPE_DEFAULT,
    ENTITY_TYPE_DRAWABLE,
    ENTITY_TYPE_MODEL,
    ENTITY_TYPE_TRIANGLE_MESH,
    ENTITY_TYPE_LINE_MESH,
    ENTITY_TYPE_FLAT_SHADED_MESH,
    ENTITY_TYPE_TERRAIN,
    ENTITY_TYPE_SCENE,
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
    EntityClass* add_child(const std::string& child_name, Args&&... args) {
        EntityClass* child = new EntityClass(child_name, std::forward<Args>(args)...);
        children.push_back(child);
        child->parent = this;
        return child;
    }

    /**
     * @brief Changes the visibility for this entity and all its children.
     * @param is_visible Whether this entity and its children are visible.
     */
    void set_visibility(bool is_visible);

    /**
     * @return Whether the entity is visible.
     */
    bool get_visibility() const;

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
     * @param frustum
     */
    void draw_drawables(const mat4& view_projection_matrix, const Frustum& frustum);

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
    bool is_visible; ///< Whether the entity is visible.
};
