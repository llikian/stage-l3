/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>
#include <glad/glad.h>
#include "callbacks.hpp"
#include "maths/mat3.hpp"
#include "maths/transforms.hpp"
#include "mesh/TriangleMesh.hpp"

Application::Application()
    : window("Projet Stage L3", this),
      shader({ "shaders/default.vert", "shaders/default.frag" }, "default"),
      camera(vec3(0.0f, 0.0f, 0.0f)),
      fov(M_PI_4), projection(perspective(fov, window.get_size_ratio(), 0.1f, 100.0f)),
      event_handler(&window, &camera) {
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    /* ---- Event Handler ---- */
    event_handler.set_active_camera(&camera);
    event_handler.set_window_size_event_action([this] {
        projection(0, 0) = 1.0f / (window.get_size_ratio() * tanf(0.5f * fov));
    });

    /* ---- Other ---- */
    glClearColor(0.1, 0.1f, 0.1f, 1.0f);
}

void Application::run() {
    const vec3 cube_positions[8]{
        vec3(1.0f, 1.0f, 1.0f),
        vec3(1.0f, 1.0f, -1.0f),
        vec3(1.0f, -1.0f, 1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(-1.0f, 1.0f, -1.0f),
        vec3(-1.0f, -1.0f, 1.0f),
        vec3(-1.0f, -1.0f, -1.0f)
    };

    TriangleMesh cube;

    auto add_face = [&cube_positions, &cube](unsigned int A, // TOP LEFT
                                             unsigned int B, // BOTTOM LEFT
                                             unsigned int C, // BOTTOM RIGHT
                                             unsigned int D, // TOP RIGHT
                                             const vec3& normal) {
        cube.addVertex(cube_positions[A], normal, vec2(0.0f, 1.0f));
        cube.addVertex(cube_positions[B], normal, vec2(0.0f, 0.0f));
        cube.addVertex(cube_positions[D], normal, vec2(1.0f, 1.0f));

        cube.addVertex(cube_positions[B], normal, vec2(0.0f, 0.0f));
        cube.addVertex(cube_positions[C], normal, vec2(1.0f, 0.0f));
        cube.addVertex(cube_positions[D], normal, vec2(1.0f, 1.0f));
    };

    add_face(0, 2, 3, 1, vec3(1.0f, 0.0f, 0.0f));
    add_face(5, 7, 6, 4, vec3(-1.0f, 0.0f, 0.0f));
    add_face(0, 1, 5, 4, vec3(0.0f, 1.0f, 0.0f));
    add_face(3, 2, 6, 7, vec3(0.0f, -1.0f, 0.0f));
    add_face(4, 6, 2, 0, vec3(0.0f, 0.0f, 1.0f));
    add_face(1, 3, 7, 5, vec3(0.0f, 0.0f, -1.0f));

    while(!glfwWindowShouldClose(window)) {
        event_handler.poll_and_handle_events();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view_projection = projection * camera.get_view_matrix();

        shader.use();

        for(int i = -5 ; i <= 5 ; ++i) {
            for(int j = -5 ; j <= 5 ; ++j) {
                update_mvp(translate(i * 3, std::sqrt(i * i + j * j), j * 3));
                cube.draw();
            }
        }

        glfwSwapBuffers(window);
    }
}

EventHandler& Application::get_event_handler() {
    return event_handler;
}

void Application::update_mvp(const mat4& model) const {
    shader.set_uniform("mvp", view_projection * model);
    shader.set_uniform("normals_model_matrix", transpose_inverse(model));
}
