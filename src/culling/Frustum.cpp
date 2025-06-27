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

    near_plane.distance = -camera.near_distance;
    far_plane.distance = -camera.far_distance;
    top_plane.distance = bottom_plane.distance = left_plane.distance = right_plane.distance = 0.0f;

    near_plane.normal = camera.direction;
    far_plane.normal = -camera.direction;
    top_plane.normal = normalize(cross(far_center + far_up, camera.right));
    bottom_plane.normal = normalize(cross(camera.right, far_center - far_up));
    left_plane.normal = normalize(cross(far_center - far_right, camera.up));
    right_plane.normal = normalize(cross(camera.up, far_center + far_right));
}

Frustum::Frustum(const Camera& camera, float aspect_ratio, LineMesh& mesh) {
    float tan_half_fov = std::tan(camera.fov / 2.0f);

    // Dimensions of the near/far quads divided by 2
    float near_height = camera.near_distance * tan_half_fov;
    float near_width = near_height * aspect_ratio;
    float far_height = camera.far_distance * tan_half_fov;
    float far_width = far_height * aspect_ratio;

    vec3 far_center = camera.far_distance * camera.direction;
    vec3 far_up = far_height * camera.up;
    vec3 far_right = far_width * camera.right;

    near_plane.distance = -camera.near_distance;
    far_plane.distance = -camera.far_distance;
    top_plane.distance = bottom_plane.distance = left_plane.distance = right_plane.distance = 0.0f;

    near_plane.normal = camera.direction;
    far_plane.normal = -camera.direction;
    top_plane.normal = normalize(cross(far_center + far_up, camera.right));
    bottom_plane.normal = normalize(cross(camera.right, far_center - far_up));
    left_plane.normal = normalize(cross(far_center - far_right, camera.up));
    right_plane.normal = normalize(cross(camera.up, far_center + far_right));

    /* Mesh */
    const float normal_length = (camera.far_distance - camera.near_distance) / 3.0f;

    mesh.add_vertex(vec3(0.0f)); // 0

    mesh.add_vertex(vec3(-near_width, near_height, -camera.near_distance));  // 1
    mesh.add_vertex(vec3(-near_width, -near_height, -camera.near_distance)); // 2
    mesh.add_vertex(vec3(near_width, -near_height, -camera.near_distance));  // 3
    mesh.add_vertex(vec3(near_width, near_height, -camera.near_distance));   // 4

    mesh.add_vertex(vec3(-far_width, far_height, -camera.far_distance));  // 5
    mesh.add_vertex(vec3(-far_width, -far_height, -camera.far_distance)); // 6
    mesh.add_vertex(vec3(far_width, -far_height, -camera.far_distance));  // 7
    mesh.add_vertex(vec3(far_width, far_height, -camera.far_distance));   // 8

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
