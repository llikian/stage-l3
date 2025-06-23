/***************************************************************************************************
 * @file  Entity.hpp
 * @brief Declaration of the Entity class
 **************************************************************************************************/

#pragma once

#include <list>
#include <memory>
#include "Transform.hpp"
#include "mesh/Model.hpp"

/**
 * @class Entity
 * @brief
 */
struct Entity {
    explicit Entity(const std::string& name);
    virtual ~Entity();

    template <typename EntityType, typename... Args>
    EntityType* add_child(const std::string& child_name, const Args&... args) {
        EntityType* child = new EntityType(child_name, args...);
        children.push_back(child);
        child->parent = this;
        return child;
    }

    void update_transform_and_children();

    void draw_drawables(const mat4& view_projection_matrix);

    virtual bool is_drawable() const;

    std::string name;
    std::list<Entity*> children;
    Entity* parent;
    Transform transform;
};

struct DrawableEntity : Entity {
    explicit DrawableEntity(const std::string& name, Shader* shader);

    virtual void draw(const mat4& view_projection_matrix) = 0;
    virtual void update_uniforms(const mat4& view_projection_matrix) const;

    bool is_drawable() const override;

    Shader* shader;
};

struct ModelEntity : DrawableEntity {
    explicit ModelEntity(const std::string& name, Shader* shader, const std::filesystem::path& path);

    void draw(const mat4& view_projection_matrix) override;

    Model model;
};

struct TriangleMeshEntity : DrawableEntity {
    explicit TriangleMeshEntity(const std::string& name, Shader* shader);

    void draw(const mat4& view_projection_matrix) override;

    TriangleMesh mesh;
};
