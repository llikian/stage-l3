/***************************************************************************************************
 * @file  SceneGraph.cpp
 * @brief Implementation of the SceneGraph class
 **************************************************************************************************/

#include "core/SceneGraph.hpp"

#include "assets/AssetManager.hpp"
#include "core/Node.hpp"
#include "debug.hpp"
#include "imgui.h"

SceneGraph::SceneGraph()
    : flat_shader_index(INVALID_INDEX), are_AABBs_drawn(false), selected_node(INVALID_INDEX) {
    add_simple_node("Scene Graph", INVALID_INDEX);
}

SceneGraph::~SceneGraph() {
    for(Scene& scene : scenes) {
        scene.free();
    }
}

void SceneGraph::draw(const mat4& view_projection, const Frustum& frustum) {
    total_drawable_objects = 0;
    total_visible_drawables = 0;
    total_culled_objects = 0;

    update_transform_and_children();
    draw(view_projection, frustum, 0);
}

void SceneGraph::update_transform_and_children(unsigned int node_index) {
    if(nodes[node_index].transform.is_local_model_dirty()) {
        force_update_transform_and_children(node_index);
    } else {
        for(unsigned int child : nodes[node_index].children) {
            update_transform_and_children(child);
        }
    }
}

void SceneGraph::force_update_transform_and_children(unsigned int node_index) {
    Node& node = nodes[node_index];
    if(nodes[node_index].parent != INVALID_INDEX) {
        node.transform.update_global_model(nodes[node.parent].transform.get_global_model());
    } else {
        node.transform.update_global_model();
    }

    for(unsigned int child : nodes[node_index].children) {
        force_update_transform_and_children(child);
    }
}

Node& SceneGraph::operator[](unsigned int node_index) { return nodes[node_index]; }

unsigned int SceneGraph::add_simple_node(const std::string& name, unsigned int parent) {
    nodes.emplace_back(name, parent, Node::Type::SIMPLE);
    unsigned int index = nodes.size() - 1;
    if(parent != INVALID_INDEX) { nodes[parent].children.push_back(index); }

    return nodes.size() - 1;
}

unsigned int SceneGraph::add_mesh_node(const std::string& name,
                                       unsigned int parent,
                                       unsigned int mesh_index,
                                       unsigned int shader_index,
                                       unsigned int AABB_index) {
    Node& node = nodes.emplace_back(name, parent, Node::Type::MESH);
    unsigned int index = nodes.size() - 1;
    nodes[parent].children.push_back(index);

    node.drawable_index = mesh_index;
    node.shader_index = shader_index;
    node.AABB_index = AABB_index;

    return index;
}

unsigned int SceneGraph::add_mesh_node(const std::string& name,
                                       unsigned int parent,
                                       const Mesh* mesh,
                                       const Shader* shader) {
    meshes.push_back(mesh);
    shaders.push_back(shader);

    vec3 min(std::numeric_limits<float>::max());
    vec3 max(std::numeric_limits<float>::lowest());
    meshes.back()->get_min_max_axis_aligned_coordinates(min, max);
    AABBs.emplace_back(min, max);

    return add_mesh_node(name, parent, meshes.size() - 1, shaders.size() - 1, AABBs.size() - 1);
}

unsigned int SceneGraph::add_flat_shaded_mesh_node(const std::string& name,
                                                   unsigned int parent,
                                                   unsigned int mesh_index,
                                                   unsigned int AABB_index,
                                                   const vec4& color) {
    Node& node = nodes.emplace_back(name, parent, Node::Type::FLAT_SHADED_MESH);
    unsigned int index = nodes.size() - 1;
    nodes[parent].children.push_back(index);

    node.drawable_index = mesh_index;
    node.shader_index = flat_shader_index;
    node.AABB_index = AABB_index;

    colors.push_back(color);
    node.color_index = colors.size() - 1;

    return index;
}

