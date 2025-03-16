#include "CrabPch.h"

#include "CrabComponents.h"

#include "D11Renderer.h"
#include "D11Utils.h"
#include "Mesh.h"
#include "Shaders.h"
#include "Textures.h"

namespace crab
{

Mat4 TransformComponent::GetWorld() const
{
    return DirectX::XMMatrixAffineTransformation(
        scale,
        Vec3::Zero,
        rotate,
        position);
}

Mat4 TransformComponent::GetWorldInvTranspose() const
{
    return GetWorld().Invert().Transpose();
}

Vec3 TransformComponent::Forward() const
{
    Mat4 rMat = Mat4::CreateFromQuaternion(rotate);
    return rMat.Backward();
}

Vec3 TransformComponent::Right() const
{
    Mat4 rMat = Mat4::CreateFromQuaternion(rotate);
    return rMat.Right();
}

Vec3 TransformComponent::Up() const
{
    Mat4 rMat = Mat4::CreateFromQuaternion(rotate);
    return rMat.Up();
}

Vec3 TransformComponent::Backward() const
{
    return -Forward();
}

Vec3 TransformComponent::Left() const
{
    return -Right();
}

Vec3 TransformComponent::Down() const
{
    return -Up();
}

void TransformComponent::AddRotatePitch(float in_degree)
{
    Quat q = Quat::CreateFromAxisAngle(Vec3::Right, in_degree);
    rotate = q * rotate;
}

void TransformComponent::AddRotateYaw(float in_degree)
{
    Quat q = Quat::CreateFromAxisAngle(Vec3::Up, in_degree);
    rotate = q * rotate;
}

void TransformComponent::AddRotateRoll(float in_degree)
{
    Quat q = Quat::CreateFromAxisAngle(Vec3::Forward, in_degree);
    rotate = q * rotate;
}

void TransformComponent::SetRotatePitch(float in_degree)
{
    Vec3 euler = rotate.ToEuler();
    euler.x    = in_degree;
    rotate     = Quat::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
}

void TransformComponent::SetRotateYaw(float in_degree)
{
    Vec3 euler = rotate.ToEuler();
    euler.y    = in_degree;
    rotate     = Quat::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
}

void TransformComponent::SetRotateRoll(float in_degree)
{
    Vec3 euler = rotate.ToEuler();
    euler.z    = in_degree;
    rotate     = Quat::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
}

Mat4 CameraComponent::GetView(const TransformComponent& in_trans)
{
    return MatrixUtil::CreateViewFromQuaternion(in_trans.position, in_trans.rotate);
}

Mat4 CameraComponent::GetProj() const
{
    switch (projectionType)
    {
        case eProjectionType::Orthographic:
            return MatrixUtil::CreateOrthographic(width, height, nearZ, farZ);
        case eProjectionType::Perspective:
            return MatrixUtil::CreatePerspective(fov, aspect, nearZ, farZ);
    }
    CRAB_DEBUG_BREAK("Invalid Projection Type.");
    return Mat4::Identity;
}

Mat4 CameraComponent::GetViewProj(const Vec3& in_eyePosition, const Quat& in_quaternion) const
{
    return MatrixUtil::CreateViewFromQuaternion(in_eyePosition, in_quaternion) * GetProj();
}

Mat4 CameraComponent::GetViewProj(const Vec3& in_eyePosition, const Vec3& in_pitchYawRoll) const
{
    return MatrixUtil::CreateViewFromEuler(in_eyePosition, in_pitchYawRoll) * GetProj();
}

Mat4 CameraComponent::GetViewProj(const TransformComponent& in_trans) const
{
    return GetViewProj(in_trans.position, in_trans.rotate);
}

Frustum CameraComponent::GetViewFrustumInView() const
{
    return FrustumUtil::CreateFromMatrixInLocal(GetProj());
}

Frustum CameraComponent::GetViewFrustumInWorld(const Vec3& in_position, const Quat& in_quaternion) const
{
    return FrustumUtil::CreateFromMatrixInWorld(in_position, in_quaternion, GetProj());
}

Frustum CameraComponent::GetViewFrustumInWorld(const Vec3& in_position, const Vec3& in_pitchYawRoll) const
{
    return FrustumUtil::CreateFromMatrixInWorld(in_position, in_pitchYawRoll, GetProj());
}

Frustum CameraComponent::GetViewFrustumInWorld(const TransformComponent& in_trans) const
{
    return FrustumUtil::CreateFromMatrixInWorld(in_trans.position, in_trans.rotate, GetProj());
}

}   // namespace crab