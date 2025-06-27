/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>
#include <ranges>
#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "culling/BoundingVolume.hpp"
#include "maths/geometry.hpp"
#include "maths/mat3.hpp"
#include "maths/transforms.hpp"
#include "mesh/primitives.hpp"
#include "utility/Random.hpp"

Application::Application()
    : window("Projet Stage L3", this),
      event_handler(window, &camera),
      camera(vec3(0.0f, 10.0f, 0.0f), M_PI_2f, window.get_size_ratio(), 0.1f, 4096.0f),
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
        ->set_visibility(true);

    /* SphereVolume Test */
    Entity* test_spheres_root = root->add_child<Entity>("Frustum Test Spheres");
    unsigned int spheres_amount = 100;
    std::vector<FlatShadedMeshEntity*> test_spheres;
    std::vector<SphereVolume> test_sphere_volumes(spheres_amount, SphereVolume(vec3(0.0f), 1.0f));
    test_spheres.reserve(spheres_amount);

    for(unsigned int i = 0 ; i < spheres_amount ; ++i) {
        test_spheres.emplace_back(test_spheres_root->add_child<FlatShadedMeshEntity>(
            "Frustum Test Sphere " + std::to_string(i),
            &shaders.at("flat")));

        create_sphere_mesh(test_spheres.back()->mesh, 16, 32);
        test_spheres.back()->transform.set_local_position(
            Random::get_vec3(vec3(-200.0f, -50.0f, -300.0f), vec3(200.0f, 50.0f, 0.0f)));
        test_spheres.back()->transform.set_local_scale(Random::get_float(1.0f, 5.0f));
    }

    /* AABB Test */
    Entity* test_AABBs_root = root->add_child<Entity>("Frustum Test AABBs");
    unsigned int boxes_amount = 100;
    std::vector<FlatShadedMeshEntity*> test_boxes;
    std::vector<AABB> test_AABBs(boxes_amount, AABB(vec3(0.0f), vec3(0.0f)));
    test_boxes.reserve(boxes_amount);

    for(unsigned int i = 0 ; i < boxes_amount ; ++i) {
        test_boxes.emplace_back(test_AABBs_root->add_child<FlatShadedMeshEntity>(
            "Frustum Test AABB " + std::to_string(i),
            &shaders.at("flat")));

        create_cube_mesh(test_boxes.back()->mesh);
        test_boxes.back()->transform.set_local_position(
            Random::get_vec3(vec3(-200.0f, -50.0f, -300.0f), vec3(200.0f, 50.0f, 0.0f)));
        test_boxes.back()->transform.set_local_scale(Random::get_vec3(1.0f, 5.0f));
    }

    /* Frustum */
    // LineMesh frustum_mesh;
    // Frustum frustum(Camera(vec3(0.0f), M_PI_4f, window.get_size_ratio(), 10.0f, 200.0f),
    //                 window.get_size_ratio(), frustum_mesh);

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

        // draw_background();

        /* Blinn-Phong Shader */ {
            const Shader& shader = shaders.at("blinn-phong");
            shader.use();

            shader.set_uniform("u_camera_position", camera_position);
            shader.set_uniform("u_light_color", light->color);
            shader.set_uniform("u_light_position", light_position);
        }

        LineMesh frustum_mesh;
        Frustum frustum(camera, window.get_size_ratio(), frustum_mesh, true);

        /* Line Mesh Shader */ {
            const Shader& shader = shaders.at("line mesh");
            shader.use();

            if(are_axes_drawn) {
                shader.set_uniform("u_mvp", view_projection * translate(camera_position + 2.0f * camera_direction));
                axes.draw(shader);
            }

            shader.set_uniform("u_mvp", view_projection);
            frustum_mesh.draw(shader);
        }

        scene_graph.draw(view_projection);

        /* Frustum Tests */
        // for(unsigned int i = 0 ; i < spheres_amount ; ++i) {
        //     if(test_sphere_volumes[i].is_in_frustum(frustum, test_spheres[i]->transform)) {
        //         test_spheres[i]->color = vec3(0.0f, 1.0f, 0.0f);
        //     } else {
        //         test_spheres[i]->color = vec3(1.0f, 0.0f, 0.0f);
        //     }
        // }

        for(unsigned int i = 0 ; i < boxes_amount ; ++i) {
            if(test_AABBs[i].is_in_frustum(frustum, test_boxes[i]->transform)) {
                test_boxes[i]->color = vec3(0.0f, 1.0f, 0.0f);
            } else {
                test_boxes[i]->color = vec3(1.0f, 0.0f, 0.0f);
            }
        }

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
