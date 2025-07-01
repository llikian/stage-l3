/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>
#include <glad/glad.h>
#include <ranges>
#include "debug.hpp"
#include "entities/entities.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "maths/geometry.hpp"
#include "maths/transforms.hpp"
#include "mesh/primitives.hpp"
#include "utility/Random.hpp"

Application::Application()
    : window("Projet Stage L3", this),
      event_handler(window, &camera),
      camera(vec3(0.0f, 10.0f, 0.0f), M_PI_2f, window.get_aspect_ratio(), 0.1f, 1024.0f),
      frustum(camera, window.get_aspect_ratio(), frustum_lines, frustum_faces),
      is_spying_enabled(true), spy_camera_position(30.0f), spy_camera_target(0.0f),
      spy_camera(spy_camera_position, spy_camera_target, camera.get_fov(), window.get_aspect_ratio(),
                 camera.get_near_distance(), 2.0f * camera.get_far_distance()),
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
    create_pyramid_mesh(spy_camera_mesh,
                        vec3(1.0f, 1.0f, -1.0f),
                        vec3(1.0f, -1.0f, -1.0f),
                        vec3(-1.0f, -1.0f, -1.0f),
                        1.0f);
    create_quad_mesh(screen, vec3(-1.0f, 1.0f, 1.0f), vec3(-1.0f, -1.0f, 1.0f), vec3(1.0f, -1.0f, 1.0f));
    create_axes_mesh(axes, 0.5f);

    /* ---- Framebuffer ---- */
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &spy_window_texture);
    glBindTexture(GL_TEXTURE_2D, spy_window_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.get_width(), window.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, spy_window_texture, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.get_width(), window.get_height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Couldn't create framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    /* ---- Other ---- */
    // glfwSwapInterval(0); // disable vsync
}

