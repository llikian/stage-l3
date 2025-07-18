/***************************************************************************************************
 * @file  Window.cpp
 * @brief Implementation of the Window class
 **************************************************************************************************/

#include "Window.hpp"

#include <glad/glad.h>
#include <unordered_set>

static void glfw_error_callback(int code, const char* message) {
    std::cerr << "GLFW Error '" << code << "' : " << message << '\n';
}

static void opengl_error_callback(unsigned int source,
                                  unsigned int type,
                                  unsigned int id,
                                  unsigned int severity,
                                  int /* length */,
                                  const GLchar* message,
                                  const void* /* userParam */) {
    static const std::unordered_set<unsigned int> ignore_list{
        // Buffer detailed info: Buffer object ... (bound to GL_ELEMENT_ARRAY_BUFFER_ARB, usage hint
        // is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
        131185,
    };

    if(ignore_list.contains(id)) { return; }

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

Window::Window()
    : window(nullptr) {
    /* ---- GLFW ---- */
    glfwSetErrorCallback(glfw_error_callback);

    if(!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "Projet Stage L3", nullptr, nullptr);
    if(window == nullptr) {
        throw std::runtime_error("Failed to create window.");
    }

    glfwMakeContextCurrent(window);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMaximizeWindow(window);
    glfwGetWindowSize(window, &width, &height);

    /* ---- GLAD ---- */
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    /* ---- OpenGL ---- */
    glViewport(0, 0, width, height);

    glEnable(GL_CULL_FACE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.1, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(opengl_error_callback, nullptr);

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    // Sets the default texture to a plain magenta color
    constexpr unsigned char magenta[3]{ 255, 0, 255 };
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, magenta);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Window::get_glfw() {
    return get().window;
}

int Window::get_width() {
    return get().width;
}

int Window::get_height() {
    return get().height;
}

float Window::get_aspect_ratio() {
    const Window& window = get();
    return static_cast<float>(window.width) / static_cast<float>(window.height);
}

void Window::update_size(int width, int height) {
    Window& window = get();
    window.width = width;
    window.height = height;
}

vec2 Window::get_resolution() {
    Window& window = get();
    return vec2(window.width, window.height);
}

bool Window::should_close() {
    return glfwWindowShouldClose(get_glfw());
}

void Window::swap_buffers() {
    glfwSwapBuffers(get_glfw());
}
