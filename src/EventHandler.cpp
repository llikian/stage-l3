/***************************************************************************************************
 * @file  EventHandler.cpp
 * @brief Implementation of the EventHandler class
 **************************************************************************************************/

#include "EventHandler.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

EventHandler::EventHandler(Window* window, Camera* camera)
    : mouse_position(window->get_width() / 2.0f, window->get_height() / 2.0f),
      time(glfwGetTime()), delta(0.0f),
      window(window), active_camera(camera),
      is_cursor_visible(false) {
    if(camera == nullptr) {
        throw std::runtime_error("Cannot set active_camera to nullptr");
    }

    associate_action_to_key(GLFW_KEY_ESCAPE, false, [this] { glfwSetWindowShouldClose(*this->window, true); });
    associate_action_to_key(GLFW_KEY_TAB, false, [this] {
        glfwSetInputMode(*this->window, GLFW_CURSOR, is_cursor_visible ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        is_cursor_visible = !is_cursor_visible;
    });

    /* ---- Camera ---- */
    associate_action_to_key(GLFW_KEY_W, true, [this] {
        active_camera->move_around(MovementDirection::FORWARD, delta);
    });
    associate_action_to_key(GLFW_KEY_A, true, [this] {
        active_camera->move_around(MovementDirection::LEFT, delta);
    });
    associate_action_to_key(GLFW_KEY_S, true, [this] {
        active_camera->move_around(MovementDirection::BACKWARD, delta);
    });
    associate_action_to_key(GLFW_KEY_D, true, [this] {
        active_camera->move_around(MovementDirection::RIGHT, delta);
    });
    associate_action_to_key(GLFW_KEY_SPACE, true, [this] {
        active_camera->move_around(MovementDirection::UPWARD, delta);
    });
    associate_action_to_key(GLFW_KEY_LEFT_SHIFT, true, [this] {
        active_camera->move_around(MovementDirection::DOWNWARD, delta);
    });
}

void EventHandler::poll_and_handle_events() {
    glfwPollEvents();

    float temp_time = glfwGetTime();
    delta = temp_time - time;
    time = temp_time;

    // Non Repeatable Keys
    while(!pressed_keys.empty()) {
        int key = pressed_keys.front();
        pressed_keys.pop();

        if(key_actions.contains(key)) {
            key_actions[key]();
        }
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

void EventHandler::set_active_camera(Camera* camera) {
    if(camera == nullptr) {
        std::cerr << "WARNING : Cannot change active_camera to nullptr.\n";
    } else {
        active_camera = camera;
    }
}

void EventHandler::set_window_size_event_action(const Action& action) {
    window_size_event_action = action;
}

void EventHandler::handle_window_size_event(int width, int height) {
    window->update_size(width, height);
    if(window_size_event_action) {
        window_size_event_action();
    }
}

void EventHandler::handle_framebuffer_size_event(int width, int height) {
    glViewport(0, 0, width, height);
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

void EventHandler::handle_cursor_position_event(int position_x, int position_y) {
    if(!is_cursor_visible) {
        active_camera->look_around(position_y - mouse_position.y, position_x - mouse_position.x);
    }

    mouse_position.x = position_x;
    mouse_position.y = position_y;
}

float EventHandler::get_time() const {
    return time;
}

float EventHandler::get_delta() const {
    return delta;
}
