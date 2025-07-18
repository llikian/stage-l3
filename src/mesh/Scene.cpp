/***************************************************************************************************
 * @file  Scene.cpp
 * @brief Implementation of the Scene class
 **************************************************************************************************/

#include "mesh/Scene.hpp"

#include <ranges>

#include "AssetManager.hpp"
#include "debug.hpp"
#include "maths/functions.hpp"
#include "utility/LifetimeLogger.hpp"

MeshInfo::MeshInfo() : material(nullptr) { }

MeshInfo::~MeshInfo() {
    if(material != nullptr) {
        material->base_color_map.free();
        material->metallic_roughness_map.free();
    }
    delete material;
}

Scene::Scene(const std::filesystem::path& path)
    : meshes(nullptr), meshes_count(0), primitives_count(nullptr) {
    load(path);
}

Scene::~Scene() {
    if(meshes != nullptr) {
        for(unsigned int i = 0 ; i < meshes_count ; ++i) { delete[] meshes[i]; }
    }

    delete[] meshes;
    delete[] primitives_count;
}

void Scene::draw(const mat4& view_projection_matrix, const Transform& transform) const {
    for(const auto& [mesh_id, primitive_id] : indices_order) {
        const MRMaterial* material = meshes[mesh_id][primitive_id].material;
        const Shader& shader = material == nullptr
                                   ? AssetManager::get_relevant_shader_from_mesh(meshes[mesh_id][primitive_id].mesh)
                                   : AssetManager::get_shader("metallic-roughness");
        shader.use();

        const mat4& global_model = transform.get_global_model_const_reference();
        shader.set_uniform_if_exists("u_model", global_model);

        int u_mvp_location = shader.get_uniform_location("u_mvp");
        if(u_mvp_location != -1) {
            Shader::set_uniform(u_mvp_location, view_projection_matrix * global_model);
        }

        int u_normals_model_matrix_location = shader.get_uniform_location("u_normals_model_matrix");
        if(u_normals_model_matrix_location != -1) {
            Shader::set_uniform(u_normals_model_matrix_location, transpose_inverse(global_model));
        }

        shader.set_uniform_if_exists("u_color", vec4(1.0f, 0.0f, 1.0f, 1.0f));

        if(material != nullptr) { // mettalic roughness
            material->base_color_map.bind(0);
            material->metallic_roughness_map.bind(1);

            shader.set_uniform_if_exists("u_material.base_color", material->base_color);
            shader.set_uniform_if_exists("u_material.metallic", material->metallic);
            shader.set_uniform_if_exists("u_material.roughness", material->roughness);
            shader.set_uniform_if_exists("u_material.reflectance", material->reflectance);
        } else { // blinn phong
            shader.set_uniform_if_exists("u_ambient", vec3(1.0f));
            shader.set_uniform_if_exists("u_diffuse", vec3(1.0f));
            shader.set_uniform_if_exists("u_specular", vec3(1.0f));
            shader.set_uniform_if_exists("u_specular_exponent", 10.0f);
            int u_diffuse_map_location = shader.get_uniform_location("u_diffuse_map");
            if(u_diffuse_map_location != -1) {
                AssetManager::get_texture("default").bind(0);
            }
        }

        meshes[mesh_id][primitive_id].mesh.draw();
    }
}

void Scene::check_cgltf_result(cgltf_result result, const std::string& error_message) {
    switch(result) {
        case cgltf_result_data_too_short: throw std::runtime_error(error_message + "data_too_short.");
        case cgltf_result_unknown_format: throw std::runtime_error(error_message + "unknown_format.");
        case cgltf_result_invalid_json: throw std::runtime_error(error_message + "invalid_json.");
        case cgltf_result_invalid_gltf: throw std::runtime_error(error_message + "invalid_gltf.");
        case cgltf_result_invalid_options: throw std::runtime_error(error_message + "invalid_options.");
        case cgltf_result_file_not_found: throw std::runtime_error(error_message + "file_not_found.");
        case cgltf_result_io_error: throw std::runtime_error(error_message + "io_error.");
        case cgltf_result_out_of_memory: throw std::runtime_error(error_message + "out_of_memory.");
        case cgltf_result_legacy_gltf: throw std::runtime_error(error_message + "legacy_gltf.");
        case cgltf_result_max_enum: throw std::runtime_error(error_message + "max_enum.");
        default: break;
    }
}

std::string Scene::cgltf_primitive_type_to_string(cgltf_primitive_type primitive_type) {
    switch(primitive_type) {
        case cgltf_primitive_type_points: return "points";
        case cgltf_primitive_type_lines: return "lines";
        case cgltf_primitive_type_line_loop: return "line_loop";
        case cgltf_primitive_type_line_strip: return "line_strip";
        case cgltf_primitive_type_triangles: return "triangles";
        case cgltf_primitive_type_triangle_strip: return "triangle_strip";
        case cgltf_primitive_type_triangle_fan: return "triangle_fan";
        default: return "invalid";
    }
}

