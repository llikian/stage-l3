/***************************************************************************************************
 * @file  Material.hpp
 * @brief Declaration of the Material struct
 **************************************************************************************************/

#pragma once

#include <string>
#include "assets/Shader.hpp"

/**
 * @struct Material
 * @brief
 */
struct Material {
    /**
     * @brief Sets default values for the material's data.
     * @param name The material's name.
     */
    explicit Material(const std::string& name);

    virtual ~Material() = default;

    /**
     * @brief Updates a shader's uniforms' values with the material's data.
     * @param shader The shader whose uniforms need to be updated.
     */
    virtual void update_shader_uniforms(const Shader* shader) const = 0;

    /**
     * @return Whether any of the material's textures has transparency.
     */
    virtual bool has_transparency() const = 0;

    /**
     * @brief Add this material to the object editor.
     */
    virtual void add_to_object_editor() = 0;

    std::string name; ///< The material's name.
};
