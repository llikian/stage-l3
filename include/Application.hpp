/***************************************************************************************************
 * @file  Application.hpp
 * @brief Declaration of the Application class
 **************************************************************************************************/

#pragma once

#include "EventHandler.hpp"
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
     * @brief Executes the main loop of the application.
     */
    void run();

    /**
     * @brief
    *  @param window The window that received the event.
    *  @param key The keyboard key that was pressed or released.
    *  @param scancode The system-specific scancode of the key.
    *  @param action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
    *  @param mods Bit field describing which [modifier keys](@ref mods) were held down.
    */
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    EventHandler event_handler; ///< Event handler. Handles key events.
private:
    Window window; ///< Contains GLFW window.

};
