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
#include "mesh/Model.hpp"
#include "mesh/TriangleMesh.hpp"

Application::Application()
    : window("Projet Stage L3", this),
      shader({ "shaders/default.vert", "shaders/blinn-phong.frag" }, "default"),
      camera(vec3(0.0f, 0.0f, 0.0f)),
      fov(M_PI_4), projection(perspective(fov, window.get_size_ratio(), 0.1f, 500.0f)),
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
    Model sponza("data/sponza/sponza.obj", scale(0.05f), true);
    // Model vokselia("data/vokselia_spawn/vokselia_spawn.obj", scale(10.0f), true);

    while(!glfwWindowShouldClose(window)) {
        event_handler.poll_and_handle_events();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view_projection = projection * camera.get_view_matrix();

        shader.use();

        shader.set_uniform("u_camera_position", camera.get_position());

        update_mvp(sponza.model_matrix);
        sponza.draw(shader);

        // update_mvp(vokselia.model);
        // vokselia.draw(shader);

        glfwSwapBuffers(window);
    }
}

EventHandler& Application::get_event_handler() {
    return event_handler;
}

void Application::update_mvp(const mat4& model) const {
    shader.set_uniform("u_mvp", view_projection * model);
    shader.set_uniform("u_model", model);
    shader.set_uniform("u_normals_model_matrix", transpose_inverse(model));
}
