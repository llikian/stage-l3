/***************************************************************************************************
 * @file  Node.cpp
 * @brief Implementation of the Node struct
 **************************************************************************************************/

#include "core/Node.hpp"

Node::Node(const std::string& name, int parent, Type type)
    : type(type),
      name(name),
      parent(parent),
      drawable_index(INVALID_INDEX),
      shader_index(INVALID_INDEX),
      color_index(INVALID_INDEX),
      material_index(INVALID_INDEX),
      scene_index(INVALID_INDEX),
      is_visible(true),
      is_selected(false) { }
