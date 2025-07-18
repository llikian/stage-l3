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
#include "maths/functions.hpp"
#include "maths/geometry.hpp"
#include "maths/transforms.hpp"
#include "mesh/primitives.hpp"
#include "utility/LifetimeLogger.hpp"
#include "utility/Random.hpp"

Application::Application()
    : camera(vec3(0.0f, 10.0f, 0.0f), M_PI_2f, 0.1f, 1024.0f),
      framebuffer(Window::get_width(), Window::get_height()),
      cubemap({
          "data/environments/town/px.png",
          "data/environments/town/nx.png",
          "data/environments/town/py.png",
          "data/environments/town/ny.png",
          "data/environments/town/pz.png",
          "data/environments/town/nz.png"
      }),
      are_axes_drawn(false),
      light_intensity(1.0f),
      uniform_test_conditions{true, true, true} {
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
    AssetManager::add_shader("lambert", {
                                 "shaders/vertex/position_and_normal.vert",
                                 "shaders/fragment/lambert.frag"
                             });
    AssetManager::add_shader("blinn-phong", {
                                 "shaders/vertex/default.vert",
                                 "shaders/fragment/blinn_phong.frag"
                             });
    AssetManager::add_shader("metallic-roughness", {
                                 "shaders/vertex/default.vert",
                                 "shaders/fragment/metallic_roughness.frag"
                             });
    AssetManager::add_shader("terrain", {
                                 "shaders/terrain/terrain.vert",
                                 "shaders/terrain/terrain.tesc",
                                 "shaders/terrain/terrain.tese",
                                 "shaders/terrain/terrain.frag"
                             });
    AssetManager::add_shader("post processing", {
                                 "shaders/vertex/position_only-no_mvp.vert",
                                 "shaders/fragment/post_processing.frag"
                             });

    /* Meshes */
    AssetManager::add_mesh("sphere 8 16", create_sphere_mesh, 8, 16);
    AssetManager::add_mesh("sphere 16 32", create_sphere_mesh, 16, 32);
    AssetManager::add_mesh("icosphere 0", create_icosphere_mesh, 0);
    AssetManager::add_mesh("icosphere 1", create_icosphere_mesh, 1);
    AssetManager::add_mesh("icosphere 2", create_icosphere_mesh, 2);
    AssetManager::add_mesh("cube", create_cube_mesh);
    AssetManager::add_mesh("wireframe cube", create_wireframe_cube_mesh);
    AssetManager::add_mesh("screen", create_screen_mesh);
    AssetManager::add_mesh("axes", create_axes_mesh, 0.5f);
    AssetManager::add_mesh("camera pyramid", create_pyramid_mesh,
                           vec3(1.0f, 1.0f, -1.0f), vec3(1.0f, -1.0f, -1.0f), vec3(-1.0f, -1.0f, -1.0f), 1.0f);

    /* Textures */
    AssetManager::add_texture("default", vec3(1.0f));
    AssetManager::add_texture("red", vec3(1.0f, 0.0f, 0.0f));
    AssetManager::add_texture("green", vec3(0.0f, 1.0f, 0.0f));
    AssetManager::add_texture("blue", vec3(0.0f, 0.0f, 1.0f));

    /* ---- Other ---- */
    // glfwSwapInterval(0); // disable vsync
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::run() {
    Entity* root = &scene_graph.root;

    /* Light */
    FlatShadedMeshEntity* light = root->add_child<FlatShadedMeshEntity>("Light",
                                                                        AssetManager::get_shader("flat"),
                                                                        AssetManager::get_mesh("icosphere 1"));
    light->transform.set_local_position(0.0f, 100.0f, 0.0f);
    const vec3& light_position = light->transform.get_local_position_reference();
    const vec4& light_color = light->color;

    /* Models */ {
        const Shader& shader = AssetManager::get_shader("blinn-phong");

        /* Models */
        // Model& sponza = AssetManager::add_model("sponza", "data/obj/sponza/sponza.obj");
        // sponza.apply_model_matrix(scale(0.05f));
        // ModelEntity* sponza_entity = root->add_child<ModelEntity>("sponza", shader, sponza);
        // sponza_entity->create_aabb();
        // sponza_entity->transform.set_local_position(-200.0f, -40.0f, 0.0f);

        // Model& vokselia = AssetManager::add_model("vokselia", "data/obj/vokselia/vokselia_spawn.obj");
        // vokselia.apply_model_matrix(scale(100.0f));
        // root->add_child<ModelEntity>("vokselia", shader, vokselia)->create_aabb();

        // Model& bmw = AssetManager::add_model("bmw", "data/obj/bmw/bmw.obj");
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
        Mesh& cube = AssetManager::get_mesh("cube");
        Mesh& sphere = AssetManager::get_mesh("sphere 16 32");

        for(unsigned int i = 0 ; i < 10'000 ; ++i) {
            auto entity = test_AABBs_root->add_child<FlatShadedMeshEntity>("Cube " + std::to_string(i), shader, cube);
            entity->transform.set_local_position(Random::get_vec3(-1000.0f, 1000.0f));
            entity->transform.set_local_scale(Random::get_vec3(1.0f, 10.0f));
            entity->create_aabb();

            entity = test_AABBs_root->add_child<FlatShadedMeshEntity>("Sphere " + std::to_string(i), shader, sphere);
            entity->transform.set_local_position(Random::get_vec3(-1000.0f, 1000.0f));
            entity->transform.set_local_scale(Random::get_vec3(1.0f, 10.0f));
            entity->create_aabb();
        }
    }
#endif

    // root->add_child<SceneEntity>("avocado", "data/gltf/avocado/Avocado.gltf")->transform.set_local_scale(500.0f);
    root->add_child<SceneEntity>("sponza", "data/gltf/sponza/Sponza.gltf")->transform.set_local_scale(0.2f);

    // SceneEntity* spheres = root->add_child<SceneEntity>("spheres", "data/gltf/spheres/MetalRoughSpheres.gltf");
    // spheres->transform.set_local_orientation_euler(vec3(90.0f, 0.0f, 180.0f));
    // spheres->transform.set_local_position(vec3(0.0f, 11.0f, -20.0f));
    // light->transform.set_local_position(vec3(0.0f, 0.0f, 5.0f));

    /* Main Loop */
    while(!Window::should_close()) {
        EventHandler::poll_and_handle_events();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        framebuffer.bind();
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

        /* Metallic-Roughness Shader */ {
            const Shader& shader = AssetManager::get_shader("metallic-roughness");
            shader.use();

            shader.set_uniform("u_camera_position", camera_position);
            shader.set_uniform("u_light.color", light_color.x, light_color.y, light_color.z);
            shader.set_uniform("u_light.position", light_position);
            shader.set_uniform("u_light.intensity", light_intensity);
            shader.set_uniform("u_material.base_color_map", 0);
            shader.set_uniform("u_material.metallic_roughness_map", 1);
            shader.set_uniform_if_exists("u_test1", uniform_test_conditions[0]);
            shader.set_uniform_if_exists("u_test2", uniform_test_conditions[1]);
            shader.set_uniform_if_exists("u_test3", uniform_test_conditions[2]);
            // shader.set_uniform("u_cubemap", 2);
            // cubemap.bind(2);
        }

        /* Line Mesh Shader */ {
            const Shader& shader = AssetManager::get_shader("line mesh");
            shader.use();

            if(are_axes_drawn) {
                shader.set_uniform("u_mvp", frustum.view_projection
                                            * translate(camera_position + 2.0f * camera_direction));
                AssetManager::get_mesh("axes").draw();
            }
        }

        /* Lambert Shader */ {
            const Shader& shader = AssetManager::get_shader("lambert");
            shader.use();
            shader.set_uniform("u_light_color", light_color.x, light_color.y, light_color.z);
            shader.set_uniform("u_light_position", light_position);
        }

        /* Terrain Shader */
        if(terrain->get_visibility()) {
            const Shader& shader = AssetManager::get_shader("terrain");
            shader.use();
            shader.set_uniform("u_frustum_view_projection_matrix", frustum.view_projection);
        }

        scene_graph.draw(frustum.view_projection, frustum);

        Framebuffer::bind_default();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_post_processing();

        draw_imgui_debug_window();
        draw_imgui_object_ediot_window();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        Window::swap_buffers();
    }
}

void Application::draw_post_processing() const {
    const Shader& shader = AssetManager::get_shader("post processing");
    shader.use();
    shader.set_uniform("u_texture", 0);
    shader.set_uniform("u_texture_resolution", framebuffer.get_resolution());
    shader.set_uniform_if_exists("u_resolution", Window::get_resolution());
    shader.set_uniform_if_exists("u_test1", uniform_test_conditions[0]);
    shader.set_uniform_if_exists("u_test2", uniform_test_conditions[1]);
    shader.set_uniform_if_exists("u_test3", uniform_test_conditions[2]);
    framebuffer.bind_texture(0);

    if(EventHandler::is_wireframe_enabled()) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    AssetManager::get_mesh("screen").draw();
    if(EventHandler::is_wireframe_enabled()) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
}

void Application::draw_background() const {
    const Shader& shader = AssetManager::get_shader("background");
    shader.use();

    shader.set_uniform("u_resolution", Window::get_resolution());
    shader.set_uniform("u_camera_direction", camera.get_direction());
    shader.set_uniform("u_camera_right", camera.get_right_vector());
    shader.set_uniform("u_camera_up", camera.get_up_vector());

    if(EventHandler::is_wireframe_enabled()) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    AssetManager::get_mesh("screen").draw();
    if(EventHandler::is_wireframe_enabled()) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
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
    ImGui::DragFloat("Light Intensity", &light_intensity, 0.25f, 1.0f, 100.0f);
    ImGui::Checkbox("Uniform Test Condition 1", &uniform_test_conditions[0]);
    ImGui::Checkbox("Uniform Test Condition 2", &uniform_test_conditions[1]);
    ImGui::Checkbox("Uniform Test Condition 3", &uniform_test_conditions[2]);

    ImGui::NewLine();
    ImGui::Text("Camera:");
    ImGui::SliderFloat("Sensitivity", &camera.sensitivity, 0.05f, 1.0f);
    ImGui::SliderFloat("Movement Speed", &camera.movement_speed, 1.0f, 100.0f);

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
