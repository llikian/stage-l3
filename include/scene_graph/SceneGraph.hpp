/***************************************************************************************************
 * @file  SceneGraph.hpp
 * @brief Declaration of the SceneGraph class
 **************************************************************************************************/

#pragma once

#include "scene_graph/Entity.hpp"

/**
 * @class SceneGraph
 * @brief A scene graph that holds the root of the graph and contains functionality for rendering it
 * with ImGui along with selecting an entity in the graph and modifying certain fields in an object
 * editor.
 */
class SceneGraph {
public:
    /**
     * @brief Default constructor.
     */
    SceneGraph();

    /**
     * @brief Add an entity to the ImGui node tree.
     * @param entity The entity to add to the tree.
     */
    void add_entity_to_imgui_node_tree(Entity* entity);

    /**
     * @brief Add an object editor for the selected entity (if one is selected) to the current ImGui
     * window.
     * @warning Need to be called within an ImGui window, between a call to ImGui::Begin(...) and
     * ImGui::End().
     */
    void add_selected_entity_editor_to_imgui_window() const;

    /**
     * @brief Draw every drawable object within the scene graph.
     * @param view_projection_matrix The projection matrix multiplied by the view matrix.
     */
    void draw(const mat4& view_projection_matrix);

    Entity root; ///< The root of the scene graph.

private:
    Entity* selected_entity; ///< The currently selected entity.
};
