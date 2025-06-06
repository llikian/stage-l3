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

Application::Application()
    : event_handler(&window),
      window("Projet Stage L3", this),
      shader({ "shaders/default.vert", "shaders/default.frag" }, "default"),
      projection(perspective(M_PI_4f, window.get_size_ratio(), 0.1f, 100.0f)) {
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    /* ---- Event Actions ---- */
    event_handler.associate_action_to_key(GLFW_KEY_ESCAPE, false, [this] { glfwSetWindowShouldClose(window, true); });
    // event_handler.set_window_size_action([this](int width, int height) { window.update_size(width, height); });

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

    std::vector<vec3> mesh_data;

    auto add_face = [&cube_positions, &mesh_data](unsigned int A, unsigned int B, unsigned int C, unsigned int D,
                                                  const vec3& normal) {
        mesh_data.push_back(cube_positions[A]);
        mesh_data.push_back(normal);
        mesh_data.push_back(cube_positions[B]);
        mesh_data.push_back(normal);
        mesh_data.push_back(cube_positions[D]);
        mesh_data.push_back(normal);

        mesh_data.push_back(cube_positions[B]);
        mesh_data.push_back(normal);
        mesh_data.push_back(cube_positions[C]);
        mesh_data.push_back(normal);
        mesh_data.push_back(cube_positions[D]);
        mesh_data.push_back(normal);
    };

    add_face(0, 2, 3, 1, vec3(1.0f, 0.0f, 0.0f));
    add_face(5, 7, 6, 4, vec3(-1.0f, 0.0f, 0.0f));
    add_face(0, 1, 5, 4, vec3(0.0f, 1.0f, 0.0f));
    add_face(3, 2, 6, 7, vec3(0.0f, -1.0f, 0.0f));
    add_face(4, 6, 2, 0, vec3(0.0f, 0.0f, 1.0f));
    add_face(1, 3, 7, 5, vec3(0.0f, 0.0f, -1.0f));

    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh_data.size() * 2 * sizeof(vec3), mesh_data.data(), GL_STATIC_DRAW);

    /* Positions */
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 2 * sizeof(vec3), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    /* Normals */
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 2 * sizeof(vec3), reinterpret_cast<void*>(sizeof(vec3)));
    glEnableVertexAttribArray(1);

    while(!glfwWindowShouldClose(window)) {
        event_handler.poll_and_handle_events();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        mat4 mvp = projection * translate_z(-5.0f) * rotate_y(45);
        shader.set_uniform("mvp", mvp);
        shader.set_uniform("normalModel", transpose_inverse(mvp));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh_data.size());

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
