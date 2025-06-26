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
#include "maths/functions.hpp"
#include "mesh/Terrain.hpp"

Application::Application()
    : window("Projet Stage L3", this),
      event_handler(window, &camera),
      camera(vec3(0.0f, 10.0f, 0.0f), M_PI_4, window.get_size_ratio(), 0.1f, 4096.0f),
      are_axes_drawn(false) {
    /* ---- Event Handler ---- */
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
    add_shader("terrain", {
                   "shaders/terrain/terrain.vert",
                   "shaders/terrain/terrain.tesc",
                   "shaders/terrain/terrain.tese",
                   "shaders/terrain/terrain.frag"
               });

    /* ---- Meshes ---- */
    create_quad_mesh(screen, vec3(-1.0f, 1.0f, 1.0f), vec3(-1.0f, -1.0f, 1.0f), vec3(1.0f, -1.0f, 1.0f));
    create_axes_mesh(axes, 0.5f);

    /* ---- Other ---- */
    // glfwSwapInterval(0); // disable vsync
}

Application::~Application() {
    for(Shader& shader : shaders | std::views::values) { shader.free(); }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::run() {
    Entity* root = &scene_graph.root;

    /* Light */
    FlatShadedMeshEntity* light = root->add_child<FlatShadedMeshEntity>("Light", &shaders.at("flat"));
    create_sphere_mesh(light->mesh, 8, 16);
    light->transform.set_local_position(0.0f, 100.0f, 0.0f);
    const vec3& light_position = light->transform.get_local_position_reference();

    /* Models */
    // root->add_child<ModelEntity>("sponza", &shaders.at("blinn-phong"), "data/sponza/sponza.obj")
    //     ->model.apply_model_matrix(scale(0.05f));

    // root->add_child<ModelEntity>("vokselia", &shaders.at("blinn-phong"), "data/vokselia_spawn/vokselia_spawn.obj")
    //     ->model.apply_model_matrix(scale(100.0f));

    // root->add_child<ModelEntity>("BMW", &shaders.at("blinn-phong"), "data/bmw/bmw.obj")
    //     ->model.apply_model_matrix(scale(0.05f));

    /* Other Entities */
    root->add_child<TerrainEntity>("terrain", shaders.at("terrain"), 32.0f, 128)
        ->set_visibility(false);

    /* Main Loop */
    while(!window.should_close()) {
        event_handler.poll_and_handle_events();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 camera_position = camera.get_position();
        vec3 camera_direction = camera.get_direction();
        mat4 view_projection = camera.get_view_projection_matrix();

        root->update_transform_and_children();

        draw_background();

        /* Blinn-Phong Shader */ {
            const Shader& shader = shaders.at("blinn-phong");
            shader.use();

            shader.set_uniform("u_camera_position", camera_position);
            shader.set_uniform("u_light_color", light->color);
            shader.set_uniform("u_light_position", light_position);
        }

        /* Line Mesh Shader */
        if(are_axes_drawn) {
            const Shader& shader = shaders.at("line mesh");
            shader.use();

            shader.set_uniform("u_mvp", view_projection * translate(camera_position + 2.0f * camera_direction));
            axes.draw(shader);
        }

        scene_graph.draw(view_projection);

        /* ImGui Debug Window */ {
            static ImVec2 win_pos(0.0f, 0.0f);
            static ImVec2 win_size(0.0f, 0.0f);

            ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

            win_size.x = 0.2f * window.get_width();
            ImGui::SetWindowPos(win_pos);
            ImGui::SetWindowSize(win_size);

            ImGui::Text("fps: %f f/s", 1.0f / event_handler.get_delta());
            ImGui::Text("delta: %fs", event_handler.get_delta());

            ImGui::Text("Camera:");
            ImGui::SliderFloat("Sensitivity", &camera.sensitivity, 0.05f, 1.0f);
            ImGui::SliderFloat("Movement Speed", &camera.movement_speed, 1.0f, 100.0f);

            ImGui::End();

            scene_graph.draw_imgui_node_tree("Debug");
        }

        /* ImGui Object Editor Window */ {
            static ImVec2 win_pos(0.0f, 0.0f);
            static ImVec2 win_size(0.0f, 0.0f);

            ImGui::Begin("Object Editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

            win_pos.x = 0.7f * window.get_width();
            ImGui::SetWindowPos(win_pos);
            win_size.x = window.get_width() - win_pos.x;
            ImGui::SetWindowSize(win_size);

            scene_graph.add_selected_entity_editor_to_imgui_window();

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

void Application::draw_background() {
    const Shader& shader = shaders.at("background");
    shader.use();

    shader.set_uniform("u_resolution", window.get_resolution());
    shader.set_uniform("u_camera_direction", camera.get_direction());
    shader.set_uniform("u_camera_right", camera.get_right_vector());
    shader.set_uniform("u_camera_up", camera.get_up_vector());

    if(event_handler.is_wireframe_on()) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    screen.draw(shader);
    if(event_handler.is_wireframe_on()) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
}
