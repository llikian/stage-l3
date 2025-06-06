/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

Application::Application()
    : window("Projet Stage L3", this) {
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    /* ---- Key Actions ---- */
    event_handler.associate_action_to_key(GLFW_KEY_ESCAPE, false, [this] { glfwSetWindowShouldClose(window, true); });
}

void Application::run() {
    while(!glfwWindowShouldClose(window)) {
        event_handler.poll_and_handle_events();
    }
}
