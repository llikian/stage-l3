/***************************************************************************************************
 * @file  LifetimeLogger.hpp
 * @brief Declaration of the LifetimeLogger struct
 **************************************************************************************************/

#pragma once

#include <chrono>
#include <string>

/**
 * @struct LifetimeLogger
 * @brief Utility class used to log the time a scope takes to end. Outputs how much time has passed
 * between its construction and destruction when it falls out of scope.
 */
struct LifetimeLogger {
    /**
     * @brief Initializes
     * @param message What the logger should output before its lifetime when it will be destructed.
     * By default: "LifetimeLogger's lifepan was: "
     */
    explicit LifetimeLogger(const std::string& message);

    /**
     * @brief Outputs how much time the logger existed.
     */
    ~LifetimeLogger();

    /// The time at which the logger was constructed.
    const std::chrono::time_point<std::chrono::system_clock> startTime;
    /// What the logger should output before its lifetime when it will be destructed.
    const std::string message;
};