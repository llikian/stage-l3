/***************************************************************************************************
 * @file  Attribute.hpp
 * @brief Declaration of the Attribute enum class
 **************************************************************************************************/

#pragma once

#include "maths/vec2.hpp"
#include "maths/vec3.hpp"
#include "maths/vec4.hpp"

enum class Attribute : unsigned char {
    POSITION,
    NORMAL,
    TEX_COORDS,
    COLOR,

    AMOUNT
};

enum class AttributeType : unsigned char {
    NONE,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
};

constexpr unsigned int get_attribute_type_count(AttributeType type) {
    switch(type) {
        case AttributeType::FLOAT: return 1;
        case AttributeType::VEC2: return 2;
        case AttributeType::VEC3: return 3;
        case AttributeType::VEC4: return 4;
        default: return 0;
    }
}

constexpr AttributeType get_attribute_type_from_value(float) { return AttributeType::FLOAT; }
constexpr AttributeType get_attribute_type_from_value(const vec2&) { return AttributeType::VEC2; }
constexpr AttributeType get_attribute_type_from_value(const vec3&) { return AttributeType::VEC3; }
constexpr AttributeType get_attribute_type_from_value(const vec4&) { return AttributeType::VEC4; }

constexpr AttributeType get_default_attribute_type(Attribute attribute) {
    switch(attribute) {
        case Attribute::POSITION: return AttributeType::VEC3;
        case Attribute::NORMAL: return AttributeType::VEC3;
        case Attribute::TEX_COORDS: return AttributeType::VEC2;
        case Attribute::COLOR: return AttributeType::VEC3;
        default: return AttributeType::NONE;
    }
}

std::string get_attribute_type_string(AttributeType type);
