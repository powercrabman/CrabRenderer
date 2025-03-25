#include "InternalPCH.h"

#define NOMINMAX

#include "LinearAlgebraUtils.h"

#include "Log.h"

#include <algorithm>
#include <array>
#include <string>

namespace crab
{

namespace vector_util
{

    Vec2 ScreenPointToNDC(const uint32 in_xInScreen,
                          const uint32 in_yInScreen,
                          const uint32 in_screenWidth,
                          const uint32 in_screenHeight)
    {
        Vec2 result;
        result.x = (2.f * in_xInScreen) / static_cast<float>(in_screenWidth) - 1.f;
        result.y = 1.f - (2.f * in_yInScreen) / static_cast<float>(in_screenHeight);
        return result;
    }

    Vec2 Normalize(const Vec2& in_vector)
    {
        Vec2 result = in_vector;
        result.Normalize();
        return result;
    }

    Vec3 Normalize(const Vec3& in_vector)
    {
        Vec3 result = in_vector;
        result.Normalize();
        return result;
    }

    Vec4 Normalize(const Vec4& in_vector)
    {
        Vec4 result = in_vector;
        result.Normalize();
        return result;
    }

    Vec2 ToVec2(const Vec3& in_vector)
    {
        return Vec2(in_vector.x, in_vector.y);
    }

    Vec2 ToVec2(const Vec4& in_vector)
    {
        return Vec2(in_vector.x, in_vector.y);
    }

    Vec3 ToVec3(const Vec2& in_vector, float z)
    {
        return Vec3(in_vector.x, in_vector.y, z);
    }

    Vec3 ToVec3(const Vec4& in_vector)
    {
        return Vec3(in_vector.x, in_vector.y, in_vector.z);
    }

    Vec4 ToVec4(const Vec2& in_vector, float z, float w)
    {
        return Vec4(in_vector.x, in_vector.y, z, w);
    }

    Vec4 ToVec4(const Vec2& in_vector, Vec2& in_zw)
    {
        return Vec4(in_vector.x, in_vector.y, in_zw.x, in_zw.y);
    }

    Vec4 ToVec4(const Vec3& in_vector, float w)
    {
        return Vec4(in_vector.x, in_vector.y, in_vector.z, w);
    }

}   // namespace vector_util

namespace matrix_util
{

    Mat4 CreateWorldFromQuaternion(const Vec3& in_position, const Quat& in_rotate, const Vec3& in_scale)
    {
        return DirectX::XMMatrixAffineTransformation(
            in_scale,
            Vec3::Zero,
            in_rotate,
            in_position);
    }

    Mat4 CreateWorldFromEuler(const Vec3& in_position, const Vec3& in_yawPitchRoll, const Vec3& in_scale)
    {
        Quat rQuat = Quat::CreateFromYawPitchRoll(in_yawPitchRoll);
        return CreateWorldFromQuaternion(in_position, rQuat, in_scale);
    }

    Mat4 CreateWorldFromLookDirection(const Vec3& in_position, const Vec3& in_lookDirection, const Vec3& in_upVector, const Vec3& in_scale)
    {
        return Mat4::CreateScale(in_scale) * Mat4::CreateWorld(in_position, in_lookDirection, in_upVector);
    }

    Mat4 CreateViewLookDirection(const Vec3& in_position, const Vec3& in_lookDirection, const Vec3& in_upVector)
    {
        return DirectX::XMMatrixLookToLH(
            in_position,
            in_lookDirection,
            in_upVector);
    }

    Mat4 CreateViewFocusPoint(const Vec3& in_position, const Vec3& in_focusPoint, const Vec3& in_upVector)
    {
        return DirectX::XMMatrixLookAtLH(
            in_position,
            in_focusPoint,
            in_upVector);
    }

    Mat4 CreateViewFromQuaternion(const Vec3& in_position, const Quat& in_quaternion, const Vec3& in_upVector)
    {
        Mat4 rMat = Mat4::CreateFromQuaternion(in_quaternion);
        return CreateViewLookDirection(
            in_position,
            rMat.Backward(),
            in_upVector);
    }

    Mat4 CreateViewFromEuler(const Vec3& in_position, const Vec3& in_yawPitchRoll, const Vec3& in_upVector)
    {
        Mat4 rMat = Mat4::CreateFromYawPitchRoll(in_yawPitchRoll);
        return CreateViewLookDirection(
            in_position,
            rMat.Backward(),
            in_upVector);
    }

    Mat4 CreatePerspective(float in_fov, float in_aspectRatio, float in_nearZ, float in_farZ)
    {
        return DirectX::XMMatrixPerspectiveFovLH(
            in_fov,
            in_aspectRatio,
            in_nearZ,
            in_farZ);
    }

    Mat4 CreateOrthographic(float in_width, float in_height, float in_nearZ, float in_farZ)
    {
        return DirectX::XMMatrixOrthographicLH(
            in_width,
            in_height,
            in_nearZ,
            in_farZ);
    }

    Mat4 CreateOrthographicAspect(float in_aspectRatio, float in_height, float in_nearZ, float in_farZ)
    {
        return DirectX::XMMatrixOrthographicLH(
            in_height * in_aspectRatio,
            in_height,
            in_nearZ,
            in_farZ);
    }

