/***************************************************************************************************
 * @file  EventHandler.cpp
 * @brief Implementation of the EventHandler class
 **************************************************************************************************/

#include "EventHandler.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "callbacks.hpp"

void EventHandler::poll_and_handle_events() {
    glfwPollEvents();

    EventHandler& event_handler = get();

    float temp_time = glfwGetTime();
    event_handler.delta = temp_time - event_handler.time;
    event_handler.time = temp_time;

    // Non Repeatable Keys
    while(!event_handler.pressed_keys.empty()) {
        int key = event_handler.pressed_keys.front();
        event_handler.pressed_keys.pop();

        std::unordered_map<int, Action>::iterator action_iterator = event_handler.key_actions.find(key);
        if(action_iterator != event_handler.key_actions.end()) { action_iterator->second(); }
    }

    // Repeatable Keys
    for(const auto& [key, is_active] : event_handler.repeatable_keys) {
        if(is_active) {
            event_handler.key_actions[key]();
        }
    }
}

void EventHandler::associate_action_to_key(int key, bool repeatable, Action action) {
    key_actions.emplace(key, action);
    if(repeatable) { repeatable_keys.emplace(key, false); }
}

void EventHandler::set_active_camera(Camera* camera) {
    if(camera == nullptr) {
        std::cerr << "WARNING : Shouldn't change active camera to nullptr.\n";
    } else {
        get().active_camera = camera;
    }
}

void EventHandler::handle_window_size_event(int width, int height) {
    Window::update_size(width, height);
    get().active_camera->update_projection_matrix();
}

void EventHandler::handle_framebuffer_size_event(int width, int height) {
    glViewport(0, 0, width, height);
}

void EventHandler::handle_key_press_event(int key) {
    EventHandler& event_handler = get();

    std::unordered_map<int, bool>::iterator repeatable_key_iterator = event_handler.repeatable_keys.find(key);
    if(repeatable_key_iterator != event_handler.repeatable_keys.end()) {
        repeatable_key_iterator->second = true;
    } else {
        event_handler.pressed_keys.push(key);
    }
}

void EventHandler::handle_key_release_event(int key) {
    EventHandler& event_handler = get();

    std::unordered_map<int, bool>::iterator repeatable_key_iterator = event_handler.repeatable_keys.find(key);
    if(repeatable_key_iterator != event_handler.repeatable_keys.end()) { repeatable_key_iterator->second = false; }
}

void EventHandler::handle_cursor_position_event(int position_x, int position_y) {
    EventHandler& event_handler = get();

    if(!event_handler.b_is_cursor_visible) {
        event_handler.active_camera->look_around(position_y - event_handler.mouse_position.y,
                                                 position_x - event_handler.mouse_position.x);
    }

    event_handler.mouse_position.x = position_x;
    event_handler.mouse_position.y = position_y;
}

float EventHandler::get_time() {
    return get().time;
}

float EventHandler::get_delta() {
    return get().delta;
}

bool EventHandler::is_cursor_visible() {
    return get().b_is_cursor_visible;
}

bool EventHandler::is_wireframe_enabled() {
    return get().b_is_wireframe_enabled;
}

bool EventHandler::is_face_culling_enabled() {
    return get().b_is_face_culling_enabled;
}

EventHandler::EventHandler()
    : time(glfwGetTime()), delta(0.0f),
      active_camera(nullptr),
      b_is_cursor_visible(glfwGetInputMode(Window::get_glfw(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL),
      b_is_face_culling_enabled(true), b_is_wireframe_enabled(false) {
    GLFWwindow* win = Window::get_glfw();
    glfwSetWindowSizeCallback(win, window_size_callback);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    glfwSetKeyCallback(win, key_callback);
    glfwSetCursorPosCallback(win, cursor_position_callback);

    /* ---- Key Actions ---- */
    /* General */
    associate_action_to_key(GLFW_KEY_ESCAPE, false, [this] { glfwSetWindowShouldClose(Window::get_glfw(), true); });

    /* Toggles */
    associate_action_to_key(GLFW_KEY_TAB, false, [this] {
        int mode = b_is_cursor_visible ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
        glfwSetInputMode(Window::get_glfw(), GLFW_CURSOR, mode);
        b_is_cursor_visible = !b_is_cursor_visible;
    });
    associate_action_to_key(GLFW_KEY_F, false, [this] {
        (b_is_face_culling_enabled ? glDisable : glEnable)(GL_CULL_FACE);
        b_is_face_culling_enabled = !b_is_face_culling_enabled;
    });
    associate_action_to_key(GLFW_KEY_Z, false, [this] {
        glPolygonMode(GL_FRONT_AND_BACK, b_is_wireframe_enabled ? GL_FILL : GL_LINE);
        b_is_wireframe_enabled = !b_is_wireframe_enabled;
    });

    /* Camera */
    associate_action_to_key(GLFW_KEY_W, true, [this] { active_camera->move_around(MovementDirection::FORWARD); });
    associate_action_to_key(GLFW_KEY_A, true, [this] { active_camera->move_around(MovementDirection::LEFT); });
    associate_action_to_key(GLFW_KEY_S, true, [this] { active_camera->move_around(MovementDirection::BACKWARD); });
    associate_action_to_key(GLFW_KEY_D, true, [this] { active_camera->move_around(MovementDirection::RIGHT); });
    associate_action_to_key(GLFW_KEY_SPACE, true, [this] { active_camera->move_around(MovementDirection::UPWARD); });
    associate_action_to_key(GLFW_KEY_C, true, [this] { active_camera->move_around(MovementDirection::DOWNWARD); });
}
