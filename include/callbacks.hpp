/***************************************************************************************************
 * @file  callbacks.hpp
 * @brief Declarations of callback functions.
 **************************************************************************************************/

#pragma once

#include <GLFW/glfw3.h>

class Application;

/**
 * @brief
 * @param window
 * @param width
 * @param height
 */
static void window_size_callback(GLFWwindow* window, int width, int height);

/**
 * @brief
 * @param window
 * @param width
 * @param height
 */
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
 * @brief Handles what happens when a key is pressed, released or held down.
 * @param window The window that received the event.
 * @param key The keyboard key that was pressed or released.
 * @param scancode The system-specific scancode of the key.
 * @param action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
 * @param mods Bit field describing which [modifier keys](@ref mods) were held down.
 */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

/**
 * @brief
 * @param window
 * @param xPos
 * @param yPos
 */
static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);