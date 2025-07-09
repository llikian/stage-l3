/***************************************************************************************************
 * @file  Scene.cpp
 * @brief Implementation of the Scene class
 **************************************************************************************************/

#include "mesh/Scene.hpp"

#include "AssetManager.hpp"
#include "debug.hpp"
#include "utility/LifetimeLogger.hpp"

Scene::Scene(const std::filesystem::path& path)
    : meshes(nullptr), meshes_count(0) {
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
    for(unsigned int i = 0 ; i < meshes_count ; ++i) {
        for(unsigned int j = 0 ; j < primitives_count[i] ; ++j) {
            const Shader& shader = AssetManager::get_relevant_shader_from_mesh(meshes[i][j]);
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

            meshes[i][j].draw();
        }
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

    cgltf_options options{};
    cgltf_data* data = nullptr;

    cgltf_result result = cgltf_parse_file(&options, path.c_str(), &data);
    check_cgltf_result(result, "Failed to read gltf file '" + path.string() + "': ");

    result = cgltf_load_buffers(&options, data, path.c_str());
    check_cgltf_result(result, "Failed to load buffers");

    result = cgltf_validate(data);
    check_cgltf_result(result, "Failed to validate data.");

    meshes = new Mesh*[data->meshes_count];
    meshes_count = data->meshes_count;
    primitives_count = new unsigned int[data->meshes_count];

    for(unsigned int i = 0 ; i < data->meshes_count ; ++i) {
        const cgltf_mesh& c_mesh = data->meshes[i];

        meshes[i] = new Mesh[c_mesh.primitives_count];
        primitives_count[i] = c_mesh.primitives_count;

        for(unsigned int j = 0 ; j < c_mesh.primitives_count ; ++j) {
            const cgltf_primitive& c_primitive = c_mesh.primitives[j];

            Mesh& mesh = meshes[i][j];

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

            std::vector<unsigned int> indices;
            indices.resize(cgltf_accessor_unpack_indices(c_primitive.indices, nullptr, sizeof(unsigned int), 0));
            cgltf_accessor_unpack_indices(c_primitive.indices, indices.data(), sizeof(unsigned int), indices.size());
            mesh.push_indices_buffer(indices);
            std::cout << "\tindices: " << indices.size() << '\n';

            AttributeInfo* attributes = new AttributeInfo[c_primitive.attributes_count];

            static constexpr unsigned int MAX_ATTRIBUTE = static_cast<unsigned char>(Attribute::AMOUNT);
            std::vector<unsigned int> attributes_order(MAX_ATTRIBUTE, MAX_ATTRIBUTE);
            unsigned int valid_attributes_count = 0;

            unsigned int vertices_count = 0;

            for(unsigned int k = 0 ; k < c_primitive.attributes_count ; ++k) {
                read_attribute(attributes[k], c_primitive.attributes[k]);
                if(!(attributes[k].type == AttributeType::NONE || attributes[k].data.empty())) {
                    ++valid_attributes_count;

                    attributes_order[static_cast<unsigned char>(attributes[k].attribute)] = k;

                    vertices_count = attributes[k].data.size() / get_attribute_type_count(attributes[k].type);
                    std::cout << '\t' << attribute_to_string(attributes[k].attribute) << ": " << vertices_count << '\n';
                }
            }

            for(unsigned int k = 0 ; k < vertices_count ; ++k) {
                for(unsigned int index : attributes_order) {
                    if(index != MAX_ATTRIBUTE) {
                        mesh.push_values(&attributes[index].data[k], get_attribute_type_count(attributes[index].type));
                    }
                }
            }

            mesh.bind_buffers();

            delete[] attributes;
        }
    }

    cgltf_free(data);
}

void Scene::read_attribute(AttributeInfo& attribute_info, const cgltf_attribute& c_attribute) {
    const cgltf_accessor* c_accessor = c_attribute.data;

    switch(c_attribute.type) {
        case cgltf_attribute_type_position:
            attribute_info.attribute = Attribute::POSITION;
            break;
        case cgltf_attribute_type_normal:
            attribute_info.attribute = Attribute::NORMAL;
            break;
        case cgltf_attribute_type_texcoord:
            attribute_info.attribute = Attribute::TEX_COORDS;
            break;
        case cgltf_attribute_type_color:
            attribute_info.attribute = Attribute::COLOR;
            std::cout << "color";
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

    attribute_info.data.resize(cgltf_accessor_unpack_floats(c_accessor, nullptr, 0));
    cgltf_accessor_unpack_floats(c_accessor, attribute_info.data.data(), attribute_info.data.size());
}
