#pragma once

namespace crab
{

struct RayCast
{
    static Ray CreateRayFromScreenPoint(
        uint32      in_xInScreen,
        uint32      in_yInScreen,
        uint32      in_screenWidth,
        uint32      in_screenHeight,
        const Vec3& in_cameraPos,
        const Mat4& in_viewProj);

    static Ray CreateRayFromNDCPoint(
        const Vec2& in_ndcPoint,
        const Vec3& in_cameraPos,
        const Mat4& in_viewProj);
};

}   // namespace crab
