/***************************************************************************************************
 * @file  EventHandler.hpp
 * @brief Declaration of the EventHandler class
 **************************************************************************************************/

#pragma once

#include <functional>
#include <queue>
#include <unordered_map>
#include "Camera.hpp"
#include "maths/vec2.hpp"
#include "Window.hpp"

using Action = std::function<void()>;

/**
 * @class EventHandler
 * @brief Handles all events.
 */
class EventHandler {
public:
    /**
     * @brief Creates an event handler.
     * @param window The window.
     * @param camera The active camera.
     */
    EventHandler(Window& window, Camera* camera);

    /**
     * @brief Polls GLFW events, handles all of them and updates all event related variables.
     */
    void poll_and_handle_events();

    /**
     * @brief Associates an action to the press of a key.
     * @param key The key to press in order to perform the action.
     * @param repeatable Whether the action is repeated when holding down the key.
     * @param action What happens when the key is pressed.
     */
    void associate_action_to_key(int key, bool repeatable, Action action);

    /**
     * @brief Changes the active camera.
     * @param camera The new active camera.
     */
    void set_active_camera(Camera* camera);

    /**
     * @brief Handles what happens when the window is resized.
     * @param width The window's new width.
     * @param height The window's new height.
     */
    void handle_window_size_event(int width, int height);

    /**
     * @brief Handles what happens when the frambuffer is resized.
     * @param width The framebuffer's new width.
     * @param height The framebuffer's new height.
     */
    void handle_framebuffer_size_event(int width, int height);

    /**
     * @brief Handles what happens when a key is pressed.
     * @param key The pressed key.
     */
    void handle_key_press_event(int key);

    /**
     * @brief Handles what happens when a key is released.
     * @param key The pressed key.
     */
    void handle_key_release_event(int key);

    /**
     * @brief Handles what happens when the mouse is moved.
     * @param position_x The mouse's new horizontal position.
     * @param position_y The mouse's new vertical position.
     */
    void handle_cursor_position_event(int position_x, int position_y);

    /**
     * @return How much time elapsed since the beginning of the program.
     */
    float get_time() const;

    /**
     * @return  How much time passed since the last frame.
     */
    float get_delta() const;

    /**
     * @return Whether wireframe mode is enabled.
     */
    bool is_wireframe_on() const;

private:
    std::unordered_map<int, Action> key_actions;   ///< Stores the action associated with each key.
    std::queue<int> pressed_keys;                  ///< Queue of presses of non repeatable keys.
    std::unordered_map<int, bool> repeatable_keys; ///< Stores repeatable keys and whether they are active.

    vec2 mouse_position; ///< The position of the cursor of the mouse on the window.

    float time;  ///< How much time elapsed since the beginning of the program.
    float delta; ///< How much time passed since the last frame.

    Window& window;        ///< A pointer to the window.
    Camera* active_camera; ///< A pointer to the active camera.

    bool is_cursor_visible;       ///< Whether the mouse's cursor is visible.
    bool is_face_culling_enabled; ///< Whether face culling is enabled.
    bool is_wireframe_enabled;    ///< Whether wireframe mode is enabled.
};
