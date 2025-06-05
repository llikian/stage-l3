/***************************************************************************************************
 * @file  main.cpp
 * @brief Contains the main program of the project
 **************************************************************************************************/

#include <iostream>
#include <stdexcept>
#include "Window.hpp"

int main() {
    try {
        Window window("Test", nullptr);

        glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int, int, int) {
            if(key == GLFW_KEY_ESCAPE) {
                glfwSetWindowShouldClose(win, true);
            }
        });

        while(!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    } catch(const std::exception& exception) {
        std::cerr << "ERROR : " << exception.what() << '\n';
        return -1;
    }

    return 0;
}
