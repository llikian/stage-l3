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
    : flat_shader_index(INVALID_INDEX),
      are_AABBs_drawn(false),
      selected_node(INVALID_INDEX),
      last_nodes_count(0) {
    add_simple_node("Scene Graph", INVALID_INDEX);

    frustum_culling_comp.create({ "shaders/compute/frustum_culling.comp" }, "Frustum Culling");

    glGenBuffers(1, &AABBs_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, AABBs_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(AABB) * AABBs.size(), AABBs.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, AABBs_buffer);

    glGenBuffers(1, &result_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, result_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * is_in_frustum.size(), is_in_frustum.data(), GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, result_buffer);
}

SceneGraph::~SceneGraph() {
    for(Scene& scene : scenes) {
        scene.free();
    }
    glDeleteBuffers(1, &AABBs_buffer);
    glDeleteBuffers(1, &result_buffer);
}

Node& SceneGraph::operator[](unsigned int node_index) { return nodes[node_index]; }

void SceneGraph::draw(const Frustum& frustum) {
    total_drawn_objects = 0;

    update_transform_and_children();

    update_AABBs();

    frustum_culling_comp.use();
    frustum_culling_comp.set_uniform("u_view_projection", frustum.view_projection);

    if(last_nodes_count != nodes.size()) {
        // Reallocate buffers with the new sizes.
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, AABBs_buffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(AABB) * AABBs.size(), AABBs.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, AABBs_buffer);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, result_buffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * is_in_frustum.size(), is_in_frustum.data(),
                     GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, result_buffer);

        last_nodes_count = nodes.size();
    }

    glDispatchCompute((AABBs.size() + 63) / 64, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, result_buffer);
    int* results = static_cast<int*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
    for(unsigned int i = 0 ; i < is_in_frustum.size() ; ++i) { is_in_frustum[i] = results[i]; }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glUseProgram(0);

    draw(frustum, 0);
}

unsigned int SceneGraph::add_simple_node(const std::string& name, unsigned int parent) {
    return add_node(name, parent, Node::Type::SIMPLE);
}

unsigned int SceneGraph::add_mesh_node(const std::string& name,
                                       unsigned int parent,
                                       unsigned int mesh_index,
                                       unsigned int shader_index) {
    unsigned int index = add_node(name, parent, Node::Type::MESH);

    nodes[index].drawable_index = mesh_index;
    nodes[index].shader_index = shader_index;

    return index;
}

unsigned int SceneGraph::add_mesh_node(const std::string& name,
                                       unsigned int parent,
                                       const Mesh* mesh,
                                       const Shader* shader) {
    meshes.push_back(mesh);
    shaders.push_back(shader);

    return add_mesh_node(name, parent, meshes.size() - 1, shaders.size() - 1);
}

unsigned int SceneGraph::add_flat_shaded_mesh_node(const std::string& name,
                                                   unsigned int parent,
                                                   unsigned int mesh_index,
                                                   const vec4& color) {
    unsigned int index = add_node(name, parent, Node::Type::FLAT_SHADED_MESH);

    nodes[index].drawable_index = mesh_index;
    nodes[index].shader_index = flat_shader_index;

    colors.push_back(color);
    nodes[index].color_index = colors.size() - 1;

    return index;
}

unsigned int SceneGraph::add_flat_shaded_mesh_node(const std::string& name,
                                                   unsigned int parent,
                                                   const Mesh* mesh,
                                                   const vec4& color) {
    meshes.push_back(mesh);

    return add_flat_shaded_mesh_node(name, parent, meshes.size() - 1, color);
}

unsigned int SceneGraph::add_model_node(const std::string& name,
                                        unsigned int parent,
                                        const Model* model,
                                        const Shader* shader) {
    unsigned int index = add_node(name, parent, Node::Type::MODEL);

    models.push_back(model);
    nodes[index].drawable_index = models.size() - 1;

    shaders.push_back(shader);
    nodes[index].shader_index = shaders.size() - 1;

    return index;
}

unsigned int SceneGraph::add_scene_node(const std::string& name,
                                        unsigned int parent,
                                        const std::filesystem::path& path) {
    unsigned int index = add_node(name, parent, Node::Type::SCENE);

    scenes.emplace_back(path, this, index);
    nodes[index].scene_index = scenes.size() - 1;

    return index;
}

