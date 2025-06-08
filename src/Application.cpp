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
    : event_handler(&window),
      window("Projet Stage L3", this),
      shader({ "shaders/default.vert", "shaders/default.frag" }, "default"),
      projection(perspective(M_PI_4f, window.get_size_ratio(), 0.1f, 100.0f)),
      camera(vec3(0.0f, 0.0f, 0.0f)) {
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    /* ---- Event Actions ---- */
    event_handler.associate_action_to_key(GLFW_KEY_ESCAPE, false, [this] { glfwSetWindowShouldClose(window, true); });

    event_handler.associate_action_to_key(GLFW_KEY_W, true, [this] {
        camera.move_around(MovementDirection::FORWARD, event_handler.get_delta());
    });
    event_handler.associate_action_to_key(GLFW_KEY_A, true, [this] {
        camera.move_around(MovementDirection::LEFT, event_handler.get_delta());
    });
    event_handler.associate_action_to_key(GLFW_KEY_S, true, [this] {
        camera.move_around(MovementDirection::BACKWARD, event_handler.get_delta());
    });
    event_handler.associate_action_to_key(GLFW_KEY_D, true, [this] {
        camera.move_around(MovementDirection::RIGHT, event_handler.get_delta());
    });
    event_handler.associate_action_to_key(GLFW_KEY_SPACE, true, [this] {
        camera.move_around(MovementDirection::UPWARD, event_handler.get_delta());
    });
    event_handler.associate_action_to_key(GLFW_KEY_LEFT_SHIFT, true, [this] {
        camera.move_around(MovementDirection::DOWNWARD, event_handler.get_delta());
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

        shader.use();

        update_mvp(translate_z(-5.0f) * rotate_y(45));

        cube.draw();

        glfwSwapBuffers(window);
    }
}

void Application::update_mvp(const mat4& model) const {
    mat4 mvp = projection * camera.get_view_matrix() * model;
    shader.set_uniform("mvp", mvp);
    shader.set_uniform("normalModel", transpose_inverse(mvp));
}
