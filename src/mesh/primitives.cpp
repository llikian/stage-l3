/***************************************************************************************************
 * @file  primitives.cpp
 * @brief Implementation of functions aimed at creating basic meshes such as cubes or spheres
 **************************************************************************************************/

#include "mesh/primitives.hpp"

#include <cmath>

#define PI_F 3.141592653589793f
#define PI_HALF_F 1.570796326794897f
#define TAU_F 6.283185307179586f

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

    for(int i = 1 ; i < horizontal_slices ; ++i) {
        for(int j = 0 ; j < vertical_slices ; ++j) {
            mesh.add_face(
                index(i + 1, j + 1),
                index(i + 1, j),
                index(i, j),
                index(i, j + 1)
            );
        }
    }
}
