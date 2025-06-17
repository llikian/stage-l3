/***************************************************************************************************
 * @file  Shader.cpp
 * @brief Implementation of the Shader class
 **************************************************************************************************/

#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <glad/glad.h>

Shader::Shader() : id(0) { }

Shader::Shader(const std::initializer_list<std::filesystem::path>& paths_list,
               const std::string& shader_program_name)
    : id(0) {
    create(paths_list, shader_program_name);
}

Shader::Shader(const Shader& shader) : id(shader.id), name(shader.name), uniform_locations(shader.uniform_locations) { }

Shader& Shader::operator=(const Shader& shader) {
    id = shader.id;
    name = shader.name;
    uniform_locations = shader.uniform_locations;

    return *this;
}

void Shader::free() {
    glDeleteProgram(id);
    uniform_locations.clear();
    std::cout << "Freed shader program '" << name << "'.\n";
}

void Shader::create(const std::initializer_list<std::filesystem::path>& paths_list,
                    const std::string& shader_program_name) {
    id = glCreateProgram();

    if(shader_program_name.empty()) {
        for(const std::filesystem::path& path : paths_list) {
            if(path.extension() == ".frag") {
                name = path.stem();
                break;
            }
        }

        if(name.empty()) { name = "shader_" + std::to_string(id); }
    } else {
        name = shader_program_name;
    }

    /* ---- Shaders ---- */
    for(const std::filesystem::path& path : paths_list) {
        unsigned int shader_id = compile_shader(path);
        glAttachShader(id, shader_id);
        glDeleteShader(shader_id);
    }

    /* ---- Shader Program ---- */
    glLinkProgram(id);

    int message_length;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &message_length);
    if(message_length > 0) {
        char* message = new char[message_length];
        glGetProgramInfoLog(id, message_length, nullptr, message);
        std::string error_message = "Failed to link shader program '" + shader_program_name + ":\n" + message;

        delete[] message;
        throw std::runtime_error(error_message);
    }

    get_uniforms();

    std::cout << "Created shader program '" << name << "'.\n";
}

unsigned int Shader::compile_shader(const std::filesystem::path& path) {
    std::string extension = path.extension();
    std::string shader_type_name;
    unsigned int shader_type;

    switch(extension[1]) {
        case 'v':
            shader_type_name = "vertex";
            shader_type = GL_VERTEX_SHADER;
            break;
        case 'f':
            shader_type_name = "fragment";
            shader_type = GL_FRAGMENT_SHADER;
            break;
        case 't':
            if(extension[4] == 'c') {
                shader_type_name = "tesselation control";
                shader_type = GL_TESS_CONTROL_SHADER;
            } else {
                shader_type_name = "tesselation evaluation";
                shader_type = GL_TESS_EVALUATION_SHADER;
            }
            break;
        case 'c':
            shader_type_name = "compute";
            shader_type = GL_COMPUTE_SHADER;
            break;
        case 'g':
            shader_type_name = "geometry";
            shader_type = GL_GEOMETRY_SHADER;
            break;
        default:
            throw std::runtime_error("Unknown shader extension: " + extension);
    }

    std::ifstream file(path);
    if(!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open " + shader_type_name + " shader."));
    }

    std::string raw_code = (std::stringstream() << file.rdbuf()).str();
    const char* code = raw_code.c_str();
    unsigned int shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &code, nullptr);
    glCompileShader(shader_id);

    int message_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &message_length);
    if(message_length > 0) {
        char* message = new char[message_length];
        glGetShaderInfoLog(shader_id, message_length, nullptr, message);
        std::string error_message = "Failed to compile " + shader_type_name + " shader '"
                                    + path.string() + "':\n" + message;

        delete[] message;
        throw std::runtime_error(error_message);
    }

    return shader_id;
}

void Shader::use() const {
    glUseProgram(id);
}

void Shader::get_uniforms() {
    use();

    int max_name_length;
    glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length);

    int length;
    int size;
    unsigned int type;
    char* uniform_name = new char[max_name_length];

    int count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

    for(int i = 0 ; i < count ; ++i) {
        glGetActiveUniform(id, i, max_name_length, &length, &size, &type, uniform_name);

        if(size == 1) { // Single value
            uniform_locations.emplace(uniform_name, glGetUniformLocation(id, uniform_name));
        } else { // Array
            std::string index;
            for(int j = 0 ; j < size ; ++j) {
                uniform_name[length - 2] = '\0';
                index = uniform_name;
                index += std::to_string(j) + ']';
                uniform_locations.emplace(index, glGetUniformLocation(id, index.c_str()));
            }
        }
    }

    delete[] uniform_name;
}

unsigned int Shader::get_id() const {
    return id;
}

std::string Shader::get_name() const {
    return name;
}

void Shader::set_uniform(int location, int value) {
    glUniform1i(location, value);
}

void Shader::set_uniform(int location, int x, int y) {
    glUniform2i(location, x, y);
}

void Shader::set_uniform(int location, int x, int y, int z) {
    glUniform3i(location, x, y, z);
}

void Shader::set_uniform(int location, int x, int y, int z, int w) {
    glUniform4i(location, x, y, z, w);
}

void Shader::set_uniform(int location, unsigned int value) {
    glUniform1ui(location, value);
}

void Shader::set_uniform(int location, bool value) {
    glUniform1i(location, static_cast<int>(value));
}

void Shader::set_uniform(int location, float value) {
    glUniform1f(location, value);
}

void Shader::set_uniform(int location, float x, float y) {
    glUniform2f(location, x, y);
}

void Shader::set_uniform(int location, float x, float y, float z) {
    glUniform3f(location, x, y, z);
}

void Shader::set_uniform(int location, float x, float y, float z, float w) {
    glUniform4f(location, x, y, z, w);
}

void Shader::set_uniform(int location, const vec2& vec) {
    glUniform2fv(location, 1, &vec.x);
}

void Shader::set_uniform(int location, const vec3& vec) {
    glUniform3fv(location, 1, &vec.x);
}

void Shader::set_uniform(int location, const vec4& vec) {
    glUniform4fv(location, 1, &vec.x);
}

void Shader::set_uniform(int location, const mat3& matrix) {
    glUniformMatrix3fv(location, 1, false, &(matrix(0, 0)));
}

void Shader::set_uniform(int location, const mat4& matrix) {
    glUniformMatrix4fv(location, 1, false, &(matrix(0, 0)));
}
