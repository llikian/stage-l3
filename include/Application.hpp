/***************************************************************************************************
 * @file  Application.hpp
 * @brief Declaration of the Application class
 **************************************************************************************************/

#pragma once

#include <unordered_map>
#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "mesh/LineMesh.hpp"
#include "mesh/Model.hpp"
#include "mesh/TriangleMesh.hpp"
#include "scene_graph/SceneGraph.hpp"

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

    /**
     * @return A reference to the Application's event handler.
     */
    EventHandler& get_event_handler();

private:
    /**
     * @brief Add a shader to the unordered_map.
     * @param name The name of the shader (key).
     * @param paths_list The paths to each of the different shaders (parameter to the value's constructor).
     */
    void add_shader(const std::string& name, const std::initializer_list<std::filesystem::path>& paths_list);

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

    Window window;              ///< Contains the GLFW window. Needs to be initialized first.
    EventHandler event_handler; ///< Event handler. Handles key events. Should be initialized last.

    SceneGraph scene_graph; ///< Scene graph.
    Camera camera;          ///< The camera.

    bool is_spying_enabled;          ///< Whether the spying window is drawn.
    Camera spy_camera;               ///< The camera used to test frustum culling.
    vec3 spy_camera_target;          ///< Where the spy camera is looking at.
    unsigned int FBO;                ///< Frame Buffer Object.
    unsigned int RBO;                ///< Rendering Buffer Object.
    unsigned int spy_window_texture; ///< The id of the texture the framebuffer will render on.
    LineMesh frustum_mesh;           ///< Mesh used to render the view frustum.

    std::unordered_map<std::string, Shader> shaders; ///< Array containing all the shaders used for rendering.

    TriangleMesh screen; ///< Mesh used to render a quad as the background.
    LineMesh axes;       ///< Mesh used to render axes in order to determine the view direction.
    bool are_axes_drawn; ///< Whether the axes are drawn.
};
