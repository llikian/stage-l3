/***************************************************************************************************
 * @file  SceneGraph.cpp
 * @brief Implementation of the SceneGraph class
 **************************************************************************************************/

#include "core/SceneGraph.hpp"

#include <stack>
#include "assets/AssetManager.hpp"
#include "core/Node.hpp"
#include "imgui.h"

SceneGraph::SceneGraph()
    : flat_shader_index(-1), selected_node(-1) {
    add_simple_node("Scene Graph", -1);
}

SceneGraph::~SceneGraph() {
    for(Scene& scene : scenes) {
        scene.free();
    }
}

void SceneGraph::draw(const mat4& view_projection, const Frustum& frustum, unsigned int node_index) const {
    const Node& node = nodes[node_index];

    const Shader* shader = nullptr;
    switch(node.type) {
        case Node::Type::MESH:
        case Node::Type::FLAT_SHADED_MESH:
        case Node::Type::MODEL:
        case Node::Type::TERRAIN:
            shader = shaders[node.data[1].index];
            break;
        default: break;
    }

    if(shader != nullptr) {
        shader->use();

        const mat4& global_model = transforms[node_index].get_global_model_const_reference();
        shader->set_uniform_if_exists("u_model", global_model);

        int u_mvp_location = shader->get_uniform_location("u_mvp");
        if(u_mvp_location != -1) {
            Shader::set_uniform(u_mvp_location, view_projection * global_model);
        }

        int u_normals_model_matrix_location = shader->get_uniform_location("u_normals_model_matrix");
        if(u_normals_model_matrix_location != -1) {
            Shader::set_uniform(u_normals_model_matrix_location, transpose_inverse(global_model));
        }

        switch(node.type) {
            case Node::Type::MESH:
            case Node::Type::FLAT_SHADED_MESH:
                if(node.data[2].type == DataType::MATERIAL) {
                    materials[node.data[2].index]->update_shader_uniforms(*shader);
                }
                meshes[node.data[0].index]->draw();
                break;
            case Node::Type::MODEL:
                models[node.data[0].index]->draw(*shader);
                break;
            case Node::Type::TERRAIN:
                terrains[node.data[0].index].draw(view_projection);
                break;
            default: break;
        }
    }

    for(unsigned int index : node.children) { draw(view_projection, frustum, index); }
}

void SceneGraph::update_transform_and_children(unsigned int node_index) {
    if(transforms[node_index].is_local_model_dirty()) {
        force_update_transform_and_children(node_index);
    } else {
        for(unsigned int child : nodes[node_index].children) {
            update_transform_and_children(child);
        }
    }
}

void SceneGraph::force_update_transform_and_children(unsigned int node_index) {
    if(nodes[node_index].parent != -1) {
        transforms[node_index].update_global_model(transforms[nodes[node_index].parent].get_global_model());
    } else {
        transforms[node_index].update_global_model();
    }

    for(unsigned int child : nodes[node_index].children) {
        force_update_transform_and_children(child);
    }
}

Node& SceneGraph::operator[](unsigned int node_index) { return nodes[node_index]; }

unsigned int SceneGraph::add_simple_node(const std::string& name, unsigned int parent) {
    unsigned int index = nodes.size();
    if(parent != -1) { nodes[parent].children.push_back(index); }
    nodes.emplace_back(name, parent, index, Node::Type::SIMPLE);
    transforms.emplace_back();

    return nodes.size() - 1;
}

unsigned int SceneGraph::add_mesh_node(const std::string& name,
                                       unsigned int parent,
                                       const Mesh* mesh,
                                       const Shader* shader) {
    unsigned int index = nodes.size();
    nodes[parent].children.push_back(index);
    nodes.emplace_back(name, parent, index, Node::Type::MESH);
    transforms.emplace_back();

    meshes.push_back(mesh);
    nodes[index].add_data(DataType::MESH, meshes.size() - 1); // 0

    shaders.push_back(shader);
    nodes[index].add_data(DataType::SHADER, shaders.size() - 1); // 1

    return index;
}

unsigned int SceneGraph::add_flat_shaded_mesh_node(const std::string& name,
                                                   unsigned int parent,
                                                   const Mesh* mesh,
                                                   const vec4& color) {
    unsigned int index = nodes.size();
    nodes[parent].children.push_back(index);
    nodes.emplace_back(name, parent, index, Node::Type::FLAT_SHADED_MESH);
    transforms.emplace_back();

    meshes.push_back(mesh);
    nodes[index].add_data(DataType::MESH, meshes.size() - 1); // 0

    if(flat_shader_index == -1) {
        shaders.push_back(AssetManager::get_shader_ptr("flat"));
        flat_shader_index = shaders.size() - 1;
    }
    nodes[index].add_data(DataType::SHADER, flat_shader_index); // 1

    vector4s.push_back(color);
    nodes[index].add_data(DataType::VEC3, vector4s.size() - 1); // 2

    return index;
}

