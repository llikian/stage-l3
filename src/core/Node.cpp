/***************************************************************************************************
 * @file  Node.cpp
 * @brief Implementation of the Node struct
 **************************************************************************************************/

#include "core/Node.hpp"

#include "core/SceneGraph.hpp"

Node::Data::Data() : type(DataType::NONE), index(-1) { }

Node::Node(const std::string& name, int parent, unsigned int transform_index, Type type)
    : type(type),
      name(name),
      parent(parent),
      transform_index(transform_index),
      is_visible(true),
      is_selected(false) { }

void Node::add_data(DataType type, unsigned int index) {
    for(unsigned int i = 0 ; i < MAX_NODE_DATA_COUNT ; ++i) {
        if(data[i].type == DataType::NONE) {
            data[i].type = type;
            data[i].index = index;
            return;
        }
    }

    throw std::runtime_error("Exceeded MAX_NODE_DATA_COUNT.");
}

void Node::set_data(unsigned int index, DataType data_type, unsigned int data_index) {
    data[index].type = data_type;
    data[index].index = data_index;
}
