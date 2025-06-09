/***************************************************************************************************
 * @file  callbacks.cpp
 * @brief Implementations of callback functions.
 **************************************************************************************************/

#include "callbacks.hpp"

#include "Application.hpp"

void window_size_callback(GLFWwindow* window, int width, int height) {
    auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    application->get_event_handler().handle_window_size_event(width, height);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    application->get_event_handler().handle_framebuffer_size_event(width, height);
}

void key_callback(GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */) {
    auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if(action == GLFW_PRESS) {
        application->get_event_handler().handle_key_press_event(key);
    } else if(action == GLFW_RELEASE) {
        application->get_event_handler().handle_key_release_event(key);
    }
}

void cursor_position_callback(GLFWwindow* window, double position_x, double position_y) {
    auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    application->get_event_handler().handle_cursor_position_event(position_x, position_y);
}
