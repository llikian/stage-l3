/***************************************************************************************************
 * @file  Scene.hpp
 * @brief Declaration of the Scene class
 **************************************************************************************************/

#pragma once

#include <filesystem>
#include "assets/Shader.hpp"
#include "cgltf.h"
#include "entities/Entity.hpp"
#include "materials/MRMaterial.hpp"
#include "maths/Transform.hpp"
#include "mesh/Mesh.hpp"

class SceneEntity;

struct AttributeInfo {
    Attribute attribute;
    AttributeType type;
    unsigned int component_count;
    std::vector<float> data;
};

struct PrimitiveInfo {
    PrimitiveInfo();
    ~PrimitiveInfo();

    bool has_transparency() const;

    std::string name;
    Mesh mesh;
    MRMaterial* material;
    const Shader* shader;
};

struct MeshInfo {
    MeshInfo();
    ~MeshInfo();

    std::string name;
    PrimitiveInfo* primitives;
    unsigned int primitive_count;
};

/**
 * @class Scene
 * @brief
 */
class Scene {
public:
    Scene(const std::filesystem::path& path, SceneEntity* entity);
    ~Scene();

    void add_children(SceneEntity* entity) const;

    static void check_cgltf_result(cgltf_result result, const std::string& error_message);
    static std::string cgltf_primitive_type_to_string(cgltf_primitive_type primitive_type);
    static std::string cgltf_attribute_type_to_string(cgltf_attribute_type attribute_type);
    static std::string cgltf_type_to_string(cgltf_type type);

private:
    MeshInfo* meshes;
    unsigned int meshes_count;

    void load(const std::filesystem::path& path, SceneEntity* entity);
    void add_node(const cgltf_node* c_node,
                  Entity* entity,
                  const std::unordered_map<const cgltf_mesh*, unsigned int>& mesh_indices);
    static void read_attribute(AttributeInfo& attribute_info, const cgltf_attribute& c_attribute);
};
