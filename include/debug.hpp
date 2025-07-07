/***************************************************************************************************
 * @file  debug.hpp
 * @brief Declaration of debugging macros used for configuring logging
 **************************************************************************************************/

#pragma once

#ifdef DEBUG

#define DEBUG_LOG_SHADER_LIFETIME
// #define DEBUG_LOG_TEXTURE_LIFETIME
// #define DEBUG_LOG_MATERIAL_LIFETIME

#define DEBUG_LOG_MODEL_READ_INFO
#define DEBUG_LOG_MATERIAL_LIBRARY_READ_INFO
#define DEBUG_LOG_GLTF_READ_INFO

// #define DEBUG_ENABLE_FRUSTUM_TESTS
// #define DEBUG_SHOW_BOUNDING_BOXES

#endif
