/***************************************************************************************************
 * @file  BoundingVolume.hpp
 * @brief Declaration of the BoundingVolume struct
 **************************************************************************************************/

#pragma once

#include "Frustum.hpp"
#include "../maths/Transform.hpp"

/**
 * @struct BoundingVolume
 * @brief
 */
struct BoundingVolume {
    virtual ~BoundingVolume() = default;

    bool is_in_frustum(const Frustum& frustum) const;
    virtual bool is_in_frustum(const Frustum& frustum, const Transform& transform) const = 0;
    virtual bool is_in_or_above_plane(const Plane& plane) const = 0;
};

struct SphereVolume final : BoundingVolume {
    SphereVolume();

    SphereVolume(const vec3& center, float radius);

    SphereVolume inline get_global_volumue(const Transform& transform) const;
    bool is_in_frustum(const Frustum& frustum, const Transform& transform) const override;
    bool is_in_or_above_plane(const Plane& plane) const override;

    vec3 center;
    float radius;
};

struct AABB final : BoundingVolume {
    AABB();
    AABB(const vec3& min, const vec3& max);
    AABB(const vec3& center, float extent_x, float extent_y, float extent_z);

    AABB inline get_global_volumue(const Transform& transform) const;
    bool is_in_frustum(const Frustum& frustum, const Transform& transform) const override;
    bool is_in_or_above_plane(const Plane& plane) const override;

    vec3 center;
    vec3 extents;
};
