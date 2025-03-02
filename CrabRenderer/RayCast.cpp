#include "CrabPch.h"

#include "RayCast.h"

namespace crab
{

Ray RayCast::CreateRayFromScreenPoint(
    const Int2& in_screenPoint,
    const Int2& in_screenResolution,
    const Vec3& in_cameraPos,
    const Mat4& in_viewProj)
{
    Vec2 posNDC;
    posNDC.x = 2.f * in_screenPoint.x / static_cast<float>(in_screenResolution.x) - 1.0f;
    posNDC.y = -2.f * in_screenPoint.y / static_cast<float>(in_screenResolution.y) + 1.0f;

    return CreateRayFromNDCPoint(Vec2(posNDC.x, posNDC.y), in_cameraPos, in_viewProj);
}

Ray RayCast::CreateRayFromNDCPoint(
    const Vec2& in_ndcPoint,
    const Vec3& in_cameraPos,
    const Mat4& in_viewProj)
{
    Vec4 posNDC   = Vec4(in_ndcPoint.x, in_ndcPoint.y, 0.0f, 0.0f);
    Vec4 posWorld = Vec4::Transform(posNDC, in_viewProj.Invert());
    Ray  ray;
    ray.position  = Vec3(posWorld.x, posWorld.y, posWorld.z);
    ray.direction = Normalize(ray.position - in_cameraPos);
    return ray;
}

}   // namespace crab