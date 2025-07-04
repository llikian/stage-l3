/***************************************************************************************************
 * @file  Application.hpp
 * @brief Declaration of the Application class
 **************************************************************************************************/

#pragma once

#include <unordered_map>
#include "Camera.hpp"
#include "culling/Frustum.hpp"
#include "EventHandler.hpp"
#include "mesh/LineMesh.hpp"
#include "mesh/Model.hpp"
#include "mesh/TriangleMesh.hpp"
#include "SceneGraph.hpp"
#include "Shader.hpp"
#include "Window.hpp"

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
     * @brief Draws the background.
     */
    void draw_background();

    /**
     * @brief Renders the scene from the spy window.
     */
    void draw_spy_window();

    /**
     * @brief Draws the imgui debug window.
     */
    void draw_imgui_debug_window();

    /**
     * @brief Draw the imgui object editor window.
     */
    void draw_imgui_object_ediot_window() const;

    SceneGraph scene_graph; ///< Scene graph.
    Camera camera;          ///< The camera.

    /* Frustum Culling */
    Frustum frustum;                 ///< The frustum used for culling.
    bool is_spying_enabled;          ///< Whether the spying window is drawn.
    vec3 spy_camera_position;        ///< The spy camera's position.
    vec3 spy_camera_target;          ///< Where the spy camera is looking at.
    Camera spy_camera;               ///< The camera used to test frustum culling.
    unsigned int FBO;                ///< Frame Buffer Object.
    unsigned int RBO;                ///< Rendering Buffer Object.
    unsigned int spy_window_texture; ///< The id of the texture the framebuffer will render on.

    bool are_axes_drawn; ///< Whether the axes are drawn.
};
