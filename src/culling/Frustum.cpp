/***************************************************************************************************
 * @file  Frustum.cpp
 * @brief Implementation of the Frustum struct
 **************************************************************************************************/

#include "culling/Frustum.hpp"

#include <cmath>
#include "maths/geometry.hpp"
#include "Window.hpp"

Frustum::Frustum(const Camera& camera) {
    update(camera);
}

Frustum::Frustum(const Camera& camera, LineMesh& lines, TriangleMesh& faces) {
    float tan_half_fov = std::tan(camera.fov / 2.0f);

    // Dimensions of the near/far quads divided by 2
    float near_height = camera.near_distance * tan_half_fov;
    float near_width = near_height * Window::get_aspect_ratio();
    vec3 near_center = camera.near_distance * camera.direction;
    vec3 near_up = near_height * camera.up;
    vec3 near_right = near_width * camera.right;

    float far_height = camera.far_distance * tan_half_fov;
    float far_width = far_height * Window::get_aspect_ratio();
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

    /* Meshes */
    const vec3 far_plane_corners[4]{
        camera.position + far_center - far_right + far_up,
        camera.position + far_center - far_right - far_up,
        camera.position + far_center + far_right - far_up,
        camera.position + far_center + far_right + far_up
    };

    lines.add_vertex(camera.position); // 0

    lines.add_vertex(camera.position + near_center - near_right + near_up); // 1
    lines.add_vertex(camera.position + near_center - near_right - near_up); // 2
    lines.add_vertex(camera.position + near_center + near_right - near_up); // 3
    lines.add_vertex(camera.position + near_center + near_right + near_up); // 4

    lines.add_vertex(far_plane_corners[0]); // 5
    lines.add_vertex(far_plane_corners[1]); // 6
    lines.add_vertex(far_plane_corners[2]); // 7
    lines.add_vertex(far_plane_corners[3]); // 8

    lines.add_line(0, 5);
    lines.add_line(0, 6);
    lines.add_line(0, 7);
    lines.add_line(0, 8);

    lines.add_line(1, 2);
    lines.add_line(2, 3);
    lines.add_line(3, 4);
    lines.add_line(4, 1);

    lines.add_line(5, 6);
    lines.add_line(6, 7);
    lines.add_line(7, 8);
    lines.add_line(8, 5);

    faces.add_vertex(camera.position, vec3(), vec2());      // 0
    faces.add_vertex(far_plane_corners[0], vec3(), vec2()); // 1
    faces.add_vertex(far_plane_corners[1], vec3(), vec2()); // 2
    faces.add_vertex(far_plane_corners[2], vec3(), vec2()); // 3
    faces.add_vertex(far_plane_corners[3], vec3(), vec2()); // 4

    faces.add_face(4, 3, 2, 1);  // FAR
    faces.add_triangle(1, 0, 4); // TOP
    faces.add_triangle(0, 2, 3); // BOTTOM
    faces.add_triangle(1, 2, 0); // LEFT
    faces.add_triangle(4, 0, 3); // RIGHT

    lines.bind_buffers();
    faces.bind_buffers();
}

void Frustum::update(const Camera& camera) {
    float far_height = camera.far_distance * std::tan(camera.fov / 2.0f);
    vec3 far_center = camera.far_distance * camera.direction;
    vec3 far_up = far_height * camera.up;
    vec3 far_right = far_height * Window::get_aspect_ratio() * camera.right;

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