std::string Scene::cgltf_attribute_type_to_string(cgltf_attribute_type attribute_type) {
    switch(attribute_type) {
        case cgltf_attribute_type_position: return "position";
        case cgltf_attribute_type_normal: return "normal";
        case cgltf_attribute_type_tangent: return "tangent";
        case cgltf_attribute_type_texcoord: return "texcoord";
        case cgltf_attribute_type_color: return "color";
        case cgltf_attribute_type_joints: return "joints";
        case cgltf_attribute_type_weights: return "weights";
        case cgltf_attribute_type_custom: return "custom";
        default: return "invalid";
    }
}

std::string Scene::cgltf_type_to_string(cgltf_type type) {
    switch(type) {
        case cgltf_type_scalar: return "scalar";
        case cgltf_type_vec2: return "vec2";
        case cgltf_type_vec3: return "vec3";
        case cgltf_type_vec4: return "vec4";
        case cgltf_type_mat2: return "mat2";
        case cgltf_type_mat3: return "mat3";
        case cgltf_type_mat4: return "mat4";
        default: return "invalid";
    }
}

void Scene::load(const std::filesystem::path& path) {
#ifdef DEBUG_LOG_GLTF_READ_INFO
    LifetimeLogger lifetime_logger("\tTook ");
    std::cout << "Reading scene from file '" << path.filename().string() << "':\n";
#endif

    const std::filesystem::path parent_path = path.parent_path();

    cgltf_options options{};
    cgltf_data* data;

    cgltf_result result = cgltf_parse_file(&options, path.c_str(), &data);
    check_cgltf_result(result, "Failed to read gltf file '" + path.string() + "': ");

    result = cgltf_load_buffers(&options, data, path.c_str());
    check_cgltf_result(result, "Failed to load buffers");

    result = cgltf_validate(data);
    check_cgltf_result(result, "Failed to validate data.");

    meshes = new MeshInfo*[data->meshes_count];
    meshes_count = data->meshes_count;
    primitives_count = new unsigned int[data->meshes_count];

    std::vector<vector2<unsigned int>> transparent_indices_order;

    for(unsigned int i = 0 ; i < data->meshes_count ; ++i) {
        const cgltf_mesh& c_mesh = data->meshes[i];

        meshes[i] = new MeshInfo[c_mesh.primitives_count];
        primitives_count[i] = c_mesh.primitives_count;

        for(unsigned int j = 0 ; j < c_mesh.primitives_count ; ++j) {
            const cgltf_primitive& c_primitive = c_mesh.primitives[j];

            MRMaterial*& material = meshes[i][j].material;

            if(c_primitive.material != nullptr) {
                const cgltf_material* c_material = c_primitive.material;

                if(c_primitive.material->has_pbr_metallic_roughness) {
                    std::cout << "\tHas metallic roughness.\n";
                    const auto& [base_color_texture,
                        metallic_roughness_texture,
                        base_color_factor,
                        metallic_factor,
                        roughness_factor] = c_material->pbr_metallic_roughness;

                    material = new MRMaterial;
                    material->base_color.x = base_color_factor[0];
                    material->base_color.y = base_color_factor[1];
                    material->base_color.z = base_color_factor[2];
                    material->base_color.w = base_color_factor[3];
                    material->metallic = metallic_factor;
                    material->roughness = roughness_factor;

                    if(base_color_texture.texture != nullptr) {
                        material->base_color_map.create(parent_path, base_color_texture, true);
                    } else {
                        material->base_color_map.create(255, 255, 255);
                    }

                    if(metallic_roughness_texture.texture != nullptr) {
                        material->metallic_roughness_map.create(parent_path, metallic_roughness_texture, false);
                    } else {
                        material->metallic_roughness_map.create(vec3(0.0f, 0.5f, 0.0f));
                    }

                    if(c_material->has_ior) {
                        material->reflectance = (c_material->ior.ior - 1.0f) / (c_material->ior.ior + 1.0f) / 0.4f;
                    }
                }

                if(c_primitive.material->has_pbr_specular_glossiness) { std::cout << "\tHas specular glossiness.\n"; }
            }

            if(material != nullptr && material->has_transparency()) {
                transparent_indices_order.emplace_back(i, j);
            } else {
                indices_order.emplace_back(i, j);
            }

            Mesh& mesh = meshes[i][j].mesh;

            switch(c_primitive.type) {
                case cgltf_primitive_type_points:
                    mesh.set_primitive(Primitive::POINTS);
                    break;
                case cgltf_primitive_type_lines:
                    mesh.set_primitive(Primitive::LINES);
                    break;
                case cgltf_primitive_type_triangles:
                    mesh.set_primitive(Primitive::TRIANGLES);
                    break;
                default:
                    throw std::runtime_error("Unhandled or invalid primitive: "
                                             + cgltf_primitive_type_to_string(c_primitive.type)
                                             + ' ');
            }

            if(c_primitive.indices != nullptr) {
                std::vector<unsigned int> indices;
                unsigned int indices_count = c_primitive.indices->count;
                indices.resize(indices_count);
                cgltf_accessor_unpack_indices(c_primitive.indices, indices.data(), sizeof(unsigned int), indices_count);
                mesh.push_indices_buffer(indices);

#ifdef DEBUG_LOG_GLTF_READ_INFO
                std::cout << "\tINDICES: " << indices.size() << '\n';
#endif
            }

            AttributeInfo* attributes = new AttributeInfo[c_primitive.attributes_count];

            // Used to push the vertex attributes int he right order. (i.e. position then normal then tex_coords...)
            std::vector<unsigned int> attributes_indices(ATTRIBUTE_AMOUNT, ATTRIBUTE_AMOUNT);

            for(unsigned int k = 0 ; k < c_primitive.attributes_count ; ++k) {
                AttributeInfo& attribute = attributes[k];
                read_attribute(attribute, c_primitive.attributes[k]);

                if(!(attribute.type == AttributeType::NONE || attribute.data.empty())) {
                    mesh.enable_attribute(attribute.attribute, attribute.type);
                    attributes_indices[attribute.attribute] = k;

#ifdef DEBUG_LOG_GLTF_READ_INFO
                    std::cout << '\t' << attribute_to_string(attribute.attribute)
                        << " (" << attribute_type_to_string(attribute.type) << "): "
                        << attribute.data.size() << "\n";
#endif
                }
            }

            // Remove the vertex attributes that aren't enabled
            unsigned int valid_attributes_count = 0;
            unsigned int next_index = ATTRIBUTE_AMOUNT - 1;
            for(unsigned int k = 0 ; k < ATTRIBUTE_AMOUNT ; ++k) {
                if(attributes_indices[k] == ATTRIBUTE_AMOUNT) {
                    while(attributes_indices[next_index] == ATTRIBUTE_AMOUNT) { next_index--; }
                    if(next_index < k) { break; }
                    std::swap(attributes_indices[k], attributes_indices[next_index--]);
                } else {
                    ++valid_attributes_count;
                }
            }
            attributes_indices.resize(valid_attributes_count);

            unsigned int vertices_count = c_primitive.attributes[0].data->count;
            for(unsigned int k = 0 ; k < vertices_count ; ++k) {
                for(unsigned int index : attributes_indices) {
                    mesh.push_values(&attributes[index].data[k * attributes[index].component_count],
                                     attributes[index].component_count);
                }
            }

            mesh.bind_buffers();

            delete[] attributes;

#ifdef DEBUG_LOG_GLTF_READ_INFO
            std::cout << '\n';
#endif
        }
    }

    cgltf_free(data);

    for(const vector2<unsigned int>& index : transparent_indices_order) { indices_order.push_back(index); }
}

