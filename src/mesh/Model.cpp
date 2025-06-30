/***************************************************************************************************
 * @file  Model.cpp
 * @brief Implementation of the Model class
 **************************************************************************************************/

#include "mesh/Model.hpp"

#include <ranges>
#include "maths/geometry.hpp"
#include "utility/hash.hpp"

#ifdef DEBUG
#include "debug.hpp"
#include "utility/LifetimeLogger.hpp"
#endif

Model::Model(const std::filesystem::path& path) {
    std::string extension = path.extension().string();

    if(extension != ".obj") {
        throw std::runtime_error("Cannot load '" + extension + "' file, file formats supported by Model are: .obj");
    }

    if(extension == ".obj") { parse_obj_file(path); }
}

Model::~Model() {
    for(Material& material : materials) { material.free(); }
}

void Model::parse_obj_file(const std::filesystem::path& path) {
#ifdef DEBUG_LOG_MODEL_READ_INFO
    LifetimeLogger lifetime_logger("\tTook ");
    std::cout << "Reading model from file '" << path.filename().string() << "':\n";
    uint64_t total_indices = 0;
#endif

    std::ifstream file(path);
    if(!file.is_open()) { throw std::runtime_error("Couldn't open file '" + path.string() + '\''); }

    // Load all materials first
    materials.emplace_back("Default Material");
    for(std::string line ; std::getline(file, line) ;) {
        if(line[0] == 'm') { // mtllib
            std::istringstream stream(line);
            std::string buffer;
            stream >> buffer;

            if(buffer == "mtllib") {
                stream >> buffer;
                parse_mtl_file(path.parent_path() / buffer);
            }
        }
    }

    unsigned int next_transparent_index = materials.size() - 1;
    for(unsigned int i = 0 ; i < materials.size() ; ++i) {
        if(materials[i].has_transparency()) {
            while(materials[next_transparent_index].has_transparency()) { next_transparent_index--; }
            if(next_transparent_index < i) { break; }
            std::swap(materials[i], materials[next_transparent_index--]);
        }
    }

    std::unordered_map<std::string, unsigned int> material_indices;
    for(unsigned int i = 0 ; i < materials.size() ; ++i) { material_indices.emplace(materials[i].name, i); }

    unsigned int current_material_index = material_indices["Default Material"];

    file.clear();
    file.seekg(0, std::ios::beg);

    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> tex_coords;

    tex_coords.emplace_back(0.0f, 0.0f); // When no texture coordinates are provided, just put it to (0.0, 0.0).

    // x is the index for the position, y for the normal and z for the tex coords
    std::vector<std::vector<llvec3>> vertex_indices(materials.size());

    for(std::string line ; std::getline(file, line) ;) {
        std::istringstream stream(line);
        std::string buffer;
        stream >> buffer; // Get the line's data type specifier (v, vn, vt, f, etc...)

        if(buffer[0] == '#') { continue; }

        if(buffer[0] == 'v') {
            float x, y, z;
            stream >> x >> y >> z;

            if(buffer.size() == 1) {
                positions.emplace_back(x, y, z);
            } else if(buffer[1] == 'n') {
                normals.push_back(normalize(vec3(x, y, z)));
            } else if(buffer[1] == 't') {
                tex_coords.emplace_back(x, y);
            }
        } else if(buffer[0] == 'f') { // Face
            std::vector<llvec3> face;

            while(stream >> buffer) {
                llvec3& vertex = face.emplace_back();
                int count = std::sscanf(buffer.c_str(), "%lld/%lld/%lld", &vertex.x, &vertex.z, &vertex.y);

                if(count == 1) {
                    std::sscanf(buffer.c_str(), "%lld//%lld", &vertex.x, &vertex.y);
                } else if(count == 0) {
                    throw std::runtime_error("Format error in .obj file, no vertex attribute.");
                }

                vertex.x = vertex.x < 0 ? positions.size() + vertex.x : vertex.x - 1;
                vertex.y = vertex.y < 0 ? normals.size() + vertex.y : vertex.y - 1;
                vertex.z = vertex.z < 0 ? tex_coords.size() + vertex.z : vertex.z;
            }

            if(face.size() < 3) {
                throw std::runtime_error("Format error in .obj file, less than 3 vertices in face.");
            }

#ifdef DEBUG_LOG_MODEL_READ_INFO
            total_indices += 3 * (face.size() - 2);
#endif

            std::vector<llvec3>& indices = vertex_indices[current_material_index];
            for(unsigned int i = 1 ; i + 1 < face.size() ; ++i) {
                indices.push_back(face[0]);
                indices.push_back(face[i]);
                indices.push_back(face[i + 1]);
            }
        } else if(buffer[0] == 'u') { // usemtl
            stream >> buffer;
            current_material_index = material_indices[buffer];
        }
    }

    meshes.reserve(materials.size());
    for(unsigned int i = 0 ; i < materials.size() ; ++i) {
        add_mesh(positions, normals, tex_coords, vertex_indices[i]);
        meshes.back().set_material(&materials[i]);
    }

#ifdef DEBUG_LOG_MODEL_READ_INFO
    std::cout << '\t' << positions.size() << " vertex positions\n";
    if(!normals.empty()) { std::cout << '\t' << normals.size() << " normals\n"; }
    if(!tex_coords.empty()) { std::cout << '\t' << tex_coords.size() << " texture coordinates\n"; }
    std::cout << '\t' << meshes.size() << " meshes\n";
    std::cout << '\t' << materials.size() << " materials\n";
    std::cout << '\t' << "For a total of " << total_indices / 3 << " triangles.\n";
#endif
}

