/***************************************************************************************************
 * @file  primitives.cpp
 * @brief Implementation of functions aimed at creating basic meshes such as cubes or spheres
 **************************************************************************************************/

#include "mesh/primitives.hpp"

#include <cmath>
#include "maths/constants.hpp"
#include "maths/geometry.hpp"
#include "Window.hpp"

void create_sphere_mesh(Mesh& mesh, unsigned int horizontal_slices, unsigned int vertical_slices) {
    mesh.set_primitive(Primitive::TRIANGLES);
    mesh.enable_attribute(Attribute::NORMAL);
    mesh.enable_attribute(Attribute::TEX_COORDS);

    const float theta_step = PI_F / horizontal_slices; // theta in [-PI/2 ; PI/2]
    const float phi_step = TAU_F / vertical_slices;    // phi in [0 ; 2PI]

    for(unsigned int i = 0 ; i <= horizontal_slices ; ++i) {
        float theta = PI_HALF_F - i * theta_step;
        float cos_theta = std::cos(theta);
        vec3 point(0.0f, std::sin(theta), 0.0f);

        for(unsigned int j = 0 ; j <= vertical_slices ; ++j) {
            float phi = j * phi_step;
            point.x = cos_theta * std::cos(phi);
            point.z = cos_theta * std::sin(phi);

            // TODO : Check if the texture coordinates are correct.
            mesh.add_vertex(point, point, vec2(1.0f - phi / TAU_F, 0.5f + 0.5f * point.y));
        }
    }

    auto index = [&vertical_slices](int i, int j) -> unsigned int {
        return j + i * (vertical_slices + 1);
    };

    for(unsigned int j = 0 ; j < vertical_slices ; ++j) {
        mesh.add_triangle(index(0, j), index(1, j + 1), index(1, j));
        mesh.add_triangle(index(horizontal_slices - 1, j),
                          index(horizontal_slices - 1, j + 1),
                          index(horizontal_slices, j));
    }

    for(unsigned int i = 1 ; i < horizontal_slices ; ++i) {
        for(unsigned int j = 0 ; j < vertical_slices ; ++j) {
            mesh.add_face(
                index(i + 1, j + 1),
                index(i + 1, j),
                index(i, j),
                index(i, j + 1)
            );
        }
    }

    mesh.bind_buffers();
}

void create_cube_mesh(Mesh& mesh) {
    mesh.set_primitive(Primitive::TRIANGLES);
    mesh.enable_attribute(Attribute::NORMAL);
    mesh.enable_attribute(Attribute::TEX_COORDS);

    static const vec3 positions[8]{
        vec3(1.0f, 1.0f, 1.0f),   // 0: TOP - RIGHT - FRONT
        vec3(1.0f, 1.0f, -1.0f),  // 1: TOP - RIGHT - BACK
        vec3(1.0f, -1.0f, 1.0f),  // 2: BOTTOM - RIGHT - FRONT
        vec3(1.0f, -1.0f, -1.0f), // 3: BOTTOM - RIGHT - BACK
        vec3(-1.0f, 1.0f, 1.0f),  // 4: TOP - LEFT - FRONT
        vec3(-1.0f, 1.0f, -1.0f), // 5: TOP - LEFT - BACK
        vec3(-1.0f, -1.0f, 1.0f), // 6: BOTTOM - LEFT - FRONT
        vec3(-1.0f, -1.0f, -1.0f) // 7: BOTTOM - LEFT - BACK
    };

    static const vec3 normals[6]{
        vec3(1.0f, 0.0f, 0.0f),  // 0: RIGHT
        vec3(-1.0f, 0.0f, 0.0f), // 1: LEFT
        vec3(0.0f, 1.0f, 0.0f),  // 2: TOP
        vec3(0.0f, -1.0f, 0.0f), // 3: BOTTOM
        vec3(0.0f, 0.0f, 1.0f),  // 4: FRONT
        vec3(0.0f, 0.0f, -1.0f)  // 5: BACK
    };

    static const uvec4 faces[6]{
        uvec4(0, 2, 3, 1), // 0: RIGHT
        uvec4(5, 7, 6, 4), // 1: LEFT
        uvec4(5, 4, 0, 1), // 2: TOP
        uvec4(6, 7, 3, 2), // 3: BOTTOM
        uvec4(4, 6, 2, 0), // 4: FRONT
        uvec4(1, 3, 7, 5)  // 5: BACK
    };

    static const vec2 tex_coords[4]{
        vec2(0.0f, 1.0f),
        vec2(0.0f, 0.0f),
        vec2(1.0f, 0.0f),
        vec2(1.0f, 1.0f)
    };

    for(unsigned int i = 0 ; i < 6 ; ++i) {
        mesh.add_vertex(positions[faces[i].x], normals[i], tex_coords[0]);
        mesh.add_vertex(positions[faces[i].y], normals[i], tex_coords[1]);
        mesh.add_vertex(positions[faces[i].z], normals[i], tex_coords[2]);
        mesh.add_vertex(positions[faces[i].w], normals[i], tex_coords[3]);
        mesh.add_face(i * 4, i * 4 + 1, i * 4 + 2, i * 4 + 3);
    }

    mesh.bind_buffers();
}

