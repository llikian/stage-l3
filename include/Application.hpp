/***************************************************************************************************
 * @file  Application.hpp
 * @brief Declaration of the Application class
 **************************************************************************************************/

#pragma once

#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "maths/mat4.hpp"

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
     * @param model The model matrix used in the next draw call.
     */
    void update_mvp(const mat4& model) const;

    Window window; ///< Contains the GLFW window. Needs to be initialized first.

    Shader shader; ///< The default shader used for draw calls.
    Camera camera; ///< The active camera.

    float fov; ///< The field of view angle.
    mat4 projection; ///< The projection matrix. Used every frame in the mvp matrix calculation.

    /// Holds the projection matrix multiplied by the view matrix in order to only do the calculation once per frame.
    mat4 view_projection;

    EventHandler event_handler; ///< Event handler. Handles key events. Should be initialized last.
};