unsigned int SceneGraph::add_flat_shaded_mesh_node(const std::string& name,
                                                   unsigned int parent,
                                                   const Mesh* mesh,
                                                   const vec4& color) {
    meshes.push_back(mesh);

    vec3 min(std::numeric_limits<float>::max());
    vec3 max(std::numeric_limits<float>::lowest());
    meshes.back()->get_min_max_axis_aligned_coordinates(min, max);
    AABBs.emplace_back(min, max);

    return add_flat_shaded_mesh_node(name, parent, meshes.size() - 1, AABBs.size() - 1, color);
}

unsigned int SceneGraph::add_model_node(const std::string& name,
                                        unsigned int parent,
                                        const Model* model,
                                        const Shader* shader) {
    Node& node = nodes.emplace_back(name, parent, Node::Type::MODEL);
    unsigned int index = nodes.size() - 1;
    nodes[parent].children.push_back(index);

    models.push_back(model);
    node.drawable_index = models.size() - 1;

    shaders.push_back(shader);
    node.shader_index = shaders.size() - 1;

    vec3 min(std::numeric_limits<float>::max());
    vec3 max(std::numeric_limits<float>::lowest());
    models.back()->get_min_max_axis_aligned_coordinates(min, max);
    AABBs.emplace_back(min, max);
    node.AABB_index = AABBs.size() - 1;

    return index;
}

unsigned int SceneGraph::add_scene_node(const std::string& name,
                                        unsigned int parent,
                                        const std::filesystem::path& path) {
    Node& node = nodes.emplace_back(name, parent, Node::Type::SCENE);
    unsigned int index = nodes.size() - 1;
    nodes[parent].children.push_back(index);

    scenes.emplace_back(path, this, index);
    node.scene_index = scenes.size() - 1;

    return index;
}

unsigned int SceneGraph::add_terrain_node(const std::string& name,
                                          unsigned int parent,
                                          float chunk_size,
                                          unsigned int chunks_on_line) {
    nodes.emplace_back(name, parent, Node::Type::TERRAIN);
    unsigned int index = nodes.size() - 1;
    nodes[parent].children.push_back(index);

    terrains.emplace_back(AssetManager::get_shader("terrain"), chunk_size, chunks_on_line);
    nodes[index].drawable_index = terrains.size() - 1;

    return index;
}

void SceneGraph::add_mesh_and_AABB(const Mesh* mesh, unsigned int& mesh_index, unsigned int& AABB_index) {
    meshes.push_back(mesh);
    mesh_index = meshes.size() - 1;

    vec3 min(std::numeric_limits<float>::max());
    vec3 max(std::numeric_limits<float>::lowest());
    meshes.back()->get_min_max_axis_aligned_coordinates(min, max);
    AABBs.emplace_back(min, max);
    AABB_index = AABBs.size() - 1;
}

unsigned int SceneGraph::add_shader(const Shader* shader) {
    shaders.push_back(shader);
    return shaders.size() - 1;
}

unsigned int SceneGraph::add_material(Material* material) {
    materials.push_back(material);
    return materials.size() - 1;
}

void SceneGraph::add_imgui_node_tree() {
    add_node_to_imgui_node_tree(0);
}

void SceneGraph::add_object_editor_to_imgui_window() {
    if(selected_node < nodes.size()) {
        Node& node = nodes[selected_node];
        Transform& transform = node.transform;

        ImGui::Text("Selected Node: '%s'", node.name.c_str());

        if(ImGui::Button("UNSELECT")) {
            set_is_selected(selected_node, false);
            selected_node = INVALID_INDEX;
        }

        if(ImGui::Checkbox("Is Object Visible", &node.is_visible)) { set_visibility(selected_node, node.is_visible); }

        bool is_dirty = ImGui::DragFloat3("Local Position", &transform.get_local_position_reference().x);

        quaternion& orientation = transform.get_local_orientation_reference();
        if(ImGui::DragFloat4("Local Orientation", &orientation.x, 0.1f)) {
            is_dirty = true;
            orientation.normalize();
        }

        is_dirty = is_dirty || ImGui::DragFloat3("Local Scale", &transform.get_local_scale_reference().x, 0.1f, 0.1f);

        if(is_dirty) { transform.set_local_model_to_dirty(); }

        ImGui::NewLine();
        if(node.shader_index != INVALID_INDEX) {
            ImGui::Text("Shader: '%s'", shaders[node.shader_index]->get_name().c_str());
        }
        if(node.color_index != INVALID_INDEX) { ImGui::ColorEdit4("Color", &colors[node.color_index].x); }
        if(node.material_index != INVALID_INDEX) { materials[node.material_index]->add_to_object_editor(); }
    } else {
        ImGui::Text("No Node is Selected");
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
        set_is_selected(index, is_selected);
    }
}

