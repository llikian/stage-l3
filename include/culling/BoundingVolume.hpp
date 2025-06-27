/***************************************************************************************************
 * @file  BoundingVolume.hpp
 * @brief Declaration of the BoundingVolume struct
 **************************************************************************************************/

#pragma once

#include "Frustum.hpp"
#include "scene_graph/Transform.hpp"

/**
 * @struct BoundingVolume
 * @brief
 */
struct BoundingVolume {
    virtual ~BoundingVolume() = default;
    virtual bool is_in_frustum(const Frustum& frustum, const Transform& transform) = 0;
    virtual bool is_in_or_above_plane(const Plane& plane) const = 0;
};

struct SphereVolume : public BoundingVolume {
    SphereVolume();

    SphereVolume(const vec3& center, float radius);

    bool is_in_frustum(const Frustum& frustum, const Transform& transform) override;
    bool is_in_or_above_plane(const Plane& plane) const override;

    vec3 center;
    float radius;
};

struct AABB : public BoundingVolume {
    AABB();
    AABB(const vec3& min, const vec3& max);
    AABB(const vec3& center, float extent_x, float extent_y, float extent_z);

    bool is_in_frustum(const Frustum& frustum, const Transform& transform) override;
    bool is_in_or_above_plane(const Plane& plane) const override;

    vec3 center;
    vec3 extents;
};
