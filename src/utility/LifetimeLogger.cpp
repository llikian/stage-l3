/***************************************************************************************************
 * @file  LifetimeLogger.cpp
 * @brief Implementation of the LifetimeLogger struct
 **************************************************************************************************/

#include "utility/LifetimeLogger.hpp"

#include <iostream>

LifetimeLogger::LifetimeLogger(const std::string& message)
    : startTime(std::chrono::high_resolution_clock::now()),
      message(message.empty() ? "LifetimeLogger's lifepan was: " : message) { }

LifetimeLogger::~LifetimeLogger() {
    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
    std::cout << message << duration.count() << "s.\n";
}
