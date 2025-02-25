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
    Quat q = Quat::CreateFromYawPitchRoll(0, in_degree, 0);
    rotate = rotate * q;
}

void TransformComponent::AddRotateYaw(float in_degree)
{
    Quat q = Quat::CreateFromYawPitchRoll(in_degree, 0, 0);
    rotate = rotate * q;
}

void TransformComponent::AddRotateRoll(float in_degree)
{
    Quat q = Quat::CreateFromYawPitchRoll(0, 0, in_degree);
    rotate = rotate * q;
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

Mat4 CameraComponent::GetView(const Vec3& in_eyePosition, const Vec3& in_pitchYawRoll) const
{
    Mat4 rMat = Mat4::CreateFromYawPitchRoll(in_pitchYawRoll.y, in_pitchYawRoll.x, in_pitchYawRoll.z);
    return DirectX::XMMatrixLookToLH(in_eyePosition, rMat.Backward(), rMat.Up());
}

Mat4 CameraComponent::GetView(const Vec3& in_eyePosition, const Quat& in_quaternion) const
{
    Mat4 rMat = Mat4::CreateFromQuaternion(in_quaternion);
    return DirectX::XMMatrixLookToLH(in_eyePosition, rMat.Backward(), rMat.Up());
}

Mat4 CameraComponent::GetProj() const
{
    switch (projectionType)
    {
        case eProjectionType::Orthographic:
            return DirectX::XMMatrixOrthographicLH(aspect, 1, nearZ, farZ);
        case eProjectionType::Perspective:
            return DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
    }
    return Mat4::Identity;
}

Mat4 CameraComponent::GetViewProj(const Vec3& in_eyePosition, const Quat& in_quaternion) const
{
    return GetView(in_eyePosition, in_quaternion) * GetProj();
}

Mat4 CameraComponent::GetViewProj(const Vec3& in_eyePosition, const Vec3& in_pitchYawRoll) const
{
    return GetView(in_eyePosition, in_pitchYawRoll) * GetProj();
}
}   // namespace crab