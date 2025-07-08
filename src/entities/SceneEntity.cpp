/***************************************************************************************************
 * @file  SceneEntity.cpp
 * @brief Implementation of the SceneEntity class
 **************************************************************************************************/

#include "entities/SceneEntity.hpp"

#include "cgltf.h"
#include "debug.hpp"
#include "utility/LifetimeLogger.hpp"

SceneEntity::SceneEntity(const std::string& name, const std::filesystem::path& path)
    : Entity(name) {
    load(path);
}

void SceneEntity::load(const std::filesystem::path& path) {
#ifdef DEBUG_LOG_GLTF_READ_INFO
    LifetimeLogger lifetime_logger("\tTook ");
    std::cout << "Reading scene from file '" << path.filename().string() << "':\n";
#endif

    cgltf_options options{};
    cgltf_data* data = nullptr;
    cgltf_result result = cgltf_parse_file(&options, path.c_str(), &data);

    switch(result) {
        case cgltf_result_data_too_short:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': data_too_short");
        case cgltf_result_unknown_format:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': unknown_format");
        case cgltf_result_invalid_json:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': invalid_json");
        case cgltf_result_invalid_gltf:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': invalid_gltf");
        case cgltf_result_invalid_options:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': invalid_options");
        case cgltf_result_file_not_found:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': file_not_found");
        case cgltf_result_io_error:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': io_error");
        case cgltf_result_out_of_memory:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': out_of_memory");
        case cgltf_result_legacy_gltf:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': legacy_gltf");
        case cgltf_result_max_enum:
            throw std::runtime_error("Failed to read gltf file '" + path.string() + "': max_enum");
        default: break;
    }

    std::cout << "\tMeshes:\n";
    for(unsigned int i = 0 ; i < data->meshes_count ; ++i) {
        const cgltf_mesh& mesh = data->meshes[i];
        std::cout << "\t\t" << mesh.name << '\n';

        for(unsigned int j = 0 ; j < mesh.primitives_count ; ++j) {
            const cgltf_primitive& primitive = mesh.primitives[j];
            std::cout << "\t\t\tprimitive " << j << " (";
            switch(primitive.type) {
                case cgltf_primitive_type_invalid:
                    std::cout << "invalid";
                    break;
                case cgltf_primitive_type_points:
                    std::cout << "points";
                    break;
                case cgltf_primitive_type_lines:
                    std::cout << "lines";
                    break;
                case cgltf_primitive_type_line_loop:
                    std::cout << "line_loop";
                    break;
                case cgltf_primitive_type_line_strip:
                    std::cout << "line_strip";
                    break;
                case cgltf_primitive_type_triangles:
                    std::cout << "triangles";
                    break;
                case cgltf_primitive_type_triangle_strip:
                    std::cout << "triangle_strip";
                    break;
                case cgltf_primitive_type_triangle_fan:
                    std::cout << "triangle_fan";
                    break;
                case cgltf_primitive_type_max_enum:
                    std::cout << "max_enum";
                    break;
            }
            std::cout << "):\n";

            for(unsigned int k = 0 ; k < primitive.attributes_count ; ++k) {
                const cgltf_attribute& attribute = primitive.attributes[k];
                std::cout << "\t\t\t\t" << attribute.name << " (";
                switch(attribute.type) {
                    case cgltf_attribute_type_invalid:
                        std::cout << "invalid";
                        break;
                    case cgltf_attribute_type_position:
                        std::cout << "position";
                        break;
                    case cgltf_attribute_type_normal:
                        std::cout << "normal";
                        break;
                    case cgltf_attribute_type_tangent:
                        std::cout << "tangent";
                        break;
                    case cgltf_attribute_type_texcoord:
                        std::cout << "texcoord";
                        break;
                    case cgltf_attribute_type_color:
                        std::cout << "color";
                        break;
                    case cgltf_attribute_type_joints:
                        std::cout << "joints";
                        break;
                    case cgltf_attribute_type_weights:
                        std::cout << "weights";
                        break;
                    case cgltf_attribute_type_custom:
                        std::cout << "custom";
                        break;
                    case cgltf_attribute_type_max_enum:
                        std::cout << "max_enum";
                        break;
                }
                std::cout << "):\n";

                std::cout << "\t\t\t\t\tindex: " << attribute.index << '\n';

                const cgltf_accessor* accessor = attribute.data;
                std::cout << "\t\t\t\t\tdata:\n";
                if(accessor->name != nullptr) {
                    std::cout << "\t\t\t\t\t\tname: " << static_cast<void*>(accessor->name) << '\n';
                }
                std::cout << "\t\t\t\t\t\ttype: ";
                switch(accessor->type) {
                    case cgltf_type_invalid:
                        std::cout << "invalid";
                        break;
                    case cgltf_type_scalar:
                        std::cout << "scalar";
                        break;
                    case cgltf_type_vec2:
                        std::cout << "vec2";
                        break;
                    case cgltf_type_vec3:
                        std::cout << "vec3";
                        break;
                    case cgltf_type_vec4:
                        std::cout << "vec4";
                        break;
                    case cgltf_type_mat2:
                        std::cout << "mat2";
                        break;
                    case cgltf_type_mat3:
                        std::cout << "mat3";
                        break;
                    case cgltf_type_mat4:
                        std::cout << "mat4";
                        break;
                    case cgltf_type_max_enum:
                        std::cout << "max_enum";
                        break;
                }
                std::cout << '\n';
                std::cout << "\t\t\t\t\t\toffset: " << accessor->offset << '\n';
                std::cout << "\t\t\t\t\t\tcount: " << accessor->count << '\n';
                std::cout << "\t\t\t\t\t\tstride: " << accessor->stride << '\n';
            }
        }
    }

    cgltf_free(data);
}
