/***************************************************************************************************
 * @file  main.cpp
 * @brief Contains the main program of the project
 **************************************************************************************************/

#include <iostream>
#include <stdexcept>
#include "Application.hpp"
#include "AssetManager.hpp"
#include "EventHandler.hpp"
#include "Window.hpp"

int main() {
    try {
        /* Making Sure Singletons are Initialized First */
        Window::get();
        EventHandler::get();
        AssetManager::get();

        /* Running Application */
        Application app;
        app.run();
    } catch(const std::exception& exception) {
        std::cerr << "ERROR : " << exception.what() << '\n';
        return -1;
    }

    return 0;
}
