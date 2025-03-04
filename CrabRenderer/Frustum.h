#pragma once

#include "CrabTypes.h"
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/SimpleMath.inl>
#include <array>

namespace crab
{

struct Frustum
{
    Frustum() = default;

    static Frustum CreateFromViewProj(const Mat4& in_viewProj)
    {
        Frustum frustum = {};

        frustum.points[0] = Vec3(-1, -1, 0);
        frustum.points[1] = Vec3(1, -1, 0);
        frustum.points[2] = Vec3(1, 1, 0);
        frustum.points[3] = Vec3(-1, 1, 0);

        frustum.points[4] = Vec3(-1, -1, 1);
        frustum.points[5] = Vec3(1, -1, 1);
        frustum.points[6] = Vec3(1, 1, 1);
        frustum.points[7] = Vec3(-1, 1, 1);

        Mat4 invViewProj = in_viewProj.Invert();

        for (auto& p: frustum.points)
            p = Vec3::Transform(p, invViewProj);

        return frustum;
    }

    Frustum operator*(const Mat4& in_mat) const
    {
        Frustum result = {};
        for (uint32 i = 0; i < 8; i++)
            result.points[i] = Vec3::Transform(points[i], in_mat);
        return result;
    }

    Vec3 GetCenter() const
    {
        Vec3 result = Vec3::Zero;
        for (const auto& p: points)
            result += p;

        return result * 0.125f;
    }

    Vec3 GetSize() const
    {
        Vec3 min = points[0];
        Vec3 max = points[0];
        for (const auto& p: points)
        {
            min = Vec3::Min(min, p);
            max = Vec3::Max(max, p);
        }

        return max - min;
    }

    std::array<Vec3, 8> points = {};
};

}   // namespace crab