/***************************************************************************************************
 * @file  Window.hpp
 * @brief Declaration of the Window class
 **************************************************************************************************/

#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

#include "maths/vec2.hpp"

/**
 * @class Window
 * @brief Acts as a layer above the GLFW window.
 */
class Window {
public:
    /**
     * @brief Initializes GLFW, GLAD and OpenGL and creates the GLFW window.
     * @param window_name The name of the GLFW window.
     * @param user_pointer A pointer to an object that needs to be accessible in callbacks.
     */
    Window(const std::string& window_name, void* user_pointer);

    /**
     * @brief Destroys the GLFW window and terminates GLFW.
     */
    ~Window();

    /**
     * @return The GLFW window pointer.
     */
    GLFWwindow* get() const;

    /**
     * @return The width of the window.
     */
    int get_width() const;

    /**
     * @return The height of the window.
     */
    int get_height() const;

    /**
     * @brief Returns the window's size ratio.
     * @return The window's width divided by its height.
     */
    float get_size_ratio() const;

    /**
     * @brief Updates the size of the window.
     * @param width The window's current width.
     * @param height The window's current height.
     */
    void update_size(int width, int height);

    /**
     * @return A vec2 with the width of the window as the x component and its height as the y component.
     */
    vec2 get_resolution() const;

    /**
     * @return Whether the window should close.
     */
    bool should_close() const;

    /**
     * @brief Swaps the front and back buffers of the window.
     */
    void swap_buffers() const;

private:
    GLFWwindow* window; ///< The GLFW window pointer.
    int width;          ///< The width of the window.
    int height;         ///< The height of the window.
};