void create_wireframe_cube_mesh(Mesh& mesh) {
    mesh.set_primitive(Primitive::LINES);

    mesh.add_vertex(vec3(1.0f, 1.0f, 1.0f));    // 0: TOP - RIGHT - FRONT
    mesh.add_vertex(vec3(1.0f, 1.0f, -1.0f));   // 1: TOP - RIGHT - BACK
    mesh.add_vertex(vec3(1.0f, -1.0f, 1.0f));   // 2: BOTTOM - RIGHT - FRONT
    mesh.add_vertex(vec3(1.0f, -1.0f, -1.0f));  // 3: BOTTOM - RIGHT - BACK
    mesh.add_vertex(vec3(-1.0f, 1.0f, 1.0f));   // 4: TOP - LEFT - FRONT
    mesh.add_vertex(vec3(-1.0f, 1.0f, -1.0f));  // 5: TOP - LEFT - BACK
    mesh.add_vertex(vec3(-1.0f, -1.0f, 1.0f));  // 6: BOTTOM - LEFT - FRONT
    mesh.add_vertex(vec3(-1.0f, -1.0f, -1.0f)); // 7: BOTTOM - LEFT - BACK

    // RIGHT  : (0, 2, 3, 1)
    // LEFT   : (5, 7, 6, 4)
    // TOP    : (5, 4, 0, 1)
    // BOTTOM : (6, 7, 3, 2)
    // FRONT  : (4, 6, 2, 0)
    // BACK   : (1, 3, 7, 5)

    // RIGHT
    mesh.add_line(0, 2);
    mesh.add_line(2, 3);
    mesh.add_line(3, 1);
    mesh.add_line(1, 0);

    // LEFT
    mesh.add_line(5, 7);
    mesh.add_line(7, 6);
    mesh.add_line(6, 4);
    mesh.add_line(4, 5);

    // FRONT
    mesh.add_line(6, 2);
    mesh.add_line(0, 4);

    // BACK
    mesh.add_line(3, 7);
    mesh.add_line(5, 1);

    mesh.bind_buffers();
}

void create_quad_mesh(Mesh& mesh, const vec3& A, const vec3& B, const vec3& C) {
    mesh.set_primitive(Primitive::TRIANGLES);
    mesh.enable_attribute(Attribute::NORMAL);
    mesh.enable_attribute(Attribute::TEX_COORDS);

    const vec3 BA(A - B);
    const vec3 BC(C - B);
    const vec3 normal(normalize(cross(BA, BC)));

    mesh.add_vertex(A, normal, vec2(0.0f, 1.0f));
    mesh.add_vertex(B, normal, vec2(0.0f, 0.0f));
    mesh.add_vertex(C, normal, vec2(1.0f, 0.0f));
    mesh.add_vertex(B + BA + BC, normal, vec2(1.0f, 1.0f));

    mesh.add_face(0, 1, 2, 3);

    mesh.bind_buffers();
}

