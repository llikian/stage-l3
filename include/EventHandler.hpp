/***************************************************************************************************
 * @file  EventHandler.hpp
 * @brief Declaration of the EventHandler class
 **************************************************************************************************/

#pragma once

#include <functional>
#include <queue>
#include <unordered_map>
#include "Camera.hpp"
#include "Window.hpp"
#include "maths/vec2.hpp"

using Action = std::function<void()>;

/**
 * @class EventHandler
 * @brief
 */
class EventHandler {
public:
    explicit EventHandler(Window* window);

    void poll_and_handle_events();

    void associate_action_to_key(int key, bool repeatable, Action action);
    void set_active_camera(Camera* camera);

    void handle_window_size_event(int width, int height);
    void handle_framebuffer_size_event(int width, int height);
    void handle_key_press_event(int key);
    void handle_key_release_event(int key);
    void handle_cursor_position_event(int position_x, int position_y);

    float get_time() const;
    float get_delta() const;

private:
    std::unordered_map<int, Action> key_actions;   ///< Stores the action associated with each key.
    std::queue<int> pressed_keys;                  ///< Queue of presses of non repeatable keys.
    std::unordered_map<int, bool> repeatable_keys; ///< Stores repeatable keys and whether they are active.

    vec2 mouse_position;

    float time;
    float delta;

    Window* window;
    Camera* active_camera;

    bool is_cursor_visible;
};
