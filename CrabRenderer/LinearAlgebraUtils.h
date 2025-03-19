#pragma once
#include "ErrorHandler.h"

namespace crab
{

//===================================================
// Vector Util
//===================================================

class VectorUtil
{
public:
    static Vec2 ScreenPointToNDC(
        const Int2& in_screenPoint,
        const Int2& in_screenResolution);

    static Vec2 Normalize(const Vec2& in_vector);
    static Vec3 Normalize(const Vec3& in_vector);
    static Vec4 Normalize(const Vec4& in_vector);

    static Vec2 ToVec2(const Vec3& in_vector);
    static Vec2 ToVec2(const Vec4& in_vector);

    static Vec3 ToVec3(const Vec2& in_vector, float z);
    static Vec3 ToVec3(const Vec4& in_vector);

    static Vec4 ToVec4(const Vec2& in_vector, float z, float w);
    static Vec4 ToVec4(const Vec2& in_vector, Vec2& in_zw);
    static Vec4 ToVec4(const Vec3& in_vector, float w);
};

//===================================================
// Matrix Util
// >> Left-hand coordinate system <<
//===================================================

class MatrixUtil
{
public:
    static Mat4 CreateWorldFromQuaternion(
        const Vec3& in_position,
        const Quat& in_rotate,
        const Vec3& in_scale = Vec3::One);

    static Mat4 CreateWorldFromEuler(
        const Vec3& in_position,
        const Vec3& in_yawPitchRoll,
        const Vec3& in_scale = Vec3::One);

    static Mat4 CreateWorldFromLookDirection(
        const Vec3& in_position,
        const Vec3& in_lookDirection,
        const Vec3& in_upVector = Vec3::Up,
        const Vec3& in_scale    = Vec3::One);

    static Mat4 CreateViewLookDirection(
        const Vec3& in_position,
        const Vec3& in_lookDirection,
        const Vec3& in_upVector = Vec3::Up);

    static Mat4 CreateViewFocusPoint(
        const Vec3& in_position,
        const Vec3& in_focusPoint,
        const Vec3& in_upVector = Vec3::Up);

    static Mat4 CreateViewFromQuaternion(
        const Vec3& in_position,
        const Quat& in_quaternion,
        const Vec3& in_upVector = Vec3::Up);

    // Yaw Pitch Roll
    static Mat4 CreateViewFromEuler(
        const Vec3& in_position,
        const Vec3& in_yawPitchRoll,
        const Vec3& in_upVector = Vec3::Up);

    static Mat4 CreatePerspective(
        float in_fov,
        float in_aspectRatio,
        float in_nearZ,
        float in_farZ);

    static Mat4 CreateOrthographic(
        float in_width,
        float in_height,
        float in_nearZ,
        float in_farZ);

    static Mat4 CreateOrthographicAspect(
        float in_aspectRatio,
        float in_height,
        float in_nearZ,
        float in_farZ);

    static Mat4 CreateOrthographicOffCenter(
        float in_left,
        float in_right,
        float in_bottom,
        float in_top,
        float in_nearZ,
        float in_farZ);

    static std::vector<Mat4> CreateOrthographicForCSM(
        const Frustum&            in_viewFrustum,
        const std::vector<float>& in_cascadeRanges,
        const Vec3&               in_lightLookDirection,
        float                     in_cameraDistFromCenter);
};

//===================================================
// Frustum Util
//===================================================

class FrustumUtil
{
public:
    static Frustum CreateFromMatrixInLocal(const Mat4& in_projection)
    {
        return Frustum(in_projection);
    }

    static Frustum CreateFromMatrixInWorld(
        const Vec3& in_position,
        const Quat& in_rotate,
        const Mat4& in_projection)
    {
        Frustum frustum(in_projection);
        frustum.Transform(frustum, MatrixUtil::CreateWorldFromQuaternion(in_position, in_rotate));
        return frustum;
    }

    static Frustum CreateFromMatrixInWorld(
        const Vec3& in_position,
        const Vec3& in_yawPitchRoll,
        const Mat4& in_projection)
    {
        Frustum frustum(in_projection);
        frustum.Transform(frustum, MatrixUtil::CreateWorldFromEuler(in_position, in_yawPitchRoll));
        return frustum;
    }

    static Frustum CreateFromMatrixInWorld(
        const Vec3& in_position,
        const Vec3& in_lookDirection,
        const Vec3& in_upVector,
        const Mat4& in_projection)
    {
        Frustum frustum(in_projection);
        frustum.Transform(
            frustum,
            MatrixUtil::CreateViewLookDirection(
                in_position,
                in_lookDirection,
                in_upVector));
        return frustum;
    }

    static std::vector<Frustum> DivideCascade(
        const Frustum&            in_frustum,
        const std::vector<float>& in_cascadeRanges)
    {
        ASSERT(std::ranges::is_sorted(in_cascadeRanges), "Cascade ranges must be sorted.");

        std::vector<Frustum> frustums;
        frustums.reserve(in_cascadeRanges.size() - 1);
        for (size_t i = 0; i < in_cascadeRanges.size() - 1; ++i)
        {
            Frustum frustum(in_frustum);
            frustum.Near = in_cascadeRanges[i];
            frustum.Far  = in_cascadeRanges[i + 1];
            frustums.push_back(frustum);
        }
        return frustums;
    }

    static std::array<Vec3, 8> GetCorners(const Frustum& in_frustum)
    {
        std::array<Vec3, 8> corners;
        in_frustum.GetCorners(corners.data());
        return corners;
    }

    static Vec3 GetCenter(const Frustum& in_frustum)
    {
        auto corners = GetCorners(in_frustum);
        Vec3 center  = Vec3::Zero;
        for (const auto& corner: corners)
            center += corner;
        return center / 8.f;
    }
};

};   // namespace crab