void create_axes_mesh(Mesh& mesh, float length) {
    mesh.set_primitive(Primitive::LINES);
    mesh.enable_attribute(Attribute::COLOR);

    const vec3 origin(0.0f, 0.0f, 0.0f);
    const vec3 axes[3]{
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.0f, 1.0f)
    };

    for(const vec3& axis : axes) {
        mesh.add_vertex(origin, axis);
        mesh.add_vertex(length * axis, axis);
    }

    mesh.bind_buffers();
}

void create_pyramid_mesh(Mesh& mesh, const vec3& A, const vec3& B, const vec3& C, float height) {
    mesh.set_primitive(Primitive::LINES);

    const vec3 BA = A - B;
    const vec3 BC = C - B;

    mesh.add_vertex(0.5f * (A + C) + height * normalize(cross(BA, BC)));
    mesh.add_vertex(A);
    mesh.add_vertex(B);
    mesh.add_vertex(C);
    mesh.add_vertex(B + BA + BC);

    mesh.add_line(0, 1);
    mesh.add_line(0, 2);
    mesh.add_line(0, 3);
    mesh.add_line(0, 4);

    mesh.add_line(1, 2);
    mesh.add_line(2, 3);
    mesh.add_line(3, 4);
    mesh.add_line(4, 1);

    mesh.bind_buffers();
}

void create_frustum_meshes(Mesh& faces, Mesh& lines, const Camera& camera) {
    static const vec4 projection_space_points[8]{
        vec4(1.0f, 1.0f, 1.0f, 1.0f),
        vec4(1.0f, 1.0f, -1.0f, 1.0f),
        vec4(1.0f, -1.0f, 1.0f, 1.0f),
        vec4(1.0f, -1.0f, -1.0f, 1.0f),
        vec4(-1.0f, 1.0f, 1.0f, 1.0f),
        vec4(-1.0f, 1.0f, -1.0f, 1.0f),
        vec4(-1.0f, -1.0f, 1.0f, 1.0f),
        vec4(-1.0f, -1.0f, -1.0f, 1.0f)
    };

    vec4 points[8];
    mat4 inverse_projection = camera.get_inverse_projection_matrix();
    for(int i = 0 ; i < 8 ; ++i) {
        points[i] = inverse_projection * projection_space_points[i];
        points[i] /= points[i].w;
    }

    /* Line Mesh */
    lines.set_primitive(Primitive::LINES);

    lines.add_vertex(vec3(points[0]));
    lines.add_vertex(vec3(points[1]));
    lines.add_vertex(vec3(points[2]));
    lines.add_vertex(vec3(points[3]));
    lines.add_vertex(vec3(points[4]));
    lines.add_vertex(vec3(points[5]));
    lines.add_vertex(vec3(points[6]));
    lines.add_vertex(vec3(points[7]));

    // Far to Near Lines
    lines.add_line(0, 4);
    lines.add_line(1, 5);
    lines.add_line(2, 6);
    lines.add_line(3, 7);

    // Far Lines
    lines.add_line(0, 1);
    lines.add_line(1, 2);
    lines.add_line(2, 3);
    lines.add_line(3, 0);

    // Near Lines
    lines.add_line(4, 5);
    lines.add_line(5, 6);
    lines.add_line(6, 7);
    lines.add_line(7, 4);

    /* Face Mesh */
    faces.set_primitive(Primitive::TRIANGLES);

    faces.add_vertex(vec3(points[0]));
    faces.add_vertex(vec3(points[1]));
    faces.add_vertex(vec3(points[2]));
    faces.add_vertex(vec3(points[3]));
    faces.add_vertex(vec3(points[4]));
    faces.add_vertex(vec3(points[5]));
    faces.add_vertex(vec3(points[6]));
    faces.add_vertex(vec3(points[7]));

    faces.add_face(0, 1, 2, 3); // FAR
    faces.add_face(4, 5, 6, 7); // NEAR
    faces.add_face(0, 4, 7, 3); // TOP
    faces.add_face(5, 1, 2, 6); // BOTTOM
    faces.add_face(0, 1, 5, 4); // LEFT
    faces.add_face(7, 6, 2, 3); // RIGHT

    lines.bind_buffers();
    faces.bind_buffers();
}
