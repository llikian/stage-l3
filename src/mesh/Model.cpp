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

    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> tex_coords;
    std::vector<ivec3> vertex_indices; // x is the index for the position, y for the normal and z for the tex coords
    Material* material = nullptr;

    tex_coords.emplace_back(0.0f, 0.0f); // When no texture coordinates are provided, just put it to (0.0, 0.0).

    std::string line;

    while(std::getline(file, line)) {
        std::istringstream stream(line);
        std::string buffer;
        stream >> buffer; // Get rid of the line's data type specifier (v, vn, vt, f, etc...)

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
            // We don't set the default value to -1 for positions and normals because we subtract 1 from every index,
            // so the 0 here later becomes -1 if sscanf doesn't replace the value.
            int v[4]{ 0, 0, 0, 0 };
            int vn[4]{ 0, 0, 0, 0 };
            int vt[4]{ 0, 0, 0, 0 };

            unsigned int vertices = 0;

            while(stream >> buffer) {
                int count = std::sscanf(buffer.c_str(), "%d/%d/%d", &v[vertices], &vt[vertices], &vn[vertices]);

                if(count == 1) {
                    std::sscanf(buffer.c_str(), "%d//%d", &v[vertices], &vn[vertices]);
                } else if(count == 0) {
                    throw std::runtime_error("Format error in .obj file, no vertex attribute.");
                }

                ++vertices;
            }

            if(vertices == 3) {
                vertex_indices.emplace_back(v[0] - 1, vn[0] - 1, vt[0]);
                vertex_indices.emplace_back(v[1] - 1, vn[1] - 1, vt[1]);
                vertex_indices.emplace_back(v[2] - 1, vn[2] - 1, vt[2]);
            } else if(vertices == 4) {
                vertex_indices.emplace_back(v[0] - 1, vn[0] - 1, vt[0]);
                vertex_indices.emplace_back(v[1] - 1, vn[1] - 1, vt[1]);
                vertex_indices.emplace_back(v[3] - 1, vn[3] - 1, vt[3]);

                vertex_indices.emplace_back(v[1] - 1, vn[1] - 1, vt[1]);
                vertex_indices.emplace_back(v[2] - 1, vn[2] - 1, vt[2]);
                vertex_indices.emplace_back(v[3] - 1, vn[3] - 1, vt[3]);
            } else if(vertices < 3) {
                throw std::runtime_error("Format error in .obj file, less than 3 vertices in face.");
            } else {
                throw std::runtime_error("Unhandled case in read_obj_file, more than 4 vertices in face.");
            }
        } else if(buffer[0] == 'g') {
            if(!vertex_indices.empty()) {
                handle_object(positions, normals, tex_coords, vertex_indices);
                meshes.back().set_material(material);
                vertex_indices.resize(0);
            }
        } else if(buffer == "usemtl") {
            stream >> buffer;
            material = &materials[buffer];
        } else if(buffer == "mtllib") {
            stream >> buffer;
            parse_mtl_file(path.parent_path().string() + '/' + buffer, verbose);
        }
    }

    handle_object(positions, normals, tex_coords, vertex_indices);
    meshes.back().set_material(material);
}

void Model::parse_mtl_file(const std::filesystem::path& path, bool verbose) {
    std::ifstream file(path);
    if(!file.is_open()) { throw std::runtime_error("Couldn't open file '" + path.string() + '\''); }


}

void Model::handle_object(std::vector<vec3>& positions,
                          std::vector<vec3>& normals,
                          std::vector<vec2>& tex_coords,
                          std::vector<ivec3>& vertex_indices) {
    TriangleMesh& mesh = meshes.emplace_back();

    std::unordered_map<uint64_t, unsigned int> unique_attribute_triplets;

    for(int i = 0 ; i + 2 < vertex_indices.size() ; i += 3) {
        if(vertex_indices[i].y == -1) {
            normals.push_back(normalize(cross(positions[vertex_indices[i + 1].x] - positions[vertex_indices[i].x],
                                              positions[vertex_indices[i + 2].x] - positions[vertex_indices[i].x])));
            vertex_indices[i].y = vertex_indices[i + 1].y = vertex_indices[i + 2].y = normals.size() - 1;
        }

        uint64_t ids[3];
        for(int j = 0 ; j < 3 ; ++j) {
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
