/***************************************************************************************************
 * @file  Frustum.cpp
 * @brief Implementation of the Frustum struct
 **************************************************************************************************/

#include "culling/Frustum.hpp"

#include <cmath>

#include "maths/geometry.hpp"

Frustum::Frustum(const Camera& camera, float aspect_ratio) {
    // Dimensions of the far quad divided by 2
    float far_height = camera.far_distance * std::tan(camera.fov / 2.0f);
    float far_width = far_height * aspect_ratio;

    vec3 far_center = camera.far_distance * camera.direction;
    vec3 far_up = far_height * camera.up;
    vec3 far_right = far_width * camera.right;

    near_plane.normal = camera.direction;
    far_plane.normal = -camera.direction;
    top_plane.normal = normalize(cross(far_center + far_up, camera.right));
    bottom_plane.normal = normalize(cross(camera.right, far_center - far_up));
    left_plane.normal = normalize(cross(far_center - far_right, camera.up));
    right_plane.normal = normalize(cross(camera.up, far_center + far_right));

    near_plane.distance = dot(camera.position + camera.near_distance * camera.direction, near_plane.normal);
    far_plane.distance = dot(camera.position + far_center, far_plane.normal);
    top_plane.distance = dot(camera.position, top_plane.normal);
    bottom_plane.distance = dot(camera.position, bottom_plane.normal);
    left_plane.distance = dot(camera.position, left_plane.normal);
    right_plane.distance = dot(camera.position, right_plane.normal);
}

Frustum::Frustum(const Camera& camera, float aspect_ratio, LineMesh& mesh, bool draw_normals) {
    float tan_half_fov = std::tan(camera.fov / 2.0f);

    // Dimensions of the near/far quads divided by 2
    float near_height = camera.near_distance * tan_half_fov;
    float near_width = near_height * aspect_ratio;
    vec3 near_center = camera.near_distance * camera.direction;
    vec3 near_up = near_height * camera.up;
    vec3 near_right = near_width * camera.right;

    float far_height = camera.far_distance * tan_half_fov;
    float far_width = far_height * aspect_ratio;
    vec3 far_center = camera.far_distance * camera.direction;
    vec3 far_up = far_height * camera.up;
    vec3 far_right = far_width * camera.right;

    near_plane.normal = camera.direction;
    far_plane.normal = -camera.direction;
    top_plane.normal = normalize(cross(far_center + far_up, camera.right));
    bottom_plane.normal = normalize(cross(camera.right, far_center - far_up));
    left_plane.normal = normalize(cross(far_center - far_right, camera.up));
    right_plane.normal = normalize(cross(camera.up, far_center + far_right));

    near_plane.distance = dot(camera.position + near_center, near_plane.normal);
    far_plane.distance = dot(camera.position + far_center, far_plane.normal);
    top_plane.distance = dot(camera.position, top_plane.normal);
    bottom_plane.distance = dot(camera.position, bottom_plane.normal);
    left_plane.distance = dot(camera.position, left_plane.normal);
    right_plane.distance = dot(camera.position, right_plane.normal);

    /* Mesh */
    mesh.add_vertex(camera.position); // 0

    mesh.add_vertex(camera.position + near_center - near_right + near_up); // 1
    mesh.add_vertex(camera.position + near_center - near_right - near_up); // 2
    mesh.add_vertex(camera.position + near_center + near_right - near_up); // 3
    mesh.add_vertex(camera.position + near_center + near_right + near_up); // 4

    mesh.add_vertex(camera.position + far_center - far_right + far_up); // 5
    mesh.add_vertex(camera.position + far_center - far_right - far_up); // 6
    mesh.add_vertex(camera.position + far_center + far_right - far_up); // 7
    mesh.add_vertex(camera.position + far_center + far_right + far_up); // 8

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

    if(draw_normals) {
        const float normal_length = (camera.far_distance - camera.near_distance) / 3.0f;

        // Near
        vec3 pos = camera.near_distance * camera.direction;
        vec3 color(0.5f, 0.5f, 1.0f);
        mesh.add_vertex(pos, color);                                    // 9
        mesh.add_vertex(pos + normal_length * camera.direction, color); // 10
        mesh.add_line(9, 10);

        // Far
        pos = far_center;
        color = vec3(0.5f, 0.5f, 0.0f);
        mesh.add_vertex(pos, color);                                    // 11
        mesh.add_vertex(pos - normal_length * camera.direction, color); // 12
        mesh.add_line(11, 12);

        // Top
        pos = (far_center + far_up) / 2.0f;
        color = vec3(0.5f, 1.0f, 0.5f);
        mesh.add_vertex(pos, color);                                                                       // 13
        mesh.add_vertex(pos + normal_length * normalize(cross(far_center + far_up, camera.right)), color); // 14
        mesh.add_line(13, 14);

        // Bottom
        pos = (far_center - far_up) / 2.0f;
        color = vec3(0.5f, 0.0f, 0.5f);
        mesh.add_vertex(pos, color);                                                                       // 15
        mesh.add_vertex(pos + normal_length * normalize(cross(camera.right, far_center - far_up)), color); // 16
        mesh.add_line(15, 16);

        // Left
        pos = (far_center - far_right) / 2.0f;
        color = vec3(0.0f, 0.5f, 0.5f);
        mesh.add_vertex(pos, color);                                                                       // 17
        mesh.add_vertex(pos + normal_length * normalize(cross(far_center - far_right, camera.up)), color); // 18
        mesh.add_line(17, 18);

        // Right
        pos = (far_center + far_right) / 2.0f;
        color = vec3(1.0f, 0.5f, 0.5f);
        mesh.add_vertex(pos, color);                                                                       // 19
        mesh.add_vertex(pos + normal_length * normalize(cross(camera.up, far_center + far_right)), color); // 20
        mesh.add_line(19, 20);
    }
}
