/***************************************************************************************************
 * @file  primitives.cpp
 * @brief Implementation of functions aimed at creating basic meshes such as cubes or spheres
 **************************************************************************************************/

#include "mesh/primitives.hpp"

#include <cmath>
#include "maths/constants.hpp"
#include "maths/geometry.hpp"

void create_sphere_mesh(TriangleMesh& mesh, unsigned int horizontal_slices, unsigned int vertical_slices) {
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
}

void create_quad_mesh(TriangleMesh& mesh, const vec3& A, const vec3& B, const vec3& C) {
    const vec3 BA(A - B);
    const vec3 BC(C - B);
    const vec3 normal(normalize(cross(BA, BC)));
    mesh.add_vertex(A, normal, vec2(0.0f, 1.0f));
    mesh.add_vertex(B, normal, vec2(0.0f, 0.0f));
    mesh.add_vertex(C, normal, vec2(1.0f, 0.0f));
    mesh.add_vertex(B + BA + BC, normal, vec2(1.0f, 1.0f));
    mesh.add_face(0, 1, 2, 3);
}

void create_axes_mesh(LineMesh& mesh, float length) {
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
}

void create_frustum_mesh(LineMesh& mesh, float fov, float aspect_ratio, float near, float far) {
    float tan_half_fov = std::tan(fov / 2.0f);

    constexpr float normal_length = 10.0f;

    mesh.add_vertex(vec3(0.0f)); // 0

    float near_height = near * tan_half_fov;
    float near_width = near_height * aspect_ratio;
    mesh.add_vertex(vec3(-near_width, near_height, -near));  // 1
    mesh.add_vertex(vec3(-near_width, -near_height, -near)); // 2
    mesh.add_vertex(vec3(near_width, -near_height, -near));  // 3
    mesh.add_vertex(vec3(near_width, near_height, -near));   // 4

    float far_height = far * tan_half_fov;
    float far_width = far_height * aspect_ratio;
    mesh.add_vertex(vec3(-far_width, far_height, -far));  // 5
    mesh.add_vertex(vec3(-far_width, -far_height, -far)); // 6
    mesh.add_vertex(vec3(far_width, -far_height, -far));  // 7
    mesh.add_vertex(vec3(far_width, far_height, -far));   // 8

    mesh.add_line(0, 5);
    mesh.add_line(0, 6);
    mesh.add_line(0, 7);
    mesh.add_line(0, 8);

    mesh.add_line(1, 2);
    mesh.add_line(2, 3);
    mesh.add_line(3, 4);
    mesh.add_line(4, 1);

    mesh.add_line(5, 6);
    mesh.add_line(6, 7);
    mesh.add_line(7, 8);
    mesh.add_line(8, 5);

    vec3 front(0.0f, 0.0f, -1.0f);
    vec3 right(1.0f, 0.0f, 0.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    vec3 far_center = far * front;
    vec3 far_up = far_height * up;
    vec3 far_right = far_width * right;

    // Near
    vec3 pos = near * front;
    mesh.add_vertex(pos, vec3(0.5f, 0.5f, 1.0f));                         // 9
    mesh.add_vertex(pos + normal_length * front, vec3(0.5f, 0.5f, 1.0f)); // 10
    mesh.add_line(9, 10);

    // Far
    pos = far_center;
    mesh.add_vertex(pos, vec3(0.5f, 0.5f, 0.0f));                         // 11
    mesh.add_vertex(pos - normal_length * front, vec3(0.5f, 0.5f, 0.0f)); // 12
    mesh.add_line(11, 12);

    // Top
    pos = (far_center + far_up) / 2.0f;
    mesh.add_vertex(pos, vec3(0.5f, 1.0f, 0.5f));                                                                // 13
    mesh.add_vertex(pos + normal_length * normalize(cross(far_center + far_up, right)), vec3(0.5f, 1.0f, 0.5f)); // 14
    mesh.add_line(13, 14);

    // Bottom
    pos = (far_center - far_up) / 2.0f;
    mesh.add_vertex(pos, vec3(0.5f, 0.0f, 0.5f));                                                                // 15
    mesh.add_vertex(pos + normal_length * normalize(cross(right, far_center - far_up)), vec3(0.5f, 0.0f, 0.5f)); // 16
    mesh.add_line(15, 16);

    // Left
    pos = (far_center - far_right) / 2.0f;
    mesh.add_vertex(pos, vec3(0.0f, 0.5f, 0.5f));                                                                // 17
    mesh.add_vertex(pos + normal_length * normalize(cross(far_center - far_right, up)), vec3(0.0f, 0.5f, 0.5f)); // 18
    mesh.add_line(17, 18);

    // Right
    pos = (far_center + far_right) / 2.0f;
    mesh.add_vertex(pos, vec3(1.0f, 0.5f, 0.5f));                                                                // 19
    mesh.add_vertex(pos + normal_length * normalize(cross(up, far_center + far_right)), vec3(1.0f, 0.5f, 0.5f)); // 20
    mesh.add_line(19, 20);
}
