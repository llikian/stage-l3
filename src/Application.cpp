/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>
#include <ranges>
#include <glad/glad.h>
#include "maths/geometry.hpp"
#include "maths/mat3.hpp"
#include "maths/transforms.hpp"
#include "mesh/primitives.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Application::Application()
    : window("Projet Stage L3", this),
      camera(vec3(0.0f, 0.0f, 0.0f)),
      fov(M_PI_4), projection(perspective(fov, window.get_size_ratio(), 0.1f, 500.0f)),
      event_handler(window, &camera),
      are_axes_drawn(false) {
    /* ---- Event Handler ---- */
    event_handler.set_active_camera(&camera);
    event_handler.set_window_size_event_action([this] {
        projection(0, 0) = 1.0f / (window.get_size_ratio() * tanf(0.5f * fov));
    });
    event_handler.associate_action_to_key(GLFW_KEY_Q, false, [this] { are_axes_drawn = !are_axes_drawn; });

    /* ---- ImGui ---- */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().IniFilename = "data/imgui.ini";
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplOpenGL3_Init();

    /* ---- Shaders ---- */
    add_shader("point mesh", { "shaders/point_mesh/point_mesh.vert", "shaders/point_mesh/point_mesh.frag" });
    add_shader("line mesh", { "shaders/line_mesh/line_mesh.vert", "shaders/line_mesh/line_mesh.frag" });
    add_shader("background", { "shaders/vertex/position_only-no_mvp.vert", "shaders/fragment/background.frag" });
    add_shader("flat", { "shaders/vertex/position_only.vert", "shaders/fragment/flat.frag" });
    add_shader("blinn-phong", { "shaders/vertex/default.vert", "shaders/fragment/blinn_phong.frag" });

    /* ---- Meshes ---- */
    create_quad_mesh(screen, vec3(-1.0f, 1.0f, 0.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f));
    create_axes_mesh(axes, 0.5f);

    /* ---- Other ---- */
    glClearColor(0.1, 0.1f, 0.1f, 1.0f);
}

Application::~Application() {
    for(Shader& shader : shaders | std::views::values) { shader.free(); }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::run() {
    Model sponza("data/sponza/sponza.obj", scale(0.05f));
    // Model vokselia("data/vokselia_spawn/vokselia_spawn.obj", scale(10.0f));
    // Model bmw("data/bmw/bmw.obj", scale(0.05f));

    TriangleMesh sphere;
    create_sphere_mesh(sphere, 8, 16);

    vec3 light_color(1.0f);
    vec3 light_position(0.0f, 20.0f, 0.0f);

    while(!window.should_close()) {
        event_handler.poll_and_handle_events();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 camera_position = camera.get_position();
        vec3 camera_direction = camera.get_direction();
        view_projection = projection * camera.get_view_matrix();

        draw_background();

        /* Blinn-Phong Shader */ {
            const Shader& shader = shaders.at("blinn-phong");
            shader.use();

            shader.set_uniform("u_camera_position", camera_position);
            shader.set_uniform("u_light_color", light_color);
            shader.set_uniform("u_light_position", light_position);

            update_mvp_and_draw(shader, sponza);
            // update_mvp_and_draw(shader, vokselia);
            // update_mvp_and_draw(shader, bmw);
        }

        /* Flat Shader */ {
            const Shader& shader = shaders.at("flat");
            shader.use();

            shader.set_uniform("u_color", light_color);
            shader.set_uniform("u_mvp", view_projection * translate(light_position));
            sphere.draw(shader);
        }

        /* Line Mesh Shader */
        if(are_axes_drawn) {
            const Shader& shader = shaders.at("line mesh");
            shader.use();

            shader.set_uniform("u_mvp", view_projection * translate(camera_position + 2.0f * camera_direction));
            axes.draw(shader);
        }

        /* ImGui */ {
            ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

            ImGui::Text("fps: %f f/s", 1.0f / event_handler.get_delta());
            ImGui::Text("delta: %fs", event_handler.get_delta());

            ImGui::SetWindowSize("Debug", ImVec2(0.1f * window.get_width(), 0.0f));

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.swap_buffers();
    }
}

EventHandler& Application::get_event_handler() {
    return event_handler;
}

void Application::add_shader(const std::string& name, const std::initializer_list<std::filesystem::path>& paths_list) {
    shaders.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(paths_list, name));
}

void Application::update_mvp(const Shader& shader, const mat4& model_matrix) const {
    shader.set_uniform("u_mvp", view_projection * model_matrix);
    shader.set_uniform("u_model", model_matrix);
    shader.set_uniform("u_normals_model_matrix", transpose_inverse(model_matrix));
}

void Application::update_mvp_and_draw(const Shader& shader, Model& model) const {
    shader.set_uniform("u_mvp", view_projection * model.model_matrix);
    shader.set_uniform("u_model", model.model_matrix);
    shader.set_uniform("u_normals_model_matrix", transpose_inverse(model.model_matrix));
    model.draw(shader);
}

void Application::draw_background() {
    const Shader& shader = shaders.at("background");
    shader.use();

    shader.set_uniform("u_resolution", window.get_resolution());
    shader.set_uniform("u_camera_direction", camera.get_direction());
    shader.set_uniform("u_camera_right", camera.get_right_vector());
    shader.set_uniform("u_camera_up", camera.get_up_vector());

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    if(event_handler.is_wireframe_on()) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

    screen.draw(shader);

    if(event_handler.is_wireframe_on()) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}