unsigned int SceneGraph::add_terrain_node(const std::string& name,
                                          unsigned int parent,
                                          float chunk_size,
                                          unsigned int chunks_on_line) {
    unsigned int index = add_node(name, parent, Node::Type::TERRAIN);

    terrains.emplace_back(AssetManager::get_shader("terrain"), chunk_size, chunks_on_line);
    nodes[index].drawable_index = terrains.size() - 1;

    return index;
}

unsigned int SceneGraph::add_mesh(const Mesh* mesh) {
    meshes.push_back(mesh);
    return meshes.size() - 1;
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
        Transform& transform = transforms[selected_node];

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

void SceneGraph::draw(const Frustum& frustum, unsigned int node_index) {
    const Node& node = nodes[node_index];

    if(!node.is_visible) { return; }

    // if(AABBs[node_index].is_in_frustum(frustum)) {
    if(is_in_frustum[node_index]) {
        if(node.drawable_index != INVALID_INDEX) {
            ++total_drawn_objects;
            const Shader* shader = shaders[node.shader_index];
            draw(frustum.view_projection, shader, node_index);
        }

        if(are_AABBs_drawn || node.is_selected) {
            const Shader* shader = shaders[flat_shader_index];
            shader->use();
            shader->set_uniform("u_mvp", frustum.view_projection * AABBs[node_index].get_global_model_matrix());

            if(node.is_selected) {
                if(node.parent == INVALID_INDEX || !nodes[node.parent].is_selected) {
                    shader->set_uniform("u_color", vec4(0.0f, 1.0f, 1.0f, 1.0f));
                } else {
                    shader->set_uniform("u_color", vec4(0.0f, 0.0f, 1.0f, 1.0f));
                }
            } else if(node.drawable_index == INVALID_INDEX) { // Not a drawable node.
                shader->set_uniform("u_color", vec4(0.0f, 1.0f, 0.0f, 1.0f));
            } else {
                shader->set_uniform("u_color", vec4(1.0f, 0.0f, 0.0f, 1.0f));
            }

            glLineWidth(3.0f);
            AssetManager::get_mesh("wireframe cube").draw();
            glLineWidth(1.0f);
        }

        for(unsigned int index : node.children) { draw(frustum, index); }
    }
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
    Node& node = nodes[node_index];
    if(nodes[node_index].parent != INVALID_INDEX) {
        transforms[node_index].update_global_model(transforms[node.parent].get_global_model());
    } else {
        transforms[node_index].update_global_model();
    }

    for(unsigned int child : nodes[node_index].children) {
        force_update_transform_and_children(child);
    }
}

void SceneGraph::update_AABBs(unsigned int node_index) {
    for(unsigned int index : nodes[node_index].children) { update_AABBs(index); }

    vec3 min(std::numeric_limits<float>::max());
    vec3 max(std::numeric_limits<float>::lowest());

    switch(nodes[node_index].type) {
        case Node::Type::MESH:
        case Node::Type::FLAT_SHADED_MESH:
            AABBs[node_index].set(meshes[nodes[node_index].drawable_index]->get_AABB(), transforms[node_index]);
            break;
        case Node::Type::MODEL:
            AABBs[node_index].set(models[nodes[node_index].drawable_index]->get_AABB(), transforms[node_index]);
            break;
        case Node::Type::SIMPLE:
        case Node::Type::SCENE:
        case Node::Type::TERRAIN:
            for(unsigned int index : nodes[node_index].children) {
                AABB::axis_aligned_min(min, AABBs[index].min_point);
                AABB::axis_aligned_max(max, AABBs[index].max_point);
            }
            AABBs[node_index].set(min, max);
            break;
    }
}

unsigned int SceneGraph::add_node(const std::string& name, unsigned int parent, Node::Type type) {
    nodes.emplace_back(name, parent, type);
    transforms.emplace_back();
    AABBs.emplace_back();
    is_in_frustum.push_back(false);

    unsigned int index = nodes.size() - 1;
    if(parent != INVALID_INDEX) { nodes[parent].children.push_back(index); }

    return nodes.size() - 1;
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