    Mat4 CreateOrthographicOffCenter(float in_left, float in_right, float in_bottom, float in_top, float in_nearZ, float in_farZ)
    {
        return DirectX::XMMatrixOrthographicOffCenterLH(
            in_left,
            in_right,
            in_bottom,
            in_top,
            in_nearZ,
            in_farZ);
    }

    std::vector<Mat4> CreateOrthographicForCSM(
        const Frustum&            in_viewFrustum,
        const std::vector<float>& in_cascadeRanges,
        const Vec3&               in_lightLookDirection,
        float                     in_cameraDistFromCenter)
    {
        ASSERT(std::ranges::is_sorted(in_cascadeRanges), "Cascade ranges must be sorted.");

        uint64               cascadeLevels   = in_cascadeRanges.size();
        std::vector<Frustum> cascadeFrustums = frustum_util::DivideCascade(in_viewFrustum, in_cascadeRanges);
        ASSERT(cascadeFrustums.size() == cascadeLevels, "Cascade frustums size is not correct");

        std::vector<Mat4> shadowViewProjs;
        shadowViewProjs.resize(cascadeLevels);

        struct OrthoGraphicDesc
        {
            float minX, maxX, minY, maxY;
            Mat4  lightViewMat;
        };
        std::vector<OrthoGraphicDesc> orthoDescs;
        orthoDescs.resize(cascadeLevels);

        float cascadeMinZ = std::numeric_limits<float>::max();
        float cascadeMaxZ = std::numeric_limits<float>::min();

        for (size_t i = 0; i < cascadeLevels; ++i)
        {
            Mat4 lightView = CreateViewLookDirection(
                frustum_util::GetCenter(cascadeFrustums[i]) + -in_lightLookDirection * in_cameraDistFromCenter,
                in_lightLookDirection);

            Frustum frustum = cascadeFrustums[i];
            frustum.Transform(frustum, lightView);

            std::array<Vec3, 8> corners;
            frustum.GetCorners(corners.data());

            float minX, maxX, minY, maxY;
            minX = minY = std::numeric_limits<float>::max();
            maxX = maxY = std::numeric_limits<float>::min();

            for (uint32 i = 0; i < 8; i++)
            {
                Vec3 corner = corners[i];

                minX        = std::min(minX, corner.x);
                maxX        = std::max(maxX, corner.x);
                minY        = std::min(minY, corner.y);
                maxY        = std::max(maxY, corner.y);
                cascadeMinZ = std::min(cascadeMinZ, corner.z);
                cascadeMaxZ = std::max(cascadeMaxZ, corner.z);
            }

            orthoDescs[i] = { minX, maxX, minY, maxY, lightView };
        }

        for (size_t i = 0; i < cascadeLevels; ++i)
        {
            shadowViewProjs[i] =
                orthoDescs[i].lightViewMat
                * CreateOrthographicOffCenter(
                    orthoDescs[i].minX,
                    orthoDescs[i].maxX,
                    orthoDescs[i].minY,
                    orthoDescs[i].maxY,
                    cascadeMinZ,
                    cascadeMaxZ);
        }

        return shadowViewProjs;
    }
}   // namespace matrix_util

namespace frustum_util
{

    Frustum CreateFromMatrixInLocal(const Mat4& in_projection)
    {
        return Frustum(in_projection);
    }

    Frustum CreateFromMatrixInWorld(const Vec3& in_position, const Quat& in_rotate, const Mat4& in_projection)
    {
        Frustum frustum(in_projection);
        frustum.Transform(frustum, matrix_util::CreateWorldFromQuaternion(in_position, in_rotate));
        return frustum;
    }

    Frustum CreateFromMatrixInWorld(const Vec3& in_position, const Vec3& in_yawPitchRoll, const Mat4& in_projection)
    {
        Frustum frustum(in_projection);
        frustum.Transform(frustum, matrix_util::CreateWorldFromEuler(in_position, in_yawPitchRoll));
        return frustum;
    }

    Frustum CreateFromMatrixInWorld(const Vec3& in_position, const Vec3& in_lookDirection, const Vec3& in_upVector, const Mat4& in_projection)
    {
        Frustum frustum(in_projection);
        frustum.Transform(
            frustum,
            matrix_util::CreateViewLookDirection(
                in_position,
                in_lookDirection,
                in_upVector));
        return frustum;
    }

    std::array<Vec3, 8> GetCorners(const Frustum& in_frustum)
    {
        std::array<Vec3, 8> corners;
        in_frustum.GetCorners(corners.data());
        return corners;
    }

    Vec3 GetCenter(const Frustum& in_frustum)
    {
        auto corners = GetCorners(in_frustum);
        Vec3 center  = Vec3::Zero;
        for (const auto& corner: corners)
            center += corner;
        return center / 8.f;
    }

    std::vector<Frustum> DivideCascade(const Frustum& in_frustum, const std::vector<float>& in_cascadeRanges)
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

}   // namespace frustum_util

}   // namespace crab