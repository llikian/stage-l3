/***************************************************************************************************
 * @file  Window.hpp
 * @brief Declaration of the Window class
 **************************************************************************************************/

#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
     * @brief Conversion operator to allow to use the Window as if it were the underlying GLFWwindow.
     */
    operator GLFWwindow*() const;

    /**
     * @return The GLFW window pointer.
     */
    GLFWwindow* get_GLFW_window() const;

    /**
     * @return The width of the window.
     */
    inline int get_width() const;

    /**
     * @return The height of the window.
     */
    inline int get_height() const;

    /**
     * @brief Returns the window's size ratio.
     * @return The window's width divided by its height.
     */
    inline float get_size_ratio() const;

private:
    GLFWwindow* window; ///< The GLFW window pointer.
    int width;          ///< The width of the window.
    int height;         ///< The height of the window.
};
