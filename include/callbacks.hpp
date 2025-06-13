/***************************************************************************************************
 * @file  callbacks.hpp
 * @brief Declarations of callback functions.
 **************************************************************************************************/

#pragma once

#include <GLFW/glfw3.h>

class Application;

/**
 * @brief Implementation for the GLFW window size callback. Uses the window user pointer to tell the
 * event handler to handle the event.
 * @param window The window that received the event.
 * @param width The window's new width.
 * @param height The window's new height.
 */
void window_size_callback(GLFWwindow* window, int width, int height);

/**
 * @brief Implementation for the GLFW framebuffer size callback. Uses the window user pointer to tell
 * the event handler to handle the event.
 * @param window The window that received the event.
 * @param width The framebuffer's new width.
 * @param height The framebuffer's new heigth.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
 * @brief Implementation for the GLFW key callback. Uses the window user pointer to tell
 * the event handler to handle the event.
 * @param window The window that received the event.
 * @param key The key that was pressed or released.
 * @param scancode The system-specific scancode of the key.
 * @param action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
 * @param mods Bit field describing which modifiers were held down.
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

/**
 * @brief Implementation for the GLFW cursor position callback. Uses the window user pointer to tell
 * the event handler to handle the event.
 * @param window The window that received the event.
 * @param position_x The cursor's new vertical position.
 * @param position_y The cursor's new horizontal position.
 */
void cursor_position_callback(GLFWwindow* window, double position_x, double position_y);