Application::~Application() {
    for(Shader& shader : shaders | std::views::values) { shader.free(); }

    glDeleteRenderbuffers(1, &RBO);
    glDeleteTextures(1, &spy_window_texture);
    glDeleteFramebuffers(1, &FBO);

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
    const vec3& light_color = light->color;

    /* Models */ {
        const Shader* shader = &shaders.at("blinn-phong");

        // ModelEntity* sponza = root->add_child<ModelEntity>("sponza", shader, "data/sponza/sponza.obj");
        // sponza->model.apply_model_matrix(scale(0.05f));
        // sponza->create_aabb();

        // ModelEntity* vokselia = root->add_child<ModelEntity>("vokselia", shader, "data/vokselia/vokselia_spawn.obj");
        // vokselia->model.apply_model_matrix(scale(100.0f));
        // vokselia->create_aabb();

        // ModelEntity* bmw = root->add_child<ModelEntity>("BMW", &shaders.at("blinn-phong"), "data/bmw/bmw.obj");
        // bmw->model.apply_model_matrix(scale(0.05f));
        // bmw->create_aabb();
    }

    /* Other Entities */
    TerrainEntity* terrain = root->add_child<TerrainEntity>("terrain", shaders.at("terrain"), 32.0f, 128);

    /* Frustum Culling Tests */
#ifdef DEBUG_ENABLE_FRUSTUM_TESTS
    unsigned int objects_amount = 1'000;

    Entity* test_AABBs_root = root->add_child<Entity>("Frustum Test AABBs");
    std::vector<FlatShadedMeshEntity*> test_boxes;
    test_boxes.reserve(objects_amount);

    for(unsigned int i = 0 ; i < objects_amount ; ++i) {
        test_boxes.emplace_back(test_AABBs_root->add_child<FlatShadedMeshEntity>(
            "Frustum Test AABB " + std::to_string(i),
            &shaders.at("flat")));

        create_cube_mesh(test_boxes.back()->mesh);
        test_boxes.back()->transform.set_local_position(Random::get_vec3(-400.0f, 400.0f));
        test_boxes.back()->transform.set_local_scale(Random::get_vec3(1.0f, 10.0f));
        test_boxes.back()->create_aabb();
    }
#endif

    /* Main Loop */
    while(!window.should_close()) {
        event_handler.poll_and_handle_events();
        frustum.update(camera, window.get_aspect_ratio());

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
            shader.set_uniform("u_light_color", light_color.x, light_color.y, light_color.z);
            shader.set_uniform("u_light_position", light_position);
        }

        /* Line Mesh Shader */ {
            const Shader& shader = shaders.at("line mesh");
            shader.use();

            if(are_axes_drawn) {
                shader.set_uniform("u_mvp", view_projection * translate(camera_position + 2.0f * camera_direction));
                axes.draw(shader);
            }
        }

        /* Flat Shader */ {
            const Shader& shader = shaders.at("flat");
            shader.use();

            if(is_spying_enabled) {
                shader.set_uniform("u_mvp", view_projection * spy_camera.get_model_matrix().scale(1024.0f));
                // shader.set_uniform("u_mvp", view_projection );
                shader.set_uniform("u_color", vec4(1.0f, 0.0f, 1.0f, 1.0f));
                glLineWidth(3.0f);
                spy_camera_mesh.draw(shader);
                glLineWidth(1.0f);
            }
        }

        /* Terrain Shader */
        if(terrain->get_visibility()) {
            const Shader& shader = shaders.at("terrain");
            shader.use();

            shader.set_uniform("u_frustum.near_plane.normal", frustum.near_plane.normal);
            shader.set_uniform("u_frustum.near_plane.distance", frustum.near_plane.distance);
            shader.set_uniform("u_frustum.far_plane.normal", frustum.far_plane.normal);
            shader.set_uniform("u_frustum.far_plane.distance", frustum.far_plane.distance);
            shader.set_uniform("u_frustum.top_plane.normal", frustum.top_plane.normal);
            shader.set_uniform("u_frustum.top_plane.distance", frustum.top_plane.distance);
            shader.set_uniform("u_frustum.bottom_plane.normal", frustum.bottom_plane.normal);
            shader.set_uniform("u_frustum.bottom_plane.distance", frustum.bottom_plane.distance);
            shader.set_uniform("u_frustum.left_plane.normal", frustum.left_plane.normal);
            shader.set_uniform("u_frustum.left_plane.distance", frustum.left_plane.distance);
            shader.set_uniform("u_frustum.right_plane.normal", frustum.right_plane.normal);
            shader.set_uniform("u_frustum.right_plane.distance", frustum.right_plane.distance);
        }

        scene_graph.draw(view_projection, frustum);

        if(is_spying_enabled) { draw_spy_window(); }

        draw_imgui_debug_window();
        draw_imgui_object_ediot_window();

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

void Application::draw_spy_window() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 spy_view_projection = spy_camera.get_view_projection_matrix();
    mat4 mvp = spy_view_projection * camera.get_model_matrix();

    scene_graph.draw(spy_view_projection, frustum);

    /* Lines */ {
        const Shader& shader = shaders.at("line mesh");
        shader.use();

        glLineWidth(5.0f);
        shader.set_uniform("u_mvp", mvp);
        frustum_lines.draw(shader);
        glLineWidth(1.0f);
    }

    /* Faces */ {
        static const vec4 faces_color(0.0f, 0.0f, 1.0f, 0.2f);
        const Shader& shader = shaders.at("flat");
        shader.use();

        glDisable(GL_CULL_FACE);
        shader.set_uniform("u_mvp", mvp);
        shader.set_uniform("u_color", faces_color);
        frustum_faces.draw(shader);
        glEnable(GL_CULL_FACE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /* ImGui Spy Window */ {
        static ImVec2 win_pos(0.0f, 0.0f);
        static ImVec2 win_size(0.0f, 0.0f);

        ImGui::Begin("Spy", nullptr, ImGuiWindowFlags_NoResize
                                     | ImGuiWindowFlags_NoMove
                                     | ImGuiWindowFlags_NoTitleBar);

        win_pos.x = 0.7f * window.get_width();
        win_pos.y = window.get_height() / 2.0f;
        ImGui::SetWindowPos(win_pos);

        win_size.x = window.get_width() - win_pos.x;
        win_size.y = window.get_height() - win_pos.y;
        ImGui::SetWindowSize(win_size);

        ImVec2 available_size = ImGui::GetContentRegionAvail();
        ImVec2 cursor_position = ImGui::GetCursorScreenPos();
        glViewport(0, 0, available_size.x, available_size.y);

        ImGui::GetWindowDrawList()->AddImage(
            spy_window_texture,
            cursor_position,
            ImVec2(cursor_position.x + available_size.x, cursor_position.y + available_size.y),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );

        glViewport(0, 0, window.get_width(), window.get_height());

        ImGui::End();
    }
}

void Application::draw_imgui_debug_window() {
    static ImVec2 win_pos(0.0f, 0.0f);
    static ImVec2 win_size(0.0f, 0.0f);

    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    win_size.x = 0.2f * window.get_width();
    ImGui::SetWindowPos(win_pos);
    ImGui::SetWindowSize(win_size);

    ImGui::Text("fps: %f f/s", 1.0f / event_handler.get_delta());
    ImGui::Text("delta: %fs", event_handler.get_delta());

    ImGui::NewLine();
    ImGui::Text("Total Drawable Entities: %d", DrawableEntity::total_drawable_entities);
    ImGui::Text("Total Not Hidden Entities: %d", DrawableEntity::total_not_hidden_entities);
    ImGui::Text("Total Drawn Entities: %d", DrawableEntity::total_drawn_entities);

    ImGui::NewLine();
    ImGui::Text("Camera:");
    ImGui::SliderFloat("Sensitivity", &camera.sensitivity, 0.05f, 1.0f);
    ImGui::SliderFloat("Movement Speed", &camera.movement_speed, 1.0f, 100.0f);

    ImGui::NewLine();
    ImGui::Checkbox("Is Spying Camera Enabled", &is_spying_enabled);
    if(is_spying_enabled) {
        if(ImGui::DragFloat3("Spy Camera Position", &spy_camera_position.x)) {
            spy_camera.set_position(spy_camera_position);
            spy_camera.look_at_point(spy_camera_target);
        }

        if(ImGui::DragFloat3("Spy Camera Target", &spy_camera_target.x)) {
            spy_camera.look_at_point(spy_camera_target);
        }
    }

    ImGui::NewLine();
    scene_graph.add_entity_to_imgui_node_tree(&scene_graph.root);

    ImGui::End();
}

void Application::draw_imgui_object_ediot_window() const {
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
