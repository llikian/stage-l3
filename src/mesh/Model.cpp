/***************************************************************************************************
 * @file  Model.cpp
 * @brief Implementation of the Model class
 **************************************************************************************************/

#include "mesh/Model.hpp"

#include "maths/geometry.hpp"
#include "utility/hash.hpp"

Model::Model(const std::filesystem::path& path, const mat4& model, bool verbose)
    : model(model) {
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

    std::string parent_path = path.parent_path().string() + '/';

    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> tex_coords;

    // x is the index for the position, y for the normal and z for the tex coords
    Material* current_material = nullptr;
    std::unordered_map<Material*, std::vector<ivec3>> vertex_indices;

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

            if(current_material == nullptr) {
                materials.emplace("default_material", Material());
                current_material = &materials["default_material"];
            }

            for(unsigned int i = 1 ; i + 1 < face.size() ; ++i) {
                vertex_indices[current_material].push_back(face[0]);
                vertex_indices[current_material].push_back(face[i]);
                vertex_indices[current_material].push_back(face[i + 1]);
            }
        } else if(buffer == "usemtl") {
            stream >> buffer;
            current_material = &materials[buffer];
        } else if(buffer == "mtllib") {
            stream >> buffer;
            parse_mtl_file(parent_path + buffer);
        }
    }

    uint64_t total_indices = 0;
    size_t original_normals_amount = normals.size();
    for(auto& [_, material] : materials) {
        handle_object(positions, normals, tex_coords, vertex_indices[&material], original_normals_amount);
        meshes.back().set_material(&material);
        total_indices += vertex_indices.size();
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

    std::string parent_path = path.parent_path().string() + '/';

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
            material->ambient_map.create(parent_path + buffer);
        } else if(buffer == "map_Kd") {
            stream >> buffer;
            for(char& c : buffer) { if(c == '\\') { c = '/'; } }
            material->diffuse_map.create(parent_path + buffer);
        }
    }
}

void Model::handle_object(std::vector<vec3>& positions,
                          std::vector<vec3>& normals,
                          std::vector<vec2>& tex_coords,
                          std::vector<ivec3>& vertex_indices,
                          size_t original_normals_amount) {
    TriangleMesh& mesh = meshes.emplace_back();

    std::unordered_map<uint64_t, unsigned int> unique_attribute_triplets;

    for(size_t i = 0 ; i + 2 < vertex_indices.size() ; i += 3) {
        if(vertex_indices[i].y == 0) {
            normals.push_back(normalize(cross(positions[vertex_indices[i + 1].x] - positions[vertex_indices[i].x],
                                              positions[vertex_indices[i + 2].x] - positions[vertex_indices[i].x])));
            vertex_indices[i].y = vertex_indices[i + 1].y = vertex_indices[i + 2].y = normals.size();
        }

        uint64_t ids[3];
        for(int j = 0 ; j < 3 ; ++j) {
            if(vertex_indices[i + j].x < 0) {
                vertex_indices[i + j].x = positions.size() + vertex_indices[i + j].x;
            } else {
                vertex_indices[i + j].x -= 1;
            }

            if(vertex_indices[i + j].y < 0) {
                vertex_indices[i + j].y = original_normals_amount + vertex_indices[i + j].y;
            } else {
                vertex_indices[i + j].y -= 1;
            }

            if(vertex_indices[i + j].z < 0) {
                vertex_indices[i + j].z = tex_coords.size() + vertex_indices[i + j].z;
            }

            ids[j] = hash_triplet(vertex_indices[i + j].x,
                                  vertex_indices[i + j].y,
                                  vertex_indices[i + j].z);

            if(!unique_attribute_triplets.contains(ids[j])) {
                mesh.addVertex(positions[vertex_indices[i + j].x],
                               normals[vertex_indices[i + j].y],
                               tex_coords[vertex_indices[i + j].z]);
                unique_attribute_triplets.emplace(ids[j], mesh.get_vertices_amount() - 1);
            }
        }

        mesh.addTriangle(unique_attribute_triplets[ids[0]],
                         unique_attribute_triplets[ids[1]],
                         unique_attribute_triplets[ids[2]]);
    }
}

void Model::draw(Shader& shader) {
    shader.use();
    for(TriangleMesh& mesh : meshes) { mesh.draw(shader); }
}
