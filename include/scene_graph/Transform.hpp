/***************************************************************************************************
 * @file  Transform.hpp
 * @brief Declaration of the Transform class
 **************************************************************************************************/

#pragma once

#include "maths/mat4.hpp"
#include "maths/transforms.hpp"

/**
 * @class Transform
 * @brief
 */
class Transform {
public:
    Transform();

    void set_local_position(const vec3& position);

    void set_local_rotation(const vec3& rotation);

    void set_local_scale(const vec3& scale);

    void set_local_model_to_dirty();

    vec3 get_local_position() const;
    vec3& get_local_position_reference();

    vec3 get_local_rotation() const;
    vec3& get_local_rotation_reference();

    vec3 get_local_scale() const;
    vec3& get_local_scale_reference();

    mat4 get_local_model() const;

    mat4 get_global_model() const;

    const mat4& get_global_model_reference() const;

    bool is_local_model_dirty() const;

    void compute_global_model();

    void compute_global_model(const mat4& parent_global_model);

private:
    vec3 local_position;
    vec3 local_rotation;
    vec3 local_scale;

    bool is_dirty;

    mat4 global_model;
};
