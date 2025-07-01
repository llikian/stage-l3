/***************************************************************************************************
 * @file  SceneGraph.cpp
 * @brief Implementation of the SceneGraph class
 **************************************************************************************************/

#include "SceneGraph.hpp"
#include "imgui.h"
#include "entities/DrawableEntity.hpp"

SceneGraph::SceneGraph() : root("Scene Graph"), selected_entity(nullptr) { }

void SceneGraph::add_entity_to_imgui_node_tree(Entity* entity) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
    if(entity->children.empty()) { flags |= ImGuiTreeNodeFlags_Leaf; }
    if(selected_entity == entity) { flags |= ImGuiTreeNodeFlags_Selected; }

    std::string label;
    switch(entity->get_type()) {
        case ENTITY_TYPE_DEFAULT:
            label += " o";
            break;
        case ENTITY_TYPE_MODEL:
            label += " M";
            break;
        case ENTITY_TYPE_TRIANGLE_MESH:
            label += "TM";
            break;
        case ENTITY_TYPE_LINE_MESH:
            label += " L";
            break;
        case ENTITY_TYPE_FLAT_SHADED_MESH:
            label += " F";
            break;
        case ENTITY_TYPE_TERRAIN:
            label += " T";
            break;
        default:
            label += '?';
            break;
    }
    label += ' ' + entity->name;

    if(ImGui::TreeNodeEx(label.c_str(), flags)) {
        if(ImGui::IsItemClicked()) { selected_entity = entity; }

        for(Entity* child : entity->children) { add_entity_to_imgui_node_tree(child); }
        ImGui::TreePop();
    }
}

void SceneGraph::add_selected_entity_editor_to_imgui_window() const {
    if(selected_entity == nullptr) {
        ImGui::Text("No Entity is Selected");
    } else {
        selected_entity->add_to_object_editor();
    }
}

void SceneGraph::draw(const mat4& view_projection_matrix, const Frustum& frustum) {
    DrawableEntity::total_drawable_entities = 0;
    DrawableEntity::total_not_hidden_entities = 0;
    DrawableEntity::total_drawn_entities = 0;

    root.draw_drawables(view_projection_matrix, frustum);
}
