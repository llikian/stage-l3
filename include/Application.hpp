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

    EventHandler event_handler; ///< Event handler. Handles key events.
private:
    void update_mvp(const mat4& model) const;

    Window window; ///< Contains GLFW window.

    Shader shader;

    mat4 projection;

    Camera camera;
};
