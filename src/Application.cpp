/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>
#include <glad/glad.h>
#include "AssetManager.hpp"
#include "debug.hpp"
#include "entities/entities.hpp"
#include "EventHandler.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "maths/geometry.hpp"
#include "maths/transforms.hpp"
#include "mesh/primitives.hpp"
#include "utility/Random.hpp"

Application::Application()
    : camera(vec3(0.0f, 10.0f, 0.0f), M_PI_2f, 0.1f, 1024.0f),
      is_spying_enabled(true),
      spy_camera_position(30.0f), spy_camera_target(0.0f),
      spy_camera(spy_camera_position, spy_camera_target, camera.get_fov(), camera.get_near_distance(),
                 2.0f * camera.get_far_distance()),
      are_axes_drawn(false) {
    /* ---- Event Handler ---- */
    EventHandler::set_active_camera(&camera);
    EventHandler::get().associate_action_to_key(GLFW_KEY_Q, false, [this] { are_axes_drawn = !are_axes_drawn; });

    /* ---- ImGui ---- */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().IniFilename = "data/imgui.ini";
    ImGui_ImplGlfw_InitForOpenGL(Window::get_glfw(), true);
    ImGui_ImplOpenGL3_Init();

    /* ---- Asset Manager ---- */
    /* Shaders */
    AssetManager::add_shader("point mesh", {
                                 "shaders/point_mesh/point_mesh.vert",
                                 "shaders/point_mesh/point_mesh.frag"
                             });
    AssetManager::add_shader("line mesh", {
                                 "shaders/line_mesh/line_mesh.vert",
                                 "shaders/line_mesh/line_mesh.frag"
                             });
    AssetManager::add_shader("background", {
                                 "shaders/vertex/position_only-no_mvp.vert",
                                 "shaders/fragment/background.frag"
                             });
    AssetManager::add_shader("flat", {
                                 "shaders/vertex/position_only.vert",
                                 "shaders/fragment/flat.frag"
                             });
    AssetManager::add_shader("blinn-phong", {
                                 "shaders/vertex/default.vert",
                                 "shaders/fragment/blinn_phong.frag"
                             });
    AssetManager::add_shader("terrain", {
                                 "shaders/terrain/terrain.vert",
                                 "shaders/terrain/terrain.tesc",
                                 "shaders/terrain/terrain.tese",
                                 "shaders/terrain/terrain.frag"
                             });

    /* Meshes */
    AssetManager::add_triangle_mesh("sphere 8 16", create_sphere_mesh, 8, 16);
    AssetManager::add_triangle_mesh("sphere 16 32", create_sphere_mesh, 16, 32);
    AssetManager::add_triangle_mesh("cube", create_cube_mesh);
    AssetManager::add_line_mesh("wireframe cube", create_wireframe_cube_mesh);
    AssetManager::add_triangle_mesh("screen", create_quad_mesh,
                                    vec3(-1.0f, 1.0f, 1.0f), vec3(-1.0f, -1.0f, 1.0f), vec3(1.0f, -1.0f, 1.0f));
    AssetManager::add_line_mesh("axes", create_axes_mesh, 0.5f);
    AssetManager::add_line_mesh("camera pyramid", create_pyramid_mesh,
                                vec3(1.0f, 1.0f, -1.0f), vec3(1.0f, -1.0f, -1.0f), vec3(-1.0f, -1.0f, -1.0f), 1.0f);
    AssetManager::add_triangle_and_line_mesh("frustum", create_frustum_meshes, camera);

    /* ---- Framebuffer ---- */
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &spy_window_texture);
    glBindTexture(GL_TEXTURE_2D, spy_window_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::get_width(), Window::get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, spy_window_texture, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::get_width(), Window::get_height());
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
    FlatShadedMeshEntity* light = root->add_child<FlatShadedMeshEntity>("Light",
                                                                        AssetManager::get_shader("flat"),
                                                                        AssetManager::get_triangle_mesh("sphere 8 16"));
    light->transform.set_local_position(0.0f, 100.0f, 0.0f);
    const vec3& light_position = light->transform.get_local_position_reference();
    const vec3& light_color = light->color;

    /* Models */ {
        const Shader& shader = AssetManager::get_shader("blinn-phong");

        /* Models */
        Model& sponza = AssetManager::add_model("sponza", "data/sponza/sponza.obj");
        sponza.apply_model_matrix(scale(0.05f));
        ModelEntity* sponza_entity = root->add_child<ModelEntity>("sponza", shader, sponza);
        sponza_entity->create_aabb();
        sponza_entity->transform.set_local_position(-200.0f, -40.0f, 0.0f);

        // Model& vokselia = AssetManager::add_model("vokselia", "data/vokselia/vokselia_spawn.obj");
        // vokselia.apply_model_matrix(scale(100.0f));
        // root->add_child<ModelEntity>("vokselia", shader, vokselia)->create_aabb();

        // Model& bmw = AssetManager::add_model("bmw", "data/bmw/bmw.obj");
        // bmw.apply_model_matrix(scale(0.05f));
        // root->add_child<ModelEntity>("bmw", shader, bmw)->create_aabb();
    }

    /* Other Entities */
    TerrainEntity* terrain = root->add_child<TerrainEntity>("terrain", AssetManager::get_shader("terrain"), 32.0f, 128);
    terrain->set_visibility(false);

