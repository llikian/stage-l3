/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>
#include <ranges>
#include <glad/glad.h>
#include "callbacks.hpp"
#include "maths/mat3.hpp"
#include "maths/transforms.hpp"
#include "mesh/Model.hpp"
#include "mesh/primitives.hpp"
#include "mesh/TriangleMesh.hpp"

Application::Application()
    : window("Projet Stage L3", this),
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

    /* ---- Shaders ---- */
    shaders.emplace("blinn-phong", Shader({ "shaders/vertex/default.vert", "shaders/fragment/blinn-phong.frag" }));
    shaders.emplace("flat", Shader({ "shaders/vertex/position-only.vert", "shaders/fragment/flat.frag" }));

    /* ---- Other ---- */
    glClearColor(0.1, 0.1f, 0.1f, 1.0f);
}

Application::~Application() {
    for(Shader& shader : shaders | std::views::values) { shader.free(); }
}

void Application::run() {
    Model sponza("data/sponza/sponza.obj", scale(0.05f), true);
    // Model vokselia("data/vokselia_spawn/vokselia_spawn.obj", scale(10.0f), true);

    TriangleMesh sphere;
    create_sphere_mesh(sphere, 8, 16);

    vec3 light_color(1.0f);
    vec3 light_position(0.0f, 20.0f, 0.0f);

    while(!glfwWindowShouldClose(window)) {
        event_handler.poll_and_handle_events();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view_projection = projection * camera.get_view_matrix();

        /* Blinn-Phong Shader */ {
            const Shader& shader = shaders["blinn-phong"];
            shader.use();

            shader.set_uniform("u_camera_position", camera.get_position());
            shader.set_uniform("u_light_color", light_color);
            shader.set_uniform("u_light_position", light_position);

            update_mvp(shader, sponza.model_matrix);
            sponza.draw(shader);

            // update_mvp(vokselia.model);
            // vokselia.draw(shader);
        }

        /* Flat Shader */ {
            const Shader& shader = shaders["flat"];
            shader.use();

            shader.set_uniform("u_color", light_color);
            shader.set_uniform("u_mvp", view_projection * translate(light_position));
            sphere.draw(shader);
        }

        glfwSwapBuffers(window);
    }
}

EventHandler& Application::get_event_handler() {
    return event_handler;
}

void Application::update_mvp(const Shader& shader, const mat4& model) const {
    shader.set_uniform("u_mvp", view_projection * model);
    shader.set_uniform("u_model", model);
    shader.set_uniform("u_normals_model_matrix", transpose_inverse(model));
}
