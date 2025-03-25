#include "InternalPCH.h"

#include "RayCast.h"

namespace crab
{

Ray RayCast::CreateRayFromScreenPoint(uint32      in_xInScreen,
                                      uint32      in_yInScreen,
                                      uint32      in_screenWidth,
                                      uint32      in_screenHeight,
                                      const Vec3& in_cameraPos,
                                      const Mat4& in_viewProj)
{
    Vec2 posNDC;
    posNDC.x = 2.f * static_cast<float>(in_xInScreen) / static_cast<float>(in_screenWidth) - 1.0f;
    posNDC.y = -2.f * static_cast<float>(in_yInScreen) / static_cast<float>(in_screenHeight) + 1.0f;

    return CreateRayFromNDCPoint(Vec2(posNDC.x, posNDC.y), in_cameraPos, in_viewProj);
}

Ray RayCast::CreateRayFromNDCPoint(const Vec2& in_ndcPoint,
                                   const Vec3& in_cameraPos,
                                   const Mat4& in_viewProj)
{
    Vec4 posNDC   = Vec4(in_ndcPoint.x, in_ndcPoint.y, 0.0f, 0.0f);
    Vec4 posWorld = Vec4::Transform(posNDC, in_viewProj.Invert());
    Ray  ray;
    ray.position  = Vec3(posWorld.x, posWorld.y, posWorld.z);
    ray.direction = vector_util::Normalize(ray.position - in_cameraPos);
    return ray;
}

}   // namespace crab