unsigned int SceneGraph::add_model_node(const std::string& name,
                                        unsigned int parent,
                                        const Model* model,
                                        const Shader* shader) {
    unsigned int index = nodes.size();
    nodes[parent].children.push_back(index);
    nodes.emplace_back(name, parent, index, Node::Type::MODEL);
    transforms.emplace_back();

    models.push_back(model);
    nodes[index].add_data(DataType::MODEL, models.size() - 1); // 0

    shaders.push_back(shader);
    nodes[index].add_data(DataType::SHADER, shaders.size() - 1); // 1

    return index;
}

unsigned int SceneGraph::add_scene_node(const std::string& name,
                                        unsigned int parent,
                                        const std::filesystem::path& path) {
    unsigned int index = nodes.size();
    nodes[parent].children.push_back(index);
    nodes.emplace_back(name, parent, index, Node::Type::SCENE);
    transforms.emplace_back();

    scenes.emplace_back(path, this, index);
    nodes[index].add_data(DataType::SCENE, scenes.size() - 1); // 0

    return index;
}

unsigned int SceneGraph::add_terrain_node(const std::string& name,
                                          unsigned int parent,
                                          float chunk_size,
                                          unsigned int chunks_on_line) {
    unsigned int index = nodes.size();
    nodes[parent].children.push_back(index);
    nodes.emplace_back(name, parent, index, Node::Type::TERRAIN);
    transforms.emplace_back();

    terrains.emplace_back(AssetManager::get_shader("terrain"), chunk_size, chunks_on_line);
    nodes[index].add_data(DataType::TERRAIN, terrains.size() - 1); // 0

    return index;
}

unsigned int SceneGraph::add_material(Material* material) {
    materials.push_back(material);
    return materials.size() - 1;
}

void SceneGraph::add_imgui_node_tree() {
    add_node_to_imgui_node_tree(0);
}

void SceneGraph::add_selected_entity_editor_to_imgui_window() {
    if(selected_node == -1) {
        ImGui::Text("No Entity is Selected");
    } else {
        Node& node = nodes[selected_node];
        Transform& transform = transforms[selected_node];

        ImGui::Text("Selected Entity: '%s'", node.name.c_str());

        if(ImGui::Button("UNSELECT")) { selected_node = -1; }

        if(ImGui::Checkbox("Is Object Visible", &node.is_visible)) { set_visibility(selected_node, node.is_visible); }

        bool is_dirty = ImGui::DragFloat3("Local Position", &transform.get_local_position_reference().x);

        quaternion& orientation = transform.get_local_orientation_reference();
        if(ImGui::DragFloat4("Local Orientation", &orientation.x, 0.1f)) {
            is_dirty = true;
            orientation.normalize();
        }

        is_dirty = is_dirty || ImGui::DragFloat3("Local Scale", &transform.get_local_scale_reference().x, 0.1f, 0.1f);

        if(is_dirty) { transform.set_local_model_to_dirty(); }
    }
}

void SceneGraph::set_visibility(unsigned int node_index, bool is_visible) {
    nodes[node_index].is_visible = is_visible;
    for(unsigned int index : nodes[node_index].children) {
        set_visibility(index, is_visible);
    }
}

void SceneGraph::set_is_selected(unsigned int node_index, bool is_selected) {
    nodes[node_index].is_selected = is_selected;
    for(unsigned int index : nodes[node_index].children) {
        set_visibility(index, is_selected);
    }
}

void SceneGraph::add_node_to_imgui_node_tree(unsigned int node_index) {
    const Node& node = nodes[node_index];

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if(node.children.empty()) { flags |= ImGuiTreeNodeFlags_Leaf; }
    if(selected_node == node_index) { flags |= ImGuiTreeNodeFlags_Selected; }

    std::string label;
    switch(node.type) {
        case Node::Type::SIMPLE:
            label += " o";
            break;
        case Node::Type::MODEL:
            label += "MO";
            break;
        case Node::Type::MESH:
            label += " M";
            break;
        case Node::Type::FLAT_SHADED_MESH:
            label += " F";
            break;
        case Node::Type::TERRAIN:
            label += " T";
            break;
        case Node::Type::SCENE:
            label += " S";
            break;
        default:
            label += '?';
            break;
    }
    label += ' ' + node.name;

    ImGui::PushID(&node);
    if(ImGui::TreeNodeEx(label.c_str(), flags)) {
        if(ImGui::IsItemClicked()) {
            if(selected_node != -1) { set_is_selected(selected_node, false); }
            selected_node = node_index;
            set_is_selected(selected_node, true);
        }

        for(unsigned int index : node.children) { add_node_to_imgui_node_tree(index); }
        ImGui::TreePop();
    } else if(ImGui::IsItemClicked()) {
        if(selected_node != -1) { set_is_selected(selected_node, false); }
        selected_node = node_index;
        set_is_selected(selected_node, true);
    }
    ImGui::PopID();
}
