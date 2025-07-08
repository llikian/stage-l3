/***************************************************************************************************
 * @file  Attribute.cpp
 * @brief Implementation of the Attribute enum class
 **************************************************************************************************/

#include "mesh/Attribute.hpp"

std::string get_attribute_type_string(AttributeType type) {
    switch(type) {
        case AttributeType::FLOAT: return "FLOAT";
        case AttributeType::VEC2: return "VEC2";
        case AttributeType::VEC3: return "VEC3";
        case AttributeType::VEC4: return "VEC4";
        default: return "NONE";
    }
}
