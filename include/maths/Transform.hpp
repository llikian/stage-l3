/***************************************************************************************************
 * @file  Transform.hpp
 * @brief Declaration of the Transform class
 **************************************************************************************************/

#pragma once

#include "maths/mat4.hpp"
#include "maths/transforms.hpp"

/**
 * @class Transform
 * @brief Holds information about an entity's local position, orientation and scale and stores its
 * global transformation information, the product of the local model matrices of all its parents and
 * itself.
 */
class Transform {
public:
    /**
     * @brief Default constructor, creates a transform at the origin, with no orientation and a
     * scale of 1.
     */
    Transform();

    /**
     * @brief Changes the local position of the transform.
     * @param position The transform's new position.
     */
    void set_local_position(const vec3& position);

    /**
     * @brief Changes the local position of the transform.
     * @param x The transform's new position on the x axis.
     * @param y The transform's new position on the y axis.
     * @param z The transform's new position on the z axis.
     */
    void set_local_position(float x, float y, float z);

    /**
     * @brief Changes the local orientation of the transform.
     * @param orientation The transform's new orientation.
     */
    void set_local_orientation(const quaternion& orientation);

    /**
     * @brief Changes the local orientation of the transform using euler angles.
     * @param angles Rotation angles in degrees around each axis.
     */
    void set_local_orientation_euler(const vec3& angles);

    /**
     * @brief Changes the local orientation of the transform.
     * @param x The value for the coefficient of the orientation quaternion's i imaginary unit.
     * @param y The value for the coefficient of the orientation quaternion's j imaginary unit.
     * @param z The value for the coefficient of the orientation quaternion's k imaginary unit.
     * @param w The value for the real part of the orientation quaternion.
     */
    void set_local_orientation(float x, float y, float z, float w);

    /**
     * @brief Changes the local scale of the transform.
     * @param scale The transform's new scale.
     */
    void set_local_scale(const vec3& scale);

    /**
     * @brief Changes the local scale of the transform uniformly along each axes.
     * @param scale The transform's new scale.
     */
    void set_local_scale(float scale);

    /**
     * @brief Changes the local scale of the transform.
     * @param x The transform's new scale along the x axis.
     * @param y The transform's new scale along the y axis.
     * @param z The transform's new scale along the z axis.
     */
    void set_local_scale(float x, float y, float z);

    /**
     * @brief Decompose a row-major model matrix's values into the local model's parameters (position,
     * orientation and scale).
     * @param model The row-major model matrix's values.
     */
    void set_local_model(const float model[16]);

    /**
     * @brief Sets the is_dirty flag to true, meaning the local model needs to be recalculated.
     */
    void set_local_model_to_dirty();

    /**
     * @return The transform's local position.
     */
    vec3 get_local_position() const;

    /**
     * @return A reference to the transform's local position. If the value is modified,
     * set_local_model_to_dirty need to be called.
     */
    vec3& get_local_position_reference();

    /**
     * @return The transform's local orientation.
     */
    quaternion get_local_orientation() const;

    /**
     * @return A reference to the transform's local orientation. If the value is modified,
     * set_local_model_to_dirty need to be called.
     */
    quaternion& get_local_orientation_reference();

    /**
     * @return The transform's local scale.
     */
    vec3 get_local_scale() const;

    /**
     * @return A reference to the transform's local scale. If the value is modified,
     * set_local_model_to_dirty need to be called.
     */
    vec3& get_local_scale_reference();

    /**
     * @return The transform's global model, the product of the local model matrices of all its
     * parents and itself.
     */
    mat4 get_global_model() const;

    /**
     * @return A const reference to the transform's global model, the product of the local model
     * matrices of all its parents and itself.
     */
    const mat4& get_global_model_const_reference() const;

    /**
     * @return The transform's global position.
     */
    vec3 get_global_position() const;

    /**
     * @return The transform's global scale.
     */
    vec3 get_global_scale() const;

    /**
     * @return The transform's global matrix's front vector: the first 3 components of its third row
     * multiplied by -1.
     */
    vec3 get_front_vector() const;

    /**
     * @return The transform's global matrix's right vector: the first 3 components of its first row.
     */
    vec3 get_right_vector() const;

    /**
     * @return The transform's global matrix's up vector: the first 3 components of its second row.
     */
    vec3 get_up_vector() const;

    /**
     * @return Whether the local model was modified and needs to be recomputed.
     */
    bool is_local_model_dirty() const;

    /**
     * @brief Computes and returns the local model matrix.
     * @return The transform's local model matrix.
     */
    mat4 compute_local_model() const;

    /**
     * @brief Sets the value of the global model matrix to the local model matrix. Used if the entity
     * related to the transform does not have any parents.
     */
    void update_global_model();

    /**
     * @brief Sets the value of the global model matrix to the product of the global model of the
     * parent of the entity related to the transform and the transform's local model matrix.
     * @param parent_global_model The entity related to the transform's parent's global model matrix.
     */
    void update_global_model(const mat4& parent_global_model);

private:
    vec3 local_position;          ///< The transform's local position.
    quaternion local_orientation; ///< The transform's local orientation.
    vec3 local_scale;             ///< The transform's local scale.

    bool is_dirty; ///< Whether the local model was modified.

    /// The transform's global model, the product of the local model matrices of all its parents and itself.
    mat4 global_model;
};
