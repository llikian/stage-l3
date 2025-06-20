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
#include "maths/mat4.hpp"
#include "mesh/Model.hpp"
#include "mesh/TriangleMesh.hpp"

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
     * @brief Updates the uniforms 'mvp' and 'normals_model_matrix' in the shader.
     * @param shader The shader whose uniforms need to be updated.
     * @param model_matrix The model matrix used in the next draw call.
     */
    void update_mvp(const Shader& shader, const mat4& model_matrix) const;

    /**
     * @brief Updates the uniforms 'mvp' and 'normals_model_matrix' in the shader and draws the model.
     * @param shader The shader whose uniforms need to be updated.
     * @param model The model to draw.
     */
    void update_mvp_and_draw(const Shader& shader, Model& model) const;

    /**
     * @brief Draws the background.
     */
    void draw_background();

    Window window; ///< Contains the GLFW window. Needs to be initialized first.

    std::unordered_map<std::string, Shader> shaders; ///< Array containing all the shaders used for rendering.
    Camera camera;                                   ///< The active camera.

    float fov;       ///< The field of view angle.
    mat4 projection; ///< The projection matrix. Used every frame in the mvp matrix calculation.

    /// Holds the projection matrix multiplied by the view matrix in order to only do the calculation once per frame.
    mat4 view_projection;

    EventHandler event_handler; ///< Event handler. Handles key events. Should be initialized last.

    TriangleMesh screen; ///< Mesh used to render a quad as the background.
};
