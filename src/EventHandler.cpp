/***************************************************************************************************
 * @file  EventHandler.cpp
 * @brief Implementation of the EventHandler class
 **************************************************************************************************/

#include "EventHandler.hpp"

#include <GLFW/glfw3.h>

void EventHandler::handle_events() {
    glfwPollEvents();

    // Non Repeatable Keys
    while(!pressed_keys.empty()) {
        key_actions[pressed_keys.front()]();
        pressed_keys.pop();
    }

    // Repeatable Keys
    for(const auto& [key, is_active] : repeatable_keys) {
        if(is_active) {
            key_actions[key]();
        }
    }
}

void EventHandler::associate_action_to_key(int key, bool repeatable, Action action) {
    key_actions.emplace(key, action);
    if(repeatable) { repeatable_keys.emplace(key, false); }
}

void EventHandler::handle_key_press_event(int key) {
    if(repeatable_keys.contains(key)) {
        repeatable_keys[key] = true;
    } else {
        pressed_keys.push(key);
    }
}

void EventHandler::handle_key_release_event(int key) {
    if(repeatable_keys.contains(key)) {
        repeatable_keys[key] = false;
    }
}