void SceneGraph::draw(const mat4& view_projection, const Frustum& frustum, unsigned int node_index) {
    const Node& node = nodes[node_index];

    if(node.drawable_index != INVALID_INDEX) {
        ++total_drawable_objects;

        if(node.is_visible) {
            ++total_visible_drawables;

            const AABB* aabb = node.AABB_index == INVALID_INDEX ? nullptr : &AABBs[node.AABB_index];
            const Shader* shader = node.shader_index == INVALID_INDEX ? nullptr : shaders[node.shader_index];

            if(aabb != nullptr) {
                if(aabb->is_in_frustum(frustum.view_projection * nodes[node_index].transform.get_global_model())) {
                    draw(view_projection, shader, node_index);
                } else {
                    ++total_culled_objects;
                }

                if(are_AABBs_drawn) {
                    shader = shaders[flat_shader_index];
                    shader->use();
                    shader->set_uniform("u_mvp", view_projection * aabb->get_global_model_matrix(node.transform));
                    shader->set_uniform("u_color", vec4(1.0f, 0.0f, 0.0f, 1.0f));
                    glLineWidth(3.0f);
                    AssetManager::get_mesh("wireframe cube").draw();
                    glLineWidth(1.0f);
                }
            } else {
                draw(view_projection, shader, node_index);
            }

            // TODO: Find a better way
            if(node.is_selected) {
                shader = shaders[flat_shader_index];
                shader->use();
                shader->set_uniform("u_color", vec4(1.0f, 0.0f, 0.0f, 0.25f));
                draw(view_projection, shader, node_index);
            }
        }
    }

    for(unsigned int index : node.children) { draw(view_projection, frustum, index); }
}

void SceneGraph::draw(const mat4& view_projection, const Shader* shader, unsigned int node_index) const {
    const Node& node = nodes[node_index];

    if(node.type == Node::Type::TERRAIN) {
        terrains[node.drawable_index].draw(view_projection);
        return;
    }

    if(shader == nullptr) {
        std::cout << "[WARNING] Node wasn't drawn as it didn't have any shader.\n";
        return;
    }

    shader->use();

    const mat4& global_model = node.transform.get_global_model_const_reference();
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
            if(node.material_index != INVALID_INDEX) {
                materials[node.material_index]->update_shader_uniforms(shader);
            }
            meshes[node.drawable_index]->draw();
            break;
        case Node::Type::FLAT_SHADED_MESH:
            shader->set_uniform("u_color", colors[node.color_index]);
            meshes[node.drawable_index]->draw();
            break;
        case Node::Type::MODEL:
            models[node.drawable_index]->draw(*shader);
            break;
        default: break;
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
            label += " ?";
            break;
    }
    label += ' ' + node.name;

    ImGui::PushID(&node);
    if(ImGui::TreeNodeEx(label.c_str(), flags)) {
        if(ImGui::IsItemClicked()) {
            if(selected_node != INVALID_INDEX) { set_is_selected(selected_node, false); }
            selected_node = node_index;
            set_is_selected(selected_node, true);
        }

        for(unsigned int index : node.children) { add_node_to_imgui_node_tree(index); }
        ImGui::TreePop();
    } else if(ImGui::IsItemClicked()) {
        if(selected_node != INVALID_INDEX) { set_is_selected(selected_node, false); }
        selected_node = node_index;
        set_is_selected(selected_node, true);
    }
    ImGui::PopID();
}
