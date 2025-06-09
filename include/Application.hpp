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

    Shader shader;
    Camera camera;

    float fov;
    mat4 projection;
    mat4 view_projection;

    EventHandler event_handler; ///< Event handler. Handles key events. Should be initialized last.
};
