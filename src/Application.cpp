/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <glad/glad.h>
#include "callbacks.hpp"

Application::Application()
    : event_handler(&window), window("Projet Stage L3", this) {
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    /* ---- Event Actions ---- */
    event_handler.associate_action_to_key(GLFW_KEY_ESCAPE, false, [this] { glfwSetWindowShouldClose(window, true); });
    // event_handler.set_window_size_action([this](int width, int height) { window.update_size(width, height); });

    /* ---- Other ---- */
    glClearColor(0.1, 0.1f, 0.1f, 1.0f);
}

void Application::run() {
    while(!glfwWindowShouldClose(window)) {
        event_handler.poll_and_handle_events();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
    }
}
