/***************************************************************************************************
 * @file  Window.cpp
 * @brief Implementation of the Window class
 **************************************************************************************************/

#include "Window.hpp"

static void glfw_error_callback(int code, const char* message) {
    std::cerr << "GLFW Error '" << code << "' : " << message << '\n';
}

static void opengl_error_callback(unsigned int source,
                                  unsigned int type,
                                  unsigned int id,
                                  unsigned int severity,
                                  int length,
                                  const GLchar* message,
                                  const void* userParam) {
    if(severity == GL_DEBUG_SEVERITY_NOTIFICATION) { return; }

    std::cerr << "OpenGL Message:"
        << "\n\tID: " << id
        << "\n\tSeverity: ";
    switch(severity) {
        case GL_DEBUG_SEVERITY_LOW:
            std::cerr << "Low";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cerr << "Medium";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            std::cerr << "High";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        default:
            std::cerr << "Notification";
            break;
    }

    std::cerr << "\n\tType: ";
    switch(type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cerr << "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cerr << "Deprecated Behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cerr << "Undefined Behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cerr << "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cerr << "Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cerr << "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cerr << "Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cerr << "Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
        default:
            std::cerr << "Other";
            break;
    }

    std::cerr << "\n\tSource: ";
    switch(source) {
        case GL_DEBUG_SOURCE_API:
            std::cerr << "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cerr << "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cerr << "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cerr << "Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cerr << "Applitcation";
            break;
        case GL_DEBUG_SOURCE_OTHER:
        default:
            std::cerr << "Other";
            break;
    }

    std::cerr << "\n\tMessage: " << message << '\n';
}

Window::Window(const std::string& window_name, void* user_pointer)
    : window(nullptr) {
    /* ---- GLFW ---- */
    glfwSetErrorCallback(glfw_error_callback);

    if(!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, window_name.c_str(), nullptr, nullptr);
    if(window == nullptr) {
        throw std::runtime_error("Failed to create window.");
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, user_pointer);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMaximizeWindow(window);
    glfwGetWindowSize(window, &width, &height);

    /* ---- GLAD ---- */
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    /* ---- OpenGL ---- */
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(opengl_error_callback, user_pointer);

    // Sets the default texture to a plain magenta color
    constexpr unsigned char magenta[3]{ 255, 255, 255 };
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, magenta);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

Window::operator GLFWwindow*() const {
    return window;
}

GLFWwindow* Window::get_GLFW_window() const {
    return window;
}

int Window::get_width() const {
    return width;
}

int Window::get_height() const {
    return height;
}

float Window::get_size_ratio() const {
    return static_cast<float>(width) / static_cast<float>(height);
}
