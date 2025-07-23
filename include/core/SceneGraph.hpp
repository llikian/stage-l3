/***************************************************************************************************
 * @file  SceneGraph.hpp
 * @brief Declaration of the SceneGraph class
 **************************************************************************************************/

#pragma once

#include <functional>
#include <vector>

#include "culling/AABB.hpp"
#include "maths/Transform.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/Model.hpp"
#include "mesh/Scene.hpp"
#include "mesh/Terrain.hpp"
#include "Node.hpp"

#define ADD_NODE_PARAMETERS const std::string& name, unsigned int parent

enum class DataType : unsigned char {
    NONE,

    COLOR,

    SHADER,
    MATERIAL,
    AABB,

    MESH,
    MODEL,
    SCENE,
    TERRAIN,
};

struct SceneGraph {
    SceneGraph();
    ~SceneGraph();

    void draw(const mat4& view_projection, const Frustum& frustum);

    void update_transform_and_children(unsigned int node_index = 0);
    void force_update_transform_and_children(unsigned int node_index = 0);

    Node& operator[](unsigned int node_index);

    unsigned int add_simple_node(ADD_NODE_PARAMETERS);
    unsigned int add_mesh_node(ADD_NODE_PARAMETERS, const Mesh* mesh, const Shader* shader);
    unsigned int add_flat_shaded_mesh_node(ADD_NODE_PARAMETERS, const Mesh* mesh, const vec4& color);
    unsigned int add_model_node(ADD_NODE_PARAMETERS, const Model* model, const Shader* shader);
    unsigned int add_scene_node(ADD_NODE_PARAMETERS, const std::filesystem::path& path);
    unsigned int add_terrain_node(ADD_NODE_PARAMETERS, float chunk_size, unsigned int chunks_on_line);

    unsigned int add_material(Material* material);

    void add_imgui_node_tree();
    void add_object_editor_to_imgui_window();

    void set_visibility(unsigned int node_index, bool is_visible);
    void set_is_selected(unsigned int node_index, bool is_selected);

    std::vector<Node> nodes; ///< The scene graph's nodes. The root is always at index 0.
    std::vector<Transform> transforms;

    int flat_shader_index;

    std::vector<vec4> colors;
    std::vector<const Shader*> shaders;
    std::vector<Material*> materials;
    std::vector<AABB> AABBs;
    std::vector<const Mesh*> meshes;
    std::vector<const Model*> models;
    std::vector<Scene> scenes;
    std::vector<Terrain> terrains;

    bool are_AABBs_drawn;
    unsigned int total_drawable_objects;
    unsigned int total_culled_objects;

private:
    void draw(const mat4& view_projection, const Frustum& frustum, unsigned int node_index) ;
    void draw(const mat4& view_projection, const Shader* shader, unsigned int node_index) const;

    void add_node_to_imgui_node_tree(unsigned int node_index);
    unsigned int selected_node;
};
