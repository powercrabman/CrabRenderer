#pragma once
#include "MathCommon.h"

namespace crab
{

namespace vector_util
{

    Vec2 ScreenPointToNDC(uint32 in_xInScreen,
                          uint32 in_yInScreen,
                          uint32 in_screenWidth,
                          uint32 in_screenHeight);

    Vec2 Normalize(const Vec2& in_vector);
    Vec3 Normalize(const Vec3& in_vector);
    Vec4 Normalize(const Vec4& in_vector);

    Vec2 ToVec2(const Vec3& in_vector);
    Vec2 ToVec2(const Vec4& in_vector);

    Vec3 ToVec3(const Vec2& in_vector, float z);
    Vec3 ToVec3(const Vec4& in_vector);

    Vec4 ToVec4(const Vec2& in_vector, float z, float w);
    Vec4 ToVec4(const Vec2& in_vector, Vec2& in_zw);
    Vec4 ToVec4(const Vec3& in_vector, float w);

}   // namespace vector

namespace matrix_util
{
    Mat4 CreateWorldFromQuaternion(
        const Vec3& in_position,
        const Quat& in_rotate,
        const Vec3& in_scale = Vec3::One);

    Mat4 CreateWorldFromEuler(
        const Vec3& in_position,
        const Vec3& in_yawPitchRoll,
        const Vec3& in_scale = Vec3::One);

    Mat4 CreateWorldFromLookDirection(
        const Vec3& in_position,
        const Vec3& in_lookDirection,
        const Vec3& in_upVector = Vec3::Up,
        const Vec3& in_scale    = Vec3::One);

    Mat4 CreateViewLookDirection(
        const Vec3& in_position,
        const Vec3& in_lookDirection,
        const Vec3& in_upVector = Vec3::Up);

    Mat4 CreateViewFocusPoint(
        const Vec3& in_position,
        const Vec3& in_focusPoint,
        const Vec3& in_upVector = Vec3::Up);

    Mat4 CreateViewFromQuaternion(
        const Vec3& in_position,
        const Quat& in_quaternion,
        const Vec3& in_upVector = Vec3::Up);

    // Yaw Pitch Roll
    Mat4 CreateViewFromEuler(
        const Vec3& in_position,
        const Vec3& in_yawPitchRoll,
        const Vec3& in_upVector = Vec3::Up);

    Mat4 CreatePerspective(
        float in_fov,
        float in_aspectRatio,
        float in_nearZ,
        float in_farZ);

    Mat4 CreateOrthographic(
        float in_width,
        float in_height,
        float in_nearZ,
        float in_farZ);

    Mat4 CreateOrthographicAspect(
        float in_aspectRatio,
        float in_height,
        float in_nearZ,
        float in_farZ);

    Mat4 CreateOrthographicOffCenter(
        float in_left,
        float in_right,
        float in_bottom,
        float in_top,
        float in_nearZ,
        float in_farZ);

    std::vector<Mat4> CreateOrthographicForCSM(
        const Frustum&            in_viewFrustum,
        const std::vector<float>& in_cascadeRanges,
        const Vec3&               in_lightLookDirection,
        float                     in_cameraDistFromCenter);
}

namespace frustum_util
{
    Frustum CreateFromMatrixInLocal(const Mat4& in_projection);

    Frustum CreateFromMatrixInWorld(
        const Vec3& in_position,
        const Quat& in_rotate,
        const Mat4& in_projection);

    Frustum CreateFromMatrixInWorld(
        const Vec3& in_position,
        const Vec3& in_yawPitchRoll,
        const Mat4& in_projection);

    Frustum CreateFromMatrixInWorld(
        const Vec3& in_position,
        const Vec3& in_lookDirection,
        const Vec3& in_upVector,
        const Mat4& in_projection);

    std::vector<Frustum> DivideCascade(
        const Frustum&            in_frustum,
        const std::vector<float>& in_cascadeRanges);

    std::array<Vec3, 8> GetCorners(const Frustum& in_frustum);

    Vec3 GetCenter(const Frustum& in_frustum);
}

};   // namespace crab