void Scene::read_attribute(AttributeInfo& attribute_info, const cgltf_attribute& c_attribute) {
    const cgltf_accessor* c_accessor = c_attribute.data;

    switch(c_attribute.type) {
        case cgltf_attribute_type_position:
            attribute_info.attribute = ATTRIBUTE_POSITION;
            break;
        case cgltf_attribute_type_normal:
            attribute_info.attribute = ATTRIBUTE_NORMAL;
            break;
        case cgltf_attribute_type_texcoord:
            attribute_info.attribute = ATTRIBUTE_TEX_COORDS;
            break;
        case cgltf_attribute_type_color:
            attribute_info.attribute = ATTRIBUTE_COLOR;
            break;
        default:
            std::cout << "\tUnhandled or invalid attribute: '"
                << cgltf_attribute_type_to_string(c_attribute.type)
                << "'.\n";
            attribute_info.type = AttributeType::NONE;
            return;
    }

    switch(c_accessor->type) {
        case cgltf_type_scalar:
            attribute_info.type = AttributeType::FLOAT;
            break;
        case cgltf_type_vec2:
            attribute_info.type = AttributeType::VEC2;
            break;
        case cgltf_type_vec3:
            attribute_info.type = AttributeType::VEC3;
            break;
        case cgltf_type_vec4:
            attribute_info.type = AttributeType::VEC4;
            break;
        default:
            throw std::runtime_error("Unhandled or invalid attribute type: "
                                     + cgltf_type_to_string(c_accessor->type)
                                     + '.');
    }

    attribute_info.component_count = get_attribute_type_count(attribute_info.type);

    attribute_info.data.resize(c_accessor->count * attribute_info.component_count);
    cgltf_accessor_unpack_floats(c_accessor, attribute_info.data.data(), attribute_info.data.size());
}
