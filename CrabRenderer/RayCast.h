#pragma once

namespace crab
{

struct RayCast
{
    static Ray CreateRayFromScreenPoint(const Vec2Int& in_screenPoint,
                                        const Vec2Int& in_screenResolution,
                                        const Vec3&    in_cameraPos,
                                        const Mat4&    in_viewProj);

    static Ray CreateRayFromNDCPoint(const Vec2& in_ndcPoint,
                                     const Vec3& in_cameraPos,
                                     const Mat4& in_viewProj);

};

}   // namespace crab
