/***************************************************************************************************
 * @file  Attribute.hpp
 * @brief Declaration of the Attribute enum class
 **************************************************************************************************/

#pragma once

#include "maths/vec2.hpp"
#include "maths/vec3.hpp"
#include "maths/vec4.hpp"

enum Attribute : unsigned char {
    ATTRIBUTE_POSITION,
    ATTRIBUTE_NORMAL,
    ATTRIBUTE_TEX_COORDS,
    ATTRIBUTE_COLOR,
    ATTRIBUTE_TANGENT,
    ATTRIBUTE_POINT_SIZE,

    ATTRIBUTE_AMOUNT
};

enum class AttributeType : unsigned char {
    NONE,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
};

inline unsigned int get_attribute_type_count(AttributeType type) {
    switch(type) {
        case AttributeType::FLOAT: return 1;
        case AttributeType::VEC2: return 2;
        case AttributeType::VEC3: return 3;
        case AttributeType::VEC4: return 4;
        default: return 0;
    }
}

inline AttributeType get_attribute_type_from_value(float) { return AttributeType::FLOAT; }

inline AttributeType get_attribute_type_from_value(const vec2&) { return AttributeType::VEC2; }

inline AttributeType get_attribute_type_from_value(const vec3&) { return AttributeType::VEC3; }

inline AttributeType get_attribute_type_from_value(const vec4&) { return AttributeType::VEC4; }

inline AttributeType get_default_attribute_type(Attribute attribute) {
    switch(attribute) {
        case ATTRIBUTE_POSITION: return AttributeType::VEC3;
        case ATTRIBUTE_NORMAL: return AttributeType::VEC3;
        case ATTRIBUTE_TEX_COORDS: return AttributeType::VEC2;
        case ATTRIBUTE_COLOR: return AttributeType::VEC3;
        case ATTRIBUTE_TANGENT: return AttributeType::VEC4;
        case ATTRIBUTE_POINT_SIZE: return AttributeType::FLOAT;
        default: return AttributeType::NONE;
    }
}

inline std::string attribute_to_string(Attribute attribute) {
    switch(attribute) {
        case ATTRIBUTE_POSITION: return "POSITION";
        case ATTRIBUTE_NORMAL: return "NORMAL";
        case ATTRIBUTE_TEX_COORDS: return "TEX_COORDS";
        case ATTRIBUTE_COLOR: return "COLOR";
        case ATTRIBUTE_TANGENT: return "TANGENT";
        case ATTRIBUTE_POINT_SIZE: return "POINT_SIZE";
        default: return "INVALID";
    }
}

inline std::string attribute_type_to_string(AttributeType type) {
    switch(type) {
        case AttributeType::FLOAT: return "FLOAT";
        case AttributeType::VEC2: return "VEC2";
        case AttributeType::VEC3: return "VEC3";
        case AttributeType::VEC4: return "VEC4";
        default: return "NONE";
    }
}
