/***************************************************************************************************
 * @file  Shader.hpp
 * @brief Declaration of the Shader class
 **************************************************************************************************/

#pragma once

#include <filesystem>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "maths/mat3.hpp"
#include "maths/mat4.hpp"
#include "maths/vec2.hpp"
#include "maths/vec3.hpp"
#include "maths/vec4.hpp"

/**
 * @class Shader
 * @brief Compiles and link shaders to a shader program that can then be bound. Contains functionality
 * to change the value of uniforms.
 */
class Shader {
public:
    /**
     * @brief Default constructor. Sets the shader id to 0.
     */
    Shader();

    /**
     * @brief Constructs a shader program and compiles then attaches the shaders at the specified paths
     * to it.
     * @param paths_list The paths to each of the different shaders.
     * @param shader_program_name The name of the shader program.
     */
    Shader(const std::initializer_list<std::filesystem::path>& paths_list,
           const std::string& shader_program_name = "");

    /**
     * @brief Copy constructor.
     * @warning The responsibility of freeing the shader program goes to the user, so if multiple
     * copies of the same shader program exist, be sure that all copies are no longer in use before
     * freeing.
     * @param shader The shader to copy.
     */
    Shader(const Shader& shader);

    /**
     * @brief Copy operator.
     * @warning The responsibility of freeing the shader program goes to the user, so if multiple
     * copies of the same shader program exist, be sure that all copies are no longer in use before
     * freeing. Additionally, if this instance of the Shader class already had a shader program
     * (id != 0) and you no longer wish to use that shader program, be sure to call the free method
     * beforehand.
     * @param shader The shader to copy.
     */
    Shader& operator=(const Shader& shader);

    /**
     * @brief Deletes the shader program.
     */
    void free();

    /**
     * @brief Creates a shader program and compiles then attaches the shaders at the specified paths
     * to it.
     * @warning The responsibility of freeing the shader program goes to the user, so if this instance
     * of the Shader class already had a shader program (id != 0) and you no longer wish to use that
     * shader program, be sure to call the free method beforehand.
     * @param paths_list The paths to each of the different shaders.
     * @param shader_program_name The name of the shader program.
     */
    void create(const std::initializer_list<std::filesystem::path>& paths_list,
                const std::string& shader_program_name = "");

    /**
     * @brief Compiles a shader and returns its corresponding id.
     * @param path The path to the shader file.
     * @return The shader's corresponding id.
     */
    static unsigned int compile_shader(const std::filesystem::path& path);

    /**
     * @brief Uses the shader program.
     */
    void use() const;

    /**
     * @brief Getter for the id of the shader.
     * @return The id of the shader.
     */
    unsigned int get_id() const;

    /**
     * @brief Getter for the name of the shader.
     * @return The name of the shader.
     */
    std::string get_name() const;

    /**
     * @brief Sets the value of a uniform of any of the available types. Prints a warning if the
     * uniform is not found.
     * @param uniform The uniform's name.
     * @param value The new value of the uniform.
     */
    template <typename... Value>
    void set_uniform(const std::string& uniform, Value&&... value) const {
        std::unordered_map<std::string, int>::const_iterator uniform_iterator = uniform_locations.find(uniform);
        if(uniform_iterator != uniform_locations.end()) {
            set_uniform(uniform_iterator->second, std::forward<Value>(value)...);
        } else {
            std::cout << "[WARNING] Unknown uniform '" << uniform << "' in 'set_uniform' call for shader '" << name <<
                "'.\n";
        }
    }

    /**
     * @brief Sets the value of a uniform of any of the available types. Does not print a warning if
     * the uniform is not found.
     * @param uniform The uniform's name.
     * @param value The new value of the uniform.
     */
    template <typename... Value>
    void set_uniform_if_exists(const std::string& uniform, Value... value) const {
        std::unordered_map<std::string, int>::const_iterator uniform_iterator = uniform_locations.find(uniform);
        if(uniform_iterator != uniform_locations.end()) {
            set_uniform(uniform_iterator->second, value...);
        }
    }

    /**
     * @param uniform The uniform's name
     * @return Whether a uniform exists in this shader program.
     */
    bool does_uniform_exist(const std::string& uniform) const;

    /**
     * @brief Get the location of a uniform.
     * @param uniform The uniform's name.
     * @return The location of the uniform if it exists in the shader program, -1 otherwise.
     */
    int get_uniform_location(const std::string& uniform) const;

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

    /**
     * @brief Sets the value of a uniform of type vec2.
     * @param location The uniform's location.
     * @param vec The new value of the uniform.
     */
    static void set_uniform(int location, const vec2& vec);

    /**
     * @brief Sets the value of a uniform of type vec3.
     * @param location The uniform's location.
     * @param vec The new value of the uniform.
     */
    static void set_uniform(int location, const vec3& vec);

    /**
     * @brief Sets the value of a uniform of type vec4.
     * @param location The uniform's location.
     * @param vec The new value of the uniform.
     */
    static void set_uniform(int location, const vec4& vec);

    /**
     * @brief Sets the value of a uniform of type mat3.
     * @param location The uniform's location.
     * @param matrix The new value of the uniform.
     */
    static void set_uniform(int location, const mat3& matrix);

    /**
     * @brief Sets the value of a uniform of type mat4.
     * @param location The uniform's location.
     * @param matrix The new value of the uniform.
     */
    static void set_uniform(int location, const mat4& matrix);

private:
    /**
     * @brief Finds and adds all the shader's uniforms' id's to the map.
     */
    void get_uniforms();

    unsigned int id;  ///< The shader program's id.
    std::string name; ///< The shader's name.

    std::unordered_map<std::string, int> uniform_locations; ///< Stores location of uniforms.
};