#ifdef DEBUG_ENABLE_FRUSTUM_TESTS
    Entity* test_AABBs_root = root->add_child<Entity>("Test Cubes");
    /* Frustum Culling Tests */ {
        const Shader& shader = AssetManager::get_shader("flat");
        TriangleMesh& mesh = AssetManager::get_triangle_mesh("cube");

        for(unsigned int i = 0 ; i < 10'000 ; ++i) {
            auto entity = test_AABBs_root->add_child<FlatShadedMeshEntity>("Cube " + std::to_string(i), shader, mesh);
            entity->transform.set_local_position(Random::get_vec3(-1000.0f, 1000.0f));
            entity->transform.set_local_scale(Random::get_vec3(1.0f, 10.0f));
            entity->create_aabb();
        }
    }
#endif

    root->add_child<SceneEntity>("avocado", "data/avocado/Avocado.gltf");

    /* Main Loop */
    while(!Window::should_close()) {
        EventHandler::poll_and_handle_events();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 camera_position = camera.get_position();
        vec3 camera_direction = camera.get_direction();
        frustum.view_projection = camera.get_view_projection_matrix();

        // test_AABBs_root->transform.set_local_orientation(0.0f, 10.0f * EventHandler::get_time(), 0.0f);
        root->update_transform_and_children();

        draw_background();

        /* Blinn-Phong Shader */ {
            const Shader& shader = AssetManager::get_shader("blinn-phong");
            shader.use();

            shader.set_uniform("u_camera_position", camera_position);
            shader.set_uniform("u_light_color", light_color.x, light_color.y, light_color.z);
            shader.set_uniform("u_light_position", light_position);
        }

        /* Line Mesh Shader */ {
            const Shader& shader = AssetManager::get_shader("line mesh");
            shader.use();

            if(are_axes_drawn) {
                shader.set_uniform("u_mvp", frustum.view_projection
                                            * translate(camera_position + 2.0f * camera_direction));
                AssetManager::get_line_mesh("axes").draw(shader);
            }
        }

        /* Flat Shader */ {
            const Shader& shader = AssetManager::get_shader("flat");
            shader.use();

            if(is_spying_enabled) {
                shader.set_uniform("u_mvp", frustum.view_projection * spy_camera.get_model_matrix().scale(1024.0f));
                shader.set_uniform("u_color", vec4(1.0f, 0.0f, 1.0f, 1.0f));
                glLineWidth(3.0f);
                AssetManager::get_line_mesh("camera pyramid").draw(shader);
                glLineWidth(1.0f);
            }
        }

        /* Terrain Shader */
        if(terrain->get_visibility()) {
            const Shader& shader = AssetManager::get_shader("terrain");
            shader.use();
            shader.set_uniform("u_frustum_view_projection_matrix", frustum.view_projection);
        }

        scene_graph.draw(frustum.view_projection, frustum);

        if(is_spying_enabled) { draw_spy_window(); }

        draw_imgui_debug_window();
        draw_imgui_object_ediot_window();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        Window::swap_buffers();
    }
}

