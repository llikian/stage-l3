/***************************************************************************************************
 * @file  SceneGraph.hpp
 * @brief Declaration of the SceneGraph class
 **************************************************************************************************/

#pragma once

#include "scene_graph/Entity.hpp"

/**
 * @class SceneGraph
 * @brief
 */
class SceneGraph {
public:
    SceneGraph();

    void draw_imgui_window(const std::string& imgui_window_name);
    void add_entity_to_imgui_window(Entity* entity);

    void draw(const mat4& view_projection_matrix);

    Entity root;

private:
    Entity* selected_entity;
};
