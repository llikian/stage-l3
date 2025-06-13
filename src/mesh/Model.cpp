/***************************************************************************************************
 * @file  Model.cpp
 * @brief Implementation of the Model class
 **************************************************************************************************/

#include "mesh/Model.hpp"

#include "maths/geometry.hpp"
#include "utility/hash.hpp"

Model::Model(const std::filesystem::path& path, const mat4& model, bool verbose)
    : model_matrix(model) {
    std::string extension = path.extension().string();

    if(extension != ".obj") {
        throw std::runtime_error("Cannot load '" + extension + "' file, file formats supported by Model are: .obj");
    }

    if(verbose) { std::cout << "Reading Model from file '" << path.filename().string() << "':\n"; }

    if(extension == ".obj") { parse_obj_file(path, verbose); }
}

void Model::parse_obj_file(const std::filesystem::path& path, bool verbose) {
    std::ifstream file(path);
    if(!file.is_open()) { throw std::runtime_error("Couldn't open file '" + path.string() + '\''); }

    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> tex_coords;

    std::string current_material_name;
    std::unordered_map<std::string, std::vector<ivec3>> vertex_indices;
    // x is the index for the position, y for the normal and z for the tex coords

    tex_coords.emplace_back(0.0f, 0.0f); // When no texture coordinates are provided, just put it to (0.0, 0.0).

    std::string line;

    while(std::getline(file, line)) {
        std::istringstream stream(line);
        std::string buffer;
        stream >> buffer; // Get the line's data type specifier (v, vn, vt, f, etc...)

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
            std::vector<ivec3> face;

            while(stream >> buffer) {
                face.emplace_back();
                int count = std::sscanf(buffer.c_str(), "%d/%d/%d", &face.back().x, &face.back().z, &face.back().y);

                if(count == 1) {
                    std::sscanf(buffer.c_str(), "%d//%d", &face.back().x, &face.back().y);
                } else if(count == 0) {
                    throw std::runtime_error("Format error in .obj file, no vertex attribute.");
                }
            }

            if(face.size() < 3) {
                throw std::runtime_error("Format error in .obj file, less than 3 vertices in face.");
            }

            if(current_material_name.empty()) {
                current_material_name = "default_material";
                materials.emplace(current_material_name, Material());
            }

            std::vector<ivec3>& indices = vertex_indices[current_material_name];

            for(unsigned int i = 1 ; i + 1 < face.size() ; ++i) {
                indices.push_back(face[0]);
                indices.push_back(face[i]);
                indices.push_back(face[i + 1]);
            }
        } else if(buffer == "usemtl") {
            stream >> current_material_name;
        } else if(buffer == "mtllib") {
            stream >> buffer;
            parse_mtl_file(path.parent_path() / buffer);
        }
    }

    uint64_t total_indices = 0;
    size_t original_normals_amount = normals.size();
    for(auto& [material_name, material] : materials) {
        add_mesh(positions, normals, tex_coords, vertex_indices[material_name], original_normals_amount);
        meshes.back().set_material(&material);
        total_indices += meshes.back().get_indices_amount();
    }

    if(verbose) {
        std::cout << '\t' << positions.size() << " vertex positions\n";
        if(!normals.empty()) { std::cout << '\t' << normals.size() << " normals\n"; }
        if(!tex_coords.empty()) { std::cout << '\t' << tex_coords.size() << " texture coordinates\n"; }
        std::cout << '\t' << meshes.size() << " meshes\n";
        std::cout << '\t' << materials.size() << " materials\n";
        std::cout << '\t' << "For a total of " << total_indices / 3 << " triangles.\n";
    }
}

void Model::parse_mtl_file(const std::filesystem::path& path) {
    std::ifstream file(path);
    if(!file.is_open()) { throw std::runtime_error("Couldn't open file '" + path.string() + '\''); }

    Material* material = nullptr;

    std::string line;

    while(std::getline(file, line)) {
        std::istringstream stream(line);
        std::string buffer;
        stream >> buffer; // Get the line's data type specifier (v, vn, vt, f, etc...)

        if(buffer == "newmtl") {
            stream >> buffer;
            materials.emplace(buffer, Material());
            material = &materials[buffer];
        } else if(buffer == "Ka") {
            stream >> material->ambient;
        } else if(buffer == "Kd") {
            stream >> material->diffuse;
        } else if(buffer == "Ks") {
            stream >> material->specular;
        } else if(buffer == "Ns") {
            stream >> material->specular_exponent;
        } else if(buffer == "map_Ka") {
            stream >> buffer;
            for(char& c : buffer) { if(c == '\\') { c = '/'; } }
            material->ambient_map.create(path.parent_path() / buffer);
        } else if(buffer == "map_Kd") {
            stream >> buffer;
            for(char& c : buffer) { if(c == '\\') { c = '/'; } }
            material->diffuse_map.create(path.parent_path() / buffer);
        }
    }
}

void Model::add_mesh(const std::vector<vec3>& positions,
                          std::vector<vec3>& normals,
                          const std::vector<vec2>& tex_coords,
                          std::vector<ivec3>& vertex_indices,
                          size_t original_normals_amount) {
    TriangleMesh& mesh = meshes.emplace_back();

    std::unordered_map<ivec3, unsigned int, ivec3_hash> unique_attribute_triplets;

    for(size_t i = 0 ; i + 2 < vertex_indices.size() ; i += 3) {
        if(vertex_indices[i].y == 0) {
            normals.push_back(normalize(cross(positions[vertex_indices[i + 1].x] - positions[vertex_indices[i].x],
                                              positions[vertex_indices[i + 2].x] - positions[vertex_indices[i].x])));
            vertex_indices[i].y = vertex_indices[i + 1].y = vertex_indices[i + 2].y = normals.size();
        }

        // Handling negative indices & Vertex Deduplication
        unsigned int indices[3];

        for(int j = 0 ; j < 3 ; ++j) {
            ivec3& vertex = vertex_indices[i + j];

            vertex.x += vertex.x < 0 ? positions.size() : -1;
            vertex.y += vertex.y < 0 ? original_normals_amount : -1;
            if(vertex.z < 0) { vertex.z += tex_coords.size(); }

            auto [index, was_inserted] = unique_attribute_triplets.try_emplace(vertex, mesh.get_vertices_amount());
            if(was_inserted) { mesh.add_vertex(positions[vertex.x], normals[vertex.y], tex_coords[vertex.z]); }
            indices[j] = index->second;
        }

        mesh.add_triangle(indices[0], indices[1], indices[2]);
    }
}

void Model::draw(Shader& shader) {
    shader.use();
    for(TriangleMesh& mesh : meshes) { mesh.draw(shader); }
}
