/***************************************************************************************************
 * @file  EventHandler.hpp
 * @brief Declaration of the EventHandler class
 **************************************************************************************************/

#pragma once

#include <functional>
#include <queue>
#include <unordered_map>

using Action = std::function<void()>;

/**
 * @class EventHandler
 * @brief
 */
class EventHandler {
public:
    void handle_events();

    void associate_action_to_key(int key, bool repeatable, Action action);

    void handle_key_press_event(int key);
    void handle_key_release_event(int key);

private:
    std::unordered_map<int, Action> key_actions;   ///< Stores the action associated with each key.
    std::queue<int> pressed_keys;                  ///< Queue of presses of non repeatable keys.
    std::unordered_map<int, bool> repeatable_keys; ///< Stores repeatable keys and whether they are active.
};
