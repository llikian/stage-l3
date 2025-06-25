/***************************************************************************************************
 * @file  Entity.cpp
 * @brief Implementation of the Entity class
 **************************************************************************************************/

#include "scene_graph/Entity.hpp"

#include "imgui.h"

Entity::Entity(const std::string& name) : name(name), parent(nullptr), is_hidden(false) { }

Entity::~Entity() {
    for(const Entity* child : children) { delete child; }
}

void Entity::set_visibility(bool is_hidden) {
    this->is_hidden = is_hidden;
    for(Entity* child : children) { child->set_visibility(is_hidden); }
}

void Entity::toggle_visibility() {
    is_hidden = !is_hidden;
    for(Entity* child : children) { child->set_visibility(is_hidden); }
}

void Entity::update_transform_and_children() {
    if(transform.is_local_model_dirty()) {
        force_update_transform_and_children();
    } else {
        for(Entity* child : children) { child->update_transform_and_children(); }
    }
}

void Entity::force_update_transform_and_children() {
    if(parent != nullptr) {
        transform.update_global_model(parent->transform.get_global_model());
    } else {
        transform.update_global_model();
    }

    for(Entity* child : children) { child->force_update_transform_and_children(); }
}

void Entity::draw_drawables(const mat4& view_projection_matrix) {
    if(is_drawable()) { static_cast<DrawableEntity*>(this)->draw(view_projection_matrix); }
    for(Entity* child : children) { child->draw_drawables(view_projection_matrix); }
}

bool Entity::is_drawable() const {
    return false;
}

void Entity::add_to_object_editor() {
    ImGui::Text("Selected Entity: '%s'", name.c_str());

    if(ImGui::Checkbox("Is Object Hidden", &is_hidden)) {
        for(Entity* child : children) { child->set_visibility(is_hidden); }
    }

    bool is_dirty = ImGui::DragFloat3("Local Position", &transform.get_local_position_reference().x);
    is_dirty = is_dirty || ImGui::DragFloat3("Local Orientation", &transform.get_local_orientation_reference().x,
                                             1.0f, 0.0f, 360.0f, "%.3f", ImGuiSliderFlags_WrapAround);
    is_dirty = is_dirty || ImGui::DragFloat3("Local Scale", &transform.get_local_scale_reference().x, 0.1f, 0.1f);

    if(is_dirty) { transform.set_local_model_to_dirty(); }
}

DrawableEntity::DrawableEntity(const std::string& name, Shader* shader)
    : Entity(name), shader(shader) { }

void DrawableEntity::update_uniforms(const mat4& view_projection_matrix) {
    int u_mvp_location = shader->get_uniform_location("u_mvp");
    if(u_mvp_location != -1) {
        Shader::set_uniform(u_mvp_location, view_projection_matrix * transform.get_global_model());
    }

    const mat4& global_model = transform.get_global_model_reference();
    shader->set_uniform_if_exists("u_model", global_model);

    int u_normals_model_matrix_location = shader->get_uniform_location("u_normals_model_matrix");
    if(u_normals_model_matrix_location != -1) {
        Shader::set_uniform(u_normals_model_matrix_location, transpose_inverse(global_model));
    }
}

bool DrawableEntity::is_drawable() const {
    return true;
}

ModelEntity::ModelEntity(const std::string& name, Shader* shader, const std::filesystem::path& path)
    : DrawableEntity(name, shader), model(path) { }

void ModelEntity::draw(const mat4& view_projection_matrix) {
    if(is_hidden) { return; }

    if(shader != nullptr) {
        shader->use();
        update_uniforms(view_projection_matrix);
        model.draw(*shader);
    } else {
        std::cout << "[WARNING] ModelEntity '" << name << "' with nullptr shader.\n";
    }
}

void ModelEntity::add_to_object_editor() {
    DrawableEntity::add_to_object_editor();

    static Material* selected = nullptr;

    ImGui::BeginTable("Materials", 1, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("Materials");
    ImGui::TableHeadersRow();
    for(Material& material : model.materials) {
        ImGui::TableNextColumn();
        if(ImGui::Selectable(material.name.c_str(), &material == selected)) { selected = &material; }
    }
    ImGui::EndTable();

    if(selected != nullptr) {
        ImGui::Text("Material: %s", selected->name.c_str());
        ImGui::ColorEdit3("Ambient Color", &selected->ambient.x);
        ImGui::ColorEdit3("Diffuse Color", &selected->diffuse.x);
        ImGui::ColorEdit3("Specular Color", &selected->specular.x);
        ImGui::DragFloat("Specular Exponent", &selected->specular_exponent);
    }
}

TriangleMeshEntity::TriangleMeshEntity(const std::string& name, Shader* shader)
    : DrawableEntity(name, shader) { }

void TriangleMeshEntity::draw(const mat4& view_projection_matrix) {
    if(is_hidden) { return; }

    if(shader != nullptr) {
        shader->use();
        update_uniforms(view_projection_matrix);
        mesh.draw(*shader);
    } else {
        std::cout << "[WARNING] TriangleMeshEntity '" << name << "' with nullptr shader.\n";
    }
}

void TriangleMeshEntity::add_to_object_editor() {
    DrawableEntity::add_to_object_editor();
    if(mesh.material != nullptr) {
        ImGui::Text("Material: %s", mesh.material->name.c_str());
        ImGui::ColorEdit3("Ambient Color", &mesh.material->ambient.x);
        ImGui::ColorEdit3("Diffuse Color", &mesh.material->diffuse.x);
        ImGui::ColorEdit3("Specular Color", &mesh.material->specular.x);
        ImGui::DragFloat("Specular Exponent", &mesh.material->specular_exponent);
    } else {
        ImGui::Text("Mesh doesn't have a material.");
    }
}

FlatShadedMeshEntity::FlatShadedMeshEntity(const std::string& name, Shader* shader, const vec3& color)
    : TriangleMeshEntity(name, shader), color(color) { }

void FlatShadedMeshEntity::update_uniforms(const mat4& view_projection_matrix) {
    TriangleMeshEntity::update_uniforms(view_projection_matrix);
    shader->set_uniform_if_exists("u_color", color);
}

void FlatShadedMeshEntity::add_to_object_editor() {
    TriangleMeshEntity::add_to_object_editor();
    ImGui::ColorEdit3("Object color", &color.x);
}
