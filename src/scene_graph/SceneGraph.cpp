/***************************************************************************************************
 * @file  SceneGraph.cpp
 * @brief Implementation of the SceneGraph class
 **************************************************************************************************/

#include "scene_graph/SceneGraph.hpp"

#include "imgui.h"

SceneGraph::SceneGraph() : root("Root"), selected_entity(nullptr) { }

void SceneGraph::draw_imgui_window(const std::string& imgui_window_name) {
    ImGui::Begin(imgui_window_name.c_str());

    ImGui::Text("Scene Graph");
    for(Entity* child : root.children) { add_entity_to_imgui_window(child); }

    ImGui::End();
}

void SceneGraph::add_entity_to_imgui_window(Entity* entity) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
    if(entity->children.empty()) { flags |= ImGuiTreeNodeFlags_Leaf; }
    if(selected_entity == entity) { flags |= ImGuiTreeNodeFlags_Selected; }

    if(ImGui::TreeNodeEx(entity->name.c_str(), flags)) {
        if(ImGui::IsItemClicked()) { selected_entity = entity; }

        for(Entity* child : entity->children) { add_entity_to_imgui_window(child); }
        ImGui::TreePop();
    }
}

void SceneGraph::draw(const mat4& view_projection_matrix) {
    root.draw_drawables(view_projection_matrix);
}
