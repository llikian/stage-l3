/***************************************************************************************************
 * @file  Scene.hpp
 * @brief Declaration of the Scene class
 **************************************************************************************************/

#pragma once

#include <filesystem>
#include "cgltf.h"
#include "maths/Transform.hpp"
#include "mesh/Mesh.hpp"

struct AttributeInfo {
    Attribute attribute;
    AttributeType type;
    std::vector<float> data;
};

/**
 * @class Scene
 * @brief
 */
class Scene {
public:
    explicit Scene(const std::filesystem::path& path);
    ~Scene();

    void draw(const mat4& view_projection_matrix, const Transform& transform) const;

    static void check_cgltf_result(cgltf_result result, const std::string& error_message);
    static std::string cgltf_primitive_type_to_string(cgltf_primitive_type primitive_type);
    static std::string cgltf_attribute_type_to_string(cgltf_attribute_type attribute_type);
    static std::string cgltf_type_to_string(cgltf_type type);

private:
    Mesh** meshes;
    unsigned int meshes_count;
    unsigned int* primitives_count;

    void load(const std::filesystem::path& path);
    static void read_attribute(AttributeInfo& attribute_info, const cgltf_attribute& c_attribute);
};
