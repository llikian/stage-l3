/***************************************************************************************************
 * @file  callbacks.cpp
 * @brief Implementations of callback functions.
 **************************************************************************************************/

#include "callbacks.hpp"

#include "EventHandler.hpp"

void window_size_callback(GLFWwindow* /* window */, int width, int height) {
    EventHandler::handle_window_size_event(width, height);
}

void framebuffer_size_callback(GLFWwindow* /* window */, int width, int height) {
    EventHandler::handle_framebuffer_size_event(width, height);
}

void key_callback(GLFWwindow* /* window */, int key, int /* scancode */, int action, int /* mods */) {
    if(action == GLFW_PRESS) {
        EventHandler::handle_key_press_event(key);
    } else if(action == GLFW_RELEASE) {
        EventHandler::handle_key_release_event(key);
    }
}

void cursor_position_callback(GLFWwindow* /* window */, double position_x, double position_y) {
    EventHandler::handle_cursor_position_event(position_x, position_y);
}
