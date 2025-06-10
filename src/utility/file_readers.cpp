/***************************************************************************************************
 * @file  file_readers.cpp
 * @brief Implementation of function aimed at reading specific file formats.
 **************************************************************************************************/

#include "utility/file_readers.hpp"

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "maths/geometry.hpp"
#include "maths/ivec3.hpp"

void read_obj_file(const std::string& path, TriangleMesh& mesh, bool verbose) {
    std::ifstream file(path);
    if(!file.is_open()) { throw std::runtime_error("Couldn't open file"); }

    std::string name;    // Currently unused.
    bool smooth_shading; // Currently unused.
    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> tex_coords;

    tex_coords.emplace_back(0.0f, 0.0f); // When no texture coordinates are provided, just put it to (0.0, 0.0).

    std::vector<ivec3> vertex_indices; // x is the index for the position, y for the normal and z for the tex coords
    std::unordered_map<std::string, int> unique_attribute_triplets;

    std::string line;
    float x, y, z;

    while(std::getline(file, line)) {
        std::stringstream stream(line);
        std::string buf;
        stream >> buf; // Get rid of the line's data type specifier (v, vn, vt, f, etc...)

        if(line[0] == 'v') {
            stream >> x >> y >> z;

            if(line[1] == ' ') {
                positions.emplace_back(x, y, z);
            } else if(line[1] == 'n') {
                normals.push_back(normalize(vec3(x, y, z)));
            } else if(line[1] == 't') {
                tex_coords.emplace_back(x, y);
            }
        } else if(line[0] == 'f') { // Face
            // We don't set the default value to -1 for positions and normals because we subtract 1 from every index,
            // so the 0 here later becomes -1 if sscanf doesn't replace the value.
            int v[4]{ 0, 0, 0, 0 };
            int vn[4]{ 0, 0, 0, 0 };
            int vt[4]{ 0, 0, 0, 0 };

            unsigned int vertices = 0;

            while(stream >> buf) {
                int count = std::sscanf(buf.c_str(), "%d/%d/%d", &v[vertices], &vt[vertices], &vn[vertices]);

                if(count == 1) {
                    std::sscanf(buf.c_str(), "%d//%d", &v[vertices], &vn[vertices]);
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
        } else if(line[0] == 'o') { // Object Name
            stream >> name;
        } else if(line[0] == 's') { // Smooth Shading
            stream >> smooth_shading;
        }
    }

    if(verbose) {
        std::cout << "Successfully loaded mesh '" << name << "' containing:\n";
        std::cout << '\t' << positions.size() << " vertex positions.\n";
        if(!normals.empty()) { std::cout << '\t' << normals.size() << " normals.\n"; }
        if(!tex_coords.empty()) { std::cout << '\t' << tex_coords.size() - 1 << " texture coordinates.\n"; }
    }

    // TODO : Find out if this is actually possible in obj files. If not it's not an unhandled case but an error.
    if(vertex_indices.empty()) { throw std::runtime_error("Unhandled case in read_obj_file, no faces."); }

    for(int i = 0 ; i + 2 < vertex_indices.size() ; i += 3) {
        if(vertex_indices[i].y == -1) {
            normals.push_back(normalize(cross(positions[vertex_indices[i + 1].x] - positions[vertex_indices[i].x],
                                              positions[vertex_indices[i + 2].x] - positions[vertex_indices[i].x])));
            vertex_indices[i].y = vertex_indices[i + 1].y = vertex_indices[i + 2].y = normals.size() - 1;
        }

        std::string ids[3];
        for(int j = 0 ; j < 3 ; ++j) {
            // TODO : Find a better solution to this.
            ids[j] = std::to_string(vertex_indices[i + j].x) + '/'
            + std::to_string(vertex_indices[i + j].y)+ '/'
            + std::to_string(vertex_indices[i + j].z);

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

    if(verbose) {
        size_t indices_amount = mesh.get_indices_amount();
        std::cout << '\t' << "For a total of "
            << mesh.get_vertices_amount() << " vertices and "
            << (indices_amount == 0 ? mesh.get_vertices_amount() : indices_amount) / 3 << " triangles.\n";
    }
}
