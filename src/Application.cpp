/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

Application::Application()
    : window("Projet Stage L3", this) {
    glfwSetKeyCallback(window, key_callback);

    /* ---- Key Actions ---- */
    event_handler.associate_action_to_key(GLFW_KEY_ESCAPE, false, [this] { glfwSetWindowShouldClose(window, true); });
}

void Application::run() {
    while(!glfwWindowShouldClose(window)) {
        event_handler.handle_events();
    }
}

void Application::key_callback(GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */) {
    auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if(action == GLFW_PRESS) {
        application->event_handler.handle_key_press_event(key);
    } else if(action == GLFW_RELEASE) {
        application->event_handler.handle_key_release_event(key);
    }
}
