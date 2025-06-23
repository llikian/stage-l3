/***************************************************************************************************
 * @file  Entity.hpp
 * @brief Declaration of the Entity class
 **************************************************************************************************/

#pragma once

#include <list>
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

    virtual void add_to_object_editor();

    std::string name;
    std::list<Entity*> children;
    Entity* parent;
    Transform transform;
};

struct DrawableEntity : Entity {
    DrawableEntity(const std::string& name, Shader* shader);

    virtual void draw(const mat4& view_projection_matrix) = 0;
    virtual void update_uniforms(const mat4& view_projection_matrix) const;

    bool is_drawable() const override;

    Shader* shader;
};

struct ModelEntity : DrawableEntity {
    ModelEntity(const std::string& name, Shader* shader, const std::filesystem::path& path);

    void draw(const mat4& view_projection_matrix) override;

    Model model;
};

struct TriangleMeshEntity : DrawableEntity {
    TriangleMeshEntity(const std::string& name, Shader* shader);

    void draw(const mat4& view_projection_matrix) override;

    TriangleMesh mesh;
};

struct FlatShadedMeshEntity : TriangleMeshEntity {
    FlatShadedMeshEntity(const std::string& name, Shader* shader, const vec3& color = vec3(1.0f));

    void update_uniforms(const mat4& view_projection_matrix) const override;

    void add_to_object_editor() override;

    vec3 color;
};
