/***************************************************************************************************
 * @file  Shader.hpp
 * @brief Declaration of the Shader class
 **************************************************************************************************/

#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>

/**
 * @class Shader
 * @brief Compiles and link shaders to a shader program that can then be bound. Contains functionality
 * to change the value of uniforms.
 */
class Shader {
public:
    /**
     * @brief Creates a shader program and compiles then attaches the shaders at the specified paths
     * to it.
     * @param paths_list The paths to each of the different shaders.
     * @param shader_program_name The name of the shader program.
     */
    Shader(std::initializer_list<std::string> paths_list, const std::string& shader_program_name);

    /**
     * @brief Deletes the shader program.
     */
    ~Shader();

    /**
     * @brief Compiles a shader and returns its corresponding id.
     * @param path The path to the shader file.
     * @return The shader's corresponding id.
     */
    static unsigned int compileShader(const std::string& path);

    /**
     * @brief Uses the shader program.
     */
    void use() const;

    /**
     * @brief Getter for the id of the shader.
     * @return The id of the shader.
     */
    uint getID() const;

    /**
     * @brief Getter for the name of the shader.
     * @return The name of the shader.
     */
    std::string getName() const;

    /**
     * @brief Sets the value of a uniform of any of the available types.
     * @param uniform The uniform's name.
     * @param value The new value of the uniform.
     */
    template <typename... Value>
    void setUniform(const std::string& uniform, Value... value);

private:
    /**
     * @brief Finds and adds all the shader's uniforms' id's to the map.
     */
    void get_uniforms();

    /**
     * @brief Sets the value of a uniform of type int.
     * @param location The uniform's location.
     * @param value The new value of the uniform.
     */
    static void set_uniform(int location, int value);

    /**
     * @brief Sets the value of a uniform of type ivec2.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     */
    static void set_uniform(int location, int x, int y);

    /**
     * @brief Sets the value of a uniform of type ivec3.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     */
    static void set_uniform(int location, int x, int y, int z);

    /**
     * @brief Sets the value of a uniform of type ivec4.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     * @param w The new w value of the uniform.
     */
    static void set_uniform(int location, int x, int y, int z, int w);

    /**
     * @brief Sets the value of a uniform of type unsigned int.
     * @param location The uniform's location.
     * @param value The new value of the uniform.
     */
    static void set_uniform(int location, unsigned int value);

    /**
     * @brief Sets the value of a uniform of type bool.
     * @param location The uniform's location.
     * @param value The new value of the uniform.
     */
    static void set_uniform(int location, bool value);

    /**
     * @brief Sets the value of a uniform of type float.
     * @param location The uniform's location.
     * @param value The new value of the uniform.
     */
    static void set_uniform(int location, float value);

    /**
     * @brief Sets the value of a uniform of type vec2.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     */
    static void set_uniform(int location, float x, float y);

    /**
     * @brief Sets the value of a uniform of type vec3.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     */
    static void set_uniform(int location, float x, float y, float z);

    /**
     * @brief Sets the value of a uniform of type vec4.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     * @param w The new w value of the uniform.
     */
    static void set_uniform(int location, float x, float y, float z, float w);

    unsigned int id;  ///< The shader program's id.
    std::string name; ///< The shader's name.

    std::unordered_map<std::string, int> uniform_locations; ///< Stores location of uniforms.
};

template <typename... Value>
void Shader::setUniform(const std::string& uniform, Value... value) {
    if(uniform.contains(uniform)) {
        setUniform(uniform_locations.at(uniform), value...);
    }
}