void Model::parse_mtl_file(const std::filesystem::path& path) {
#ifdef DEBUG_LOG_MATERIAL_LIBRARY_READ_INFO
    LifetimeLogger logger("\t\tTook: ");
    std::cout << "\tReading material library from file '" << path.filename().string() << "':\n";
#endif

    std::ifstream file(path);
    if(!file.is_open()) { throw std::runtime_error("Couldn't open file '" + path.string() + '\''); }

    unsigned int materials_amount = materials.size();
    std::string line;
    while(std::getline(file, line)) {
        if(line[0] == 'n' && line.substr(0, 6) == "newmtl") { ++materials_amount; }
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    materials.reserve(materials_amount);
    unsigned int material_index = 0;

    while(std::getline(file, line)) {
        std::istringstream stream(line);
        std::string buffer;
        stream >> buffer; // Get the line's data type specifier (Ka, Kd, Ks, Ns...)

        if(buffer == "newmtl") {
            stream >> buffer;
            materials.emplace_back(buffer);
            material_index = materials.size() - 1;
        } else if(buffer == "Ka") {
            stream >> materials[material_index].ambient;
        } else if(buffer == "Kd") {
            stream >> materials[material_index].diffuse;
        } else if(buffer == "Ks") {
            stream >> materials[material_index].specular;
        } else if(buffer == "Ns") {
            stream >> materials[material_index].specular_exponent;
        } else if(buffer == "map_Kd") {
            std::string texture_path;
            stream >> texture_path;
            while(stream >> buffer) { texture_path += ' ' + buffer; }
            for(char& c : texture_path) { if(c == '\\') { c = '/'; } }
            materials[material_index].diffuse_map.create(path.parent_path() / texture_path);
        }
    }

#ifdef DEBUG_LOG_MATERIAL_LIBRARY_READ_INFO
    std::cout << "\t\t" << materials.size() << " materials.\n";
#endif
}

void Model::add_mesh(const std::vector<vec3>& positions,
                     std::vector<vec3>& normals,
                     const std::vector<vec2>& tex_coords,
                     std::vector<llvec3>& vertex_indices) {
    TriangleMesh& mesh = meshes.emplace_back();

    std::unordered_map<llvec3, long long, vector3_hash<long long>> unique_attribute_triplets;

    for(size_t i = 0 ; i + 2 < vertex_indices.size() ; i += 3) {
        if(vertex_indices[i].y == -1) {
            normals.push_back(normalize(cross(positions[vertex_indices[i + 1].x] - positions[vertex_indices[i].x],
                                              positions[vertex_indices[i + 2].x] - positions[vertex_indices[i].x])));
            vertex_indices[i].y = vertex_indices[i + 1].y = vertex_indices[i + 2].y = normals.size() - 1;
        }

        // Handling negative indices & Vertex Deduplication
        long long indices[3];

        for(int j = 0 ; j < 3 ; ++j) {
            const llvec3& vertex = vertex_indices[i + j];
            auto [index, was_inserted] = unique_attribute_triplets.try_emplace(vertex, mesh.get_vertices_amount());
            if(was_inserted) { mesh.add_vertex(positions[vertex.x], normals[vertex.y], tex_coords[vertex.z]); }
            indices[j] = index->second;
        }

        mesh.add_triangle(indices[0], indices[1], indices[2]);
    }
}

void Model::draw(const Shader& shader) {
    shader.use();
    for(TriangleMesh& mesh : meshes) { mesh.draw(shader); }
}

void Model::apply_model_matrix(const mat4& model) {
    for(TriangleMesh& mesh : meshes) { mesh.apply_model_matrix(model); }
}

void Model::get_min_max_axis_aligned_coordinates(vec3& minimum, vec3& maximum) const {
    for(unsigned int i = 0 ; i < meshes.size() ; ++i) {
        meshes[i].get_min_max_axis_aligned_coordinates(minimum, maximum);
    }
}
