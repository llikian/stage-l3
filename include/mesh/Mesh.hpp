/***************************************************************************************************
 * @file  Mesh.hpp
 * @brief Declaration of the Mesh class
 **************************************************************************************************/

#pragma once

#include <vector>

#include "Attribute.hpp"
#include "culling/AABB.hpp"
#include "glad/glad.h"
#include "maths/mat4.hpp"
#include "maths/vec2.hpp"
#include "maths/vec3.hpp"
#include "maths/vec4.hpp"

enum class Primitive : unsigned char {
    NONE,
    POINTS,
    LINES,
    TRIANGLES,
};

/**
 * @class Mesh
 * @brief
 */
class Mesh {
public:
    explicit Mesh(Primitive primitive = Primitive::NONE);
    ~Mesh();

    void draw() const;

    void set_primitive(Primitive primitive);

    /**
     * @return The mesh's primitive.
     */
    Primitive get_primitive() const;

    AttributeType get_attribute_type(Attribute attribute) const;

    bool has_attribute(Attribute attribute) const;

    /**
     * @return The amount of vertices in the mesh.
     */
    size_t get_vertices_amount() const;

    /**
     * @return The amount of indices in the mesh.
     */
    size_t get_indices_amount() const;

    AttributeType get_attribute_type(Attribute attribute);

    AABB get_AABB() const;

    /**
     * @brief Calculates the minimum and maximum value for each coordinate for every position in the
     * mesh. If the mesh doesn't have positions, does nothing.
     * @param minimum The lowest x, y and z values of the position in the mesh.
     * @param maximum The highest x, y and z values of the position in the mesh.
     */
    void get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const;

    /**
     * @brief Delete OpenGL buffers and clears the vertices array and the indices array.
     */
    void clear();

    /**
     * @brief Delete OpenGL buffers.
     */
    void delete_buffers();

    /**
     * @brief Applies a model matrix to each vertex in the mesh. If they are enabled, does this to
     * the following attributes:
     * - Position: P = model * P
     * - Normal: N = normalize(transpose(inverse(mat3(model))) * N)
     * @param model The model matrix to apply.
     */
    void apply_model_matrix(const mat4& model);

    /**
     * @brief  Enables an attribute by setting its data type.
     * @param attribute The attribute to enable.
     * @param type The data type of the attribute. If 'AttributeType::NONE' is passed, a default
     * value is set, see @link get_default_attribute_type.
     */
    void enable_attribute(Attribute attribute, AttributeType type = AttributeType::NONE);
    void disable_attribute(Attribute attribute);

    template <typename... Args>
    void add_vertex(Args&&... attribute_values) {
        if(sizeof...(attribute_values) != active_attributes_count) {
            throw std::runtime_error("Trying to pass "
                                     + std::to_string(sizeof...(attribute_values))
                                     + " arguments to add_vertex but the mesh has "
                                     + std::to_string(active_attributes_count)
                                     + " active attributes.");
        }

        add_vertex_helper(0, std::forward<Args>(attribute_values)...);
    }

    void add_line(unsigned int start, unsigned int end);
    void add_line_triangle(unsigned int A, unsigned int B, unsigned int C);
    void add_line_quad(unsigned int A, unsigned int B, unsigned int C, unsigned int D);

    void add_triangle(unsigned int top, unsigned int left, unsigned int right);
    void add_face(unsigned int topL, unsigned int bottomL, unsigned int bottomR, unsigned int topR);

    void bind_buffers();

    void push_value(float value);
    void push_value(const vec2& value);
    void push_value(const vec3& value);
    void push_value(const vec4& value);
    void push_values(const float* values, unsigned int n);

    void push_indices_buffer(const std::vector<unsigned int>& indices);

private:
    unsigned int get_attribute_offset(Attribute attribute) const;

    template <typename Type, typename... Args>
    void add_vertex_helper(unsigned int attribute_id, Type&& value, Args&&... attribute_values) {
        while(attributes[attribute_id] == AttributeType::NONE) { ++attribute_id; }

        AttributeType type = attributes[attribute_id];
        AttributeType value_type = get_attribute_type_from_value(value);

        if(type != value_type) {
            throw std::runtime_error("Trying to add an attribute value of type '"
                                     + attribute_type_to_string(value_type)
                                     + "' for an attribute that has type '"
                                     + attribute_type_to_string(type)
                                     + "'.");
        }

        push_value(std::forward<Type>(value));

        add_vertex_helper(++attribute_id, std::forward<Args>(attribute_values)...);
    }

    static void add_vertex_helper(unsigned int) { }

    Primitive primitive;

    AttributeType attributes[ATTRIBUTE_AMOUNT];
    unsigned int stride;                  ///< Stride in amount of floats (not bytes).
    unsigned int active_attributes_count; ///< Amount of active attributes.

    std::vector<float> data;
    std::vector<unsigned int> indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    AABB aabb;
};

inline unsigned int get_opengl_enum_for_primitive(Primitive primitive) {
    switch(primitive) {
        case Primitive::POINTS: return GL_POINTS;
        case Primitive::LINES: return GL_LINES;
        case Primitive::TRIANGLES: return GL_TRIANGLES;
        default: return GL_NONE;
    }
}
