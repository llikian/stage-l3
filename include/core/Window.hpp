/***************************************************************************************************
 * @file  Window.hpp
 * @brief Declaration of the Window class
 **************************************************************************************************/

#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "maths/vec2.hpp"

/**
 * @class Window
 * @brief Acts as a layer above the GLFW window.
 */
class Window {
public:
    Window(const Window&) = delete; ///< Delete copy constructor.
    Window& operator=(const Window&) = delete; ///< Deleted copy operator.

    /**
     * @brief Access the Window singleton.
     * @return A reference to the Window singleton.
     */
    static inline Window& get() {
        static Window window;
        return window;
    }

    /**
     * @return The GLFW window pointer.
     */
    static GLFWwindow* get_glfw();

    /**
     * @return The width of the window.
     */
    static int get_width();

    /**
     * @return The height of the window.
     */
    static int get_height();

    /**
     * @brief Returns the window's aspect ratio.
     * @return The window's width divided by its height.
     */
    static float get_aspect_ratio();

    /**
     * @brief Updates the size of the window.
     * @param width The window's current width.
     * @param height The window's current height.
     */
    static void update_size(int width, int height);

    /**
     * @return A vec2 with the width of the window as the x component and its height as the y component.
     */
    static vec2 get_resolution();

    /**
     * @return Whether the window should close.
     */
    static bool should_close();

    /**
     * @brief Swaps the front and back buffers of the window.
     */
    static void swap_buffers();

private:
    /**
     * @brief Initializes GLFW, GLAD and OpenGL and creates the GLFW window.
     */
    Window();

    /**
     * @brief Destroys the GLFW window and terminates GLFW.
     */
    ~Window();

    GLFWwindow* window; ///< The GLFW window pointer.
    int width;          ///< The width of the window.
    int height;         ///< The height of the window.
};
