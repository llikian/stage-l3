/***************************************************************************************************
 * @file  Mesh.cpp
 * @brief Implementation of the Mesh class
 **************************************************************************************************/

#include "mesh/Mesh.hpp"

#include "maths/geometry.hpp"
#include "maths/mat3.hpp"

Mesh::Mesh(Primitive primitive)
    : primitive(primitive), stride(0), VAO(0), VBO(0), EBO(0) {
    for(AttributeType& attribute : attributes) { attribute = AttributeType::NONE; }
    enable_attribute(Attribute::POSITION);
}

Mesh::~Mesh() {
    delete_buffers();
}

void Mesh::draw() const {
    if(primitive == Primitive::NONE) {
        std::cout << "[WARNING] Mesh wasn't drawn as it didn't have a primitive.\n";
        return;
    }

    if(stride == 0) {
        std::cout << "[WARNING] Mesh wasn't drawn as it didn't have any active attributes.\n";
        return;
    }

    glBindVertexArray(VAO);

    if(indices.empty()) {
        glDrawArrays(get_opengl_enum_for_primitive(primitive), 0, data.size() / stride);
    } else {
        glDrawElements(get_opengl_enum_for_primitive(primitive), indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

void Mesh::set_primitive(Primitive primitive) {
    this->primitive = primitive;
}

Primitive Mesh::get_primitive() const {
    return primitive;
}

AttributeType Mesh::get_attribute_type(Attribute attribute) const {
    return attributes[static_cast<unsigned char>(attribute)];
}

bool Mesh::has_attribute(Attribute attribute) const {
    return get_attribute_type(attribute) != AttributeType::NONE;
}

size_t Mesh::get_vertices_amount() const {
    return data.size() / stride;
}

size_t Mesh::get_indices_amount() const {
    return indices.size();
}

void Mesh::get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const {
    if(has_attribute(Attribute::POSITION)) {
        const unsigned int offset = get_attribute_offset(Attribute::POSITION);
        for(unsigned int i = offset ; i < data.size() ; i += stride) {
            minimum.x = std::min(minimum.x, data[i]);
            minimum.y = std::min(minimum.y, data[i + 1]);
            minimum.z = std::min(minimum.z, data[i + 2]);

            maximum.x = std::max(maximum.x, data[i]);
            maximum.y = std::max(maximum.y, data[i + 1]);
            maximum.z = std::max(maximum.z, data[i + 2]);
        }
    }
}

void Mesh::clear() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    data.clear();
    indices.clear();
}

void Mesh::delete_buffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::apply_model_matrix(const mat4& model) {
    mat3 normals_model = transpose_inverse(model);

    const unsigned int pos_offset = get_attribute_offset(Attribute::POSITION);
    const unsigned int normal_offset = get_attribute_offset(Attribute::NORMAL);

    for(unsigned int i = 0 ; i < data.size() ; i += stride) {
        if(has_attribute(Attribute::POSITION)) {
            vec3* pos = reinterpret_cast<vec3*>(&data[pos_offset + i]);
            *pos = model * vec4(pos->x, pos->y, pos->z, 1.0f);
        }

        if(has_attribute(Attribute::NORMAL)) {
            vec3* normal = reinterpret_cast<vec3*>(&data[normal_offset + i]);
            *normal = normalize(normals_model * (*normal));
        }
    }

    bind_buffers();
}

void Mesh::enable_attribute(Attribute attribute, AttributeType type) {
    if(type == AttributeType::NONE) { type = get_default_attribute_type(attribute); }

    AttributeType& old_type = get_attribute_type(attribute);
    stride += get_attribute_type_count(type) - get_attribute_type_count(old_type);
    old_type = type;
}

void Mesh::disable_attribute(Attribute attribute) {
    AttributeType& type = get_attribute_type(attribute);
    stride -= get_attribute_type_count(type);
    type = AttributeType::NONE;
}

void Mesh::add_line(unsigned int start, unsigned int end) {
    indices.push_back(start);
    indices.push_back(end);
}

void Mesh::add_line_triangle(unsigned int A, unsigned int B, unsigned int C) {
    add_line(A, B);
    add_line(B, C);
    add_line(C, A);
}

void Mesh::add_line_quad(unsigned int A, unsigned int B, unsigned int C, unsigned int D) {
    add_line(A, B);
    add_line(B, C);
    add_line(C, D);
    add_line(D, A);
}

void Mesh::add_triangle(unsigned int top, unsigned int left, unsigned int right) {
    indices.push_back(top);
    indices.push_back(left);
    indices.push_back(right);
}

void Mesh::add_face(unsigned int topL, unsigned int bottomL, unsigned int bottomR, unsigned int topR) {
    add_triangle(topL, bottomL, bottomR);
    add_triangle(topL, bottomR, topR);
}

void Mesh::bind_buffers() {
    /* VAO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* VBO */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    /* Vertex Attributes */
    float stride_in_bytes = stride * sizeof(float);
    unsigned int offset = 0;

    for(unsigned int i = 0 ; i < static_cast<unsigned char>(Attribute::AMOUNT) ; ++i) {
        AttributeType type = get_attribute_type(static_cast<Attribute>(i));
        if(type != AttributeType::NONE) {
            unsigned int size = get_attribute_type_count(type);
            glVertexAttribPointer(i, size, GL_FLOAT, false, stride_in_bytes, reinterpret_cast<void*>(offset));
            glEnableVertexAttribArray(i);
            offset += size * sizeof(float);
        }
    }

    /* Indices & EBO */
    if(!indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
    }
}

unsigned int Mesh::get_attribute_offset(Attribute attribute) const {
    const unsigned int attribute_id = static_cast<unsigned char>(attribute);
    unsigned int offset = 0;

    for(unsigned int i = 0 ; i < attribute_id ; ++i) {
        Attribute attr = static_cast<Attribute>(i);
        offset += get_attribute_type_count(get_attribute_type(attr));
    }

    return offset;
}

void Mesh::push_value(float value) {
    data.push_back(value);
}

void Mesh::push_value(const vec2& value) {
    data.push_back(value.x);
    data.push_back(value.y);
}

void Mesh::push_value(const vec3& value) {
    data.push_back(value.x);
    data.push_back(value.y);
    data.push_back(value.z);
}

void Mesh::push_value(const vec4& value) {
    data.push_back(value.x);
    data.push_back(value.y);
    data.push_back(value.z);
    data.push_back(value.w);
}

AttributeType& Mesh::get_attribute_type(Attribute attribute) {
    return attributes[static_cast<unsigned char>(attribute)];
}
