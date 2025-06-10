/***************************************************************************************************
 * @file  file_readers.cpp
 * @brief Implementation of function aimed at reading specific file formats.
 **************************************************************************************************/

#include "utility/file_readers.hpp"

#include <sstream>
#include "maths/geometry.hpp"

void read_obj_file(const std::string& path, TriangleMesh& mesh) {
    std::ifstream file(path);
    if(!file.is_open()) { throw std::runtime_error("Couldn't open file"); }

    std::string name;    // Currently unused.
    bool smooth_shading; // Currently unused.
    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> tex_coords;

    positions.emplace_back(0.0f, 0.0f, 0.0f); // Should not be used. Means there is a problem.
    normals.emplace_back(0.0f, 0.0f, 0.0f);   // Should not be used. Calculated with cross product.
    tex_coords.emplace_back(0.0f, 0.0f);

    struct VertexIndices {
        int v, vt, vn;
    };
    std::vector<VertexIndices> vertex_indices;

    std::string line;
    float x, y, z;

    while(std::getline(file, line)) {
        std::stringstream stream(line);
        std::string buf;
        stream >> buf; // Get rid of the line's data type specifier (v, vt, vn, f, etc...)

        if(line[0] == 'v') {
            if(line[1] == ' ') {
                stream >> x >> y >> z;
                positions.emplace_back(x, y, z);
            } else if(line[1] == 'n') {
                normals.push_back(normalize(vec3(x, y, z)));
            } else if(line[1] == 't') {
                stream >> x >> y;
                tex_coords.emplace_back(x, y);
            }
        } else if(line[0] == 'f') { // Face
            int v[4]{ 0, 0, 0, 0 };
            int vt[4]{ 0, 0, 0, 0 };
            int vn[4]{ 0, 0, 0, 0 };

            unsigned int vertices = 0;

            while(stream >> buf) {
                int count = std::sscanf(buf.c_str(), "%d/%d/%d", &v[vertices], &vt[vertices], &vn[vertices]);

                if(count == 1) {
                    std::sscanf(buf.c_str(), "%d//%d", &v[vertices], &vn[vertices]);
                } else if(count == 0) {
                    throw std::runtime_error("Format error in .obj file, no vertex attribute.");
                } else if(count > 3) {
                    throw std::runtime_error("Format error in .obj file, more than 3 vertex attributes.");
                }

                ++vertices;
            }

            if(vertices == 3) {
                vertex_indices.emplace_back(v[0], vt[0], vn[0]);
                vertex_indices.emplace_back(v[1], vt[1], vn[1]);
                vertex_indices.emplace_back(v[2], vt[2], vn[2]);
            } else if(vertices == 4) {
                vertex_indices.emplace_back(v[0], vt[0], vn[0]);
                vertex_indices.emplace_back(v[1], vt[1], vn[1]);
                vertex_indices.emplace_back(v[3], vt[3], vn[3]);

                vertex_indices.emplace_back(v[1], vt[1], vn[1]);
                vertex_indices.emplace_back(v[2], vt[2], vn[2]);
                vertex_indices.emplace_back(v[3], vt[3], vn[3]);
            } else if(vertices < 3) {
                throw std::runtime_error("Format error in .obj file, less than 3 vertices in face.");
            } else {
                throw std::runtime_error("Unhandled case in read_obj_file, more than 4 vertices in face.");
            }
        } else if(line[0] == 'o') { // Object Name
            name = line.substr(2);
        } else if(line[0] == 's') { // Smooth Shading
            smooth_shading = line[2] == 1;
        }
    }

    if(vertex_indices.empty()) {
        // TODO : Find out if this is actually possible in obj files.
        throw std::runtime_error("Unhandled case in read_obj_file, no faces.");
    } else {
        for(int i = 0 ; i + 2 < vertex_indices.size() ; i += 3) {
            if(vertex_indices[i].vn == 0) {
                normals[0] = normalize(cross(positions[vertex_indices[i + 1].v] - positions[vertex_indices[i].v],
                                             positions[vertex_indices[i + 2].v] - positions[vertex_indices[i].v]));
            }

            mesh.addVertex(positions[vertex_indices[i].v],
                           normals[vertex_indices[i].vn],
                           tex_coords[vertex_indices[i].vt]);
            mesh.addVertex(positions[vertex_indices[i + 1].v],
                           normals[vertex_indices[i + 1].vn],
                           tex_coords[vertex_indices[i + 1].vt]);
            mesh.addVertex(positions[vertex_indices[i + 2].v],
                           normals[vertex_indices[i + 2].vn],
                           tex_coords[vertex_indices[i + 2].vt]);
        }
    }
}
