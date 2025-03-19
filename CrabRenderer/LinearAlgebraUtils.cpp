#include "CrabPch.h"

#include "LinearAlgebraUtils.h"

namespace crab
{

//===================================================
// Vector Util
//===================================================

Vec2 VectorUtil::ScreenPointToNDC(
    const Int2& in_screenPoint,
    const Int2& in_screenResolution)
{
    Vec2 result;
    result.x = (2.f * in_screenPoint.x) / in_screenResolution.x - 1.f;
    result.y = 1.f - (2.f * in_screenPoint.y) / in_screenResolution.y;
    return result;
}

Vec2 VectorUtil::Normalize(const Vec2& in_vector)
{
    Vec2 result = in_vector;
    result.Normalize();
    return result;
}

Vec3 VectorUtil::Normalize(const Vec3& in_vector)
{
    Vec3 result = in_vector;
    result.Normalize();
    return result;
}

Vec4 VectorUtil::Normalize(const Vec4& in_vector)
{
    Vec4 result = in_vector;
    result.Normalize();
    return result;
}

Vec2 VectorUtil::ToVec2(const Vec3& in_vector)
{
    return Vec2(in_vector.x, in_vector.y);
}

Vec2 VectorUtil::ToVec2(const Vec4& in_vector)
{
    return Vec2(in_vector.x, in_vector.y);
}

Vec3 VectorUtil::ToVec3(const Vec2& in_vector, float z)
{
    return Vec3(in_vector.x, in_vector.y, z);
}

Vec3 VectorUtil::ToVec3(const Vec4& in_vector)
{
    return Vec3(in_vector.x, in_vector.y, in_vector.z);
}

Vec4 VectorUtil::ToVec4(const Vec2& in_vector, float z, float w)
{
    return Vec4(in_vector.x, in_vector.y, z, w);
}

Vec4 VectorUtil::ToVec4(const Vec2& in_vector, Vec2& in_zw)
{
    return Vec4(in_vector.x, in_vector.y, in_zw.x, in_zw.y);
}

Vec4 VectorUtil::ToVec4(const Vec3& in_vector, float w)
{
    return Vec4(in_vector.x, in_vector.y, in_vector.z, w);
}

Mat4 MatrixUtil::CreateWorldFromQuaternion(const Vec3& in_position, const Quat& in_rotate, const Vec3& in_scale)
{
    return DirectX::XMMatrixAffineTransformation(
        in_scale,
        Vec3::Zero,
        in_rotate,
        in_position);
}

Mat4 MatrixUtil::CreateWorldFromEuler(const Vec3& in_position, const Vec3& in_yawPitchRoll, const Vec3& in_scale)
{
    Quat rQuat = Quat::CreateFromYawPitchRoll(in_yawPitchRoll);
    return CreateWorldFromQuaternion(in_position, rQuat, in_scale);
}

Mat4 MatrixUtil::CreateWorldFromLookDirection(const Vec3& in_position, const Vec3& in_lookDirection, const Vec3& in_upVector, const Vec3& in_scale)
{
    return Mat4::CreateScale(in_scale) * Mat4::CreateWorld(in_position, in_lookDirection, in_upVector);
}

//===================================================
// Matrix Util
//===================================================

Mat4 MatrixUtil::CreateViewLookDirection(const Vec3& in_position, const Vec3& in_lookDirection, const Vec3& in_upVector)
{
    return DirectX::XMMatrixLookToLH(
        in_position,
        in_lookDirection,
        in_upVector);
}

Mat4 MatrixUtil::CreateViewFocusPoint(const Vec3& in_position, const Vec3& in_focusPoint, const Vec3& in_upVector)
{
    return DirectX::XMMatrixLookAtLH(
        in_position,
        in_focusPoint,
        in_upVector);
}

Mat4 MatrixUtil::CreateViewFromQuaternion(const Vec3& in_position, const Quat& in_quaternion, const Vec3& in_upVector)
{
    Mat4 rMat = Mat4::CreateFromQuaternion(in_quaternion);
    return CreateViewLookDirection(
        in_position,
        rMat.Backward(),
        in_upVector);
}

Mat4 MatrixUtil::CreateViewFromEuler(const Vec3& in_position, const Vec3& in_yawPitchRoll, const Vec3& in_upVector)
{
    Mat4 rMat = Mat4::CreateFromYawPitchRoll(in_yawPitchRoll);
    return CreateViewLookDirection(
        in_position,
        rMat.Backward(),
        in_upVector);
}

Mat4 MatrixUtil::CreatePerspective(float in_fov, float in_aspectRatio, float in_nearZ, float in_farZ)
{
    return DirectX::XMMatrixPerspectiveFovLH(
        in_fov,
        in_aspectRatio,
        in_nearZ,
        in_farZ);
}

Mat4 MatrixUtil::CreateOrthographic(float in_width, float in_height, float in_nearZ, float in_farZ)
{
    return DirectX::XMMatrixOrthographicLH(
        in_width,
        in_height,
        in_nearZ,
        in_farZ);
}

Mat4 MatrixUtil::CreateOrthographicAspect(float in_aspectRatio, float in_height, float in_nearZ, float in_farZ)
{
    return DirectX::XMMatrixOrthographicLH(
        in_height * in_aspectRatio,
        in_height,
        in_nearZ,
        in_farZ);
}

Mat4 MatrixUtil::CreateOrthographicOffCenter(float in_left, float in_right, float in_bottom, float in_top, float in_nearZ, float in_farZ)
{
    return DirectX::XMMatrixOrthographicOffCenterLH(
        in_left,
        in_right,
        in_bottom,
        in_top,
        in_nearZ,
        in_farZ);
}

std::vector<Mat4> MatrixUtil::CreateOrthographicForCSM(
    const Frustum&            in_viewFrustum,
    const std::vector<float>& in_cascadeRanges,
    const Vec3&               in_lightLookDirection,
    float                     in_cameraDistFromCenter)
{
    ASSERT(std::ranges::is_sorted(in_cascadeRanges), "Cascade ranges must be sorted.");

    std::vector<Frustum> cascadeFrustums = FrustumUtil::DivideCascade(in_viewFrustum, in_cascadeRanges);
    ASSERT(cascadeFrustums.size() == MAX_CASCADE_SHADOW_LEVEL, "Cascade frustums size is not correct");

    std::vector<Mat4> shadowViewProjs;
    shadowViewProjs.resize(MAX_CASCADE_SHADOW_LEVEL);

    struct OrthoGraphicDesc
    {
        float minX, maxX, minY, maxY;
        Mat4  lightViewMat;
    };
    std::vector<OrthoGraphicDesc> orthoDescs;
    orthoDescs.resize(MAX_CASCADE_SHADOW_LEVEL);

    float cascadeMinZ = std::numeric_limits<float>::max();
    float cascadeMaxZ = std::numeric_limits<float>::min();

    for (size_t i = 0; i < MAX_CASCADE_SHADOW_LEVEL; ++i)
    {
        Mat4 lightView = MatrixUtil::CreateViewLookDirection(
            FrustumUtil::GetCenter(cascadeFrustums[i]) + -in_lightLookDirection * in_cameraDistFromCenter,
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

    for (size_t i = 0; i < MAX_CASCADE_SHADOW_LEVEL; ++i)
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

//===================================================
// Quaternion Util
//===================================================

}   // namespace crab