void Application::draw_background() {
    const Shader& shader = AssetManager::get_shader("background");
    shader.use();

    shader.set_uniform("u_resolution", Window::get_resolution());
    shader.set_uniform("u_camera_direction", camera.get_direction());
    shader.set_uniform("u_camera_right", camera.get_right_vector());
    shader.set_uniform("u_camera_up", camera.get_up_vector());

    if(EventHandler::is_wireframe_on()) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    AssetManager::get_triangle_mesh("screen").draw(shader);
    if(EventHandler::is_wireframe_on()) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
}

void Application::draw_spy_window() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 spy_view_projection = spy_camera.get_view_projection_matrix();
    mat4 mvp = spy_view_projection * camera.get_model_matrix();

    scene_graph.draw(spy_view_projection, frustum);

    /* Lines */ {
        const Shader& shader = AssetManager::get_shader("line mesh");
        shader.use();

        glLineWidth(5.0f);
        shader.set_uniform("u_mvp", mvp);
        AssetManager::get_line_mesh("frustum").draw(shader);
        glLineWidth(1.0f);
    }

    /* Faces */ {
        static const vec4 faces_color(0.0f, 0.0f, 1.0f, 0.2f);
        const Shader& shader = AssetManager::get_shader("flat");
        shader.use();

        glDisable(GL_CULL_FACE);
        shader.set_uniform("u_mvp", mvp);
        shader.set_uniform("u_color", faces_color);
        AssetManager::get_triangle_mesh("frustum").draw(shader);
        glEnable(GL_CULL_FACE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /* ImGui Spy Window */ {
        static ImVec2 win_pos(0.0f, 0.0f);
        static ImVec2 win_size(0.0f, 0.0f);

        ImGui::Begin("Spy", nullptr, ImGuiWindowFlags_NoResize
                                     | ImGuiWindowFlags_NoMove
                                     | ImGuiWindowFlags_NoTitleBar);

        win_pos.x = 0.7f * Window::get_width();
        win_pos.y = Window::get_height() / 2.0f;
        ImGui::SetWindowPos(win_pos);

        win_size.x = Window::get_width() - win_pos.x;
        win_size.y = Window::get_height() - win_pos.y;
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

        glViewport(0, 0, Window::get_width(), Window::get_height());

        ImGui::End();
    }
}

void Application::draw_imgui_debug_window() {
    static ImVec2 win_pos(0.0f, 0.0f);
    static ImVec2 win_size(0.0f, 0.0f);

    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    win_size.x = 0.2f * Window::get_width();
    ImGui::SetWindowPos(win_pos);
    ImGui::SetWindowSize(win_size);

    ImGui::Text("fps: %f f/s", 1.0f / EventHandler::get_delta());
    ImGui::Text("delta: %fs", EventHandler::get_delta());

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

        if(ImGui::Button("Move Spy to Camera")) { spy_camera.set_position(camera.get_position()); }
        if(ImGui::Button("Make Spy Look at Camera")) { spy_camera.look_at_point(camera.get_position()); }
    }

    ImGui::NewLine();
    scene_graph.add_imgui_node_tree();

    ImGui::End();
}

void Application::draw_imgui_object_ediot_window() const {
    static ImVec2 win_pos(0.0f, 0.0f);
    static ImVec2 win_size(0.0f, 0.0f);

    ImGui::Begin("Object Editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    win_pos.x = 0.7f * Window::get_width();
    ImGui::SetWindowPos(win_pos);
    win_size.x = Window::get_width() - win_pos.x;
    ImGui::SetWindowSize(win_size);

    scene_graph.add_selected_entity_editor_to_imgui_window();

    ImGui::End();
}
