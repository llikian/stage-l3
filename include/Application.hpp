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

    EventHandler event_handler; ///< Event handler. Handles key events.
private:
    Window window; ///< Contains GLFW window.
};
