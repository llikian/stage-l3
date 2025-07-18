/***************************************************************************************************
 * @file  Application.hpp
 * @brief Declaration of the Application class
 **************************************************************************************************/

#pragma once

#include "Camera.hpp"
#include "Cubemap.hpp"
#include "culling/Frustum.hpp"
#include "Framebuffer.hpp"
#include "mesh/MRMaterial.hpp"
#include "SceneGraph.hpp"
#include "Shader.hpp"

/**
 * @class Application
 * @brief Core of the project. Assembles everything together and handles the main loop.
 */
class Application {
public:
    /**
     * @brief Constructor.
     */
    Application();

    /**
     * @brief Frees all resources.
     */
    ~Application();

    /**
     * @brief Executes the main loop of the application.
     */
    void run();

private:
    /**
     * @brief Draws the framebuffer's texture on the screen and applies post processing shader.
     */
    void draw_post_processing() const;

    /**
     * @brief Draws the background.
     */
    void draw_background() const;

    /**
     * @brief Draws the imgui debug window.
     */
    void draw_imgui_debug_window();

    /**
     * @brief Draw the imgui object editor window.
     */
    void draw_imgui_object_ediot_window() const;

    SceneGraph scene_graph;  ///< Scene graph.
    Camera camera;           ///< The camera.
    Framebuffer framebuffer; ///< The framebuffer used to render.

    Cubemap cubemap;

    Frustum frustum; ///< The frustum used for culling.

    bool are_axes_drawn; ///< Whether the axes are drawn.

    float light_intensity;

    bool uniform_test_conditions[3];
};
