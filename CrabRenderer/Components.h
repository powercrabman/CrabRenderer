#pragma once
#include "Material.h"

namespace crab
{
// - ID
struct IDComponent
{
    uint32 id;
};

// - Name
struct TagComponent
{
    std::string name;
};

// - Transform
struct Transform
{
    Mat4 GetWorld() const
    {
        return DirectX::XMMatrixAffineTransformation(scale,
                                                     Vec3::Zero,
                                                     quaternion,
                                                     position);
    }

    Mat4 GetWorldInvTranspose() const
    {
        return GetWorld().Invert().Transpose();
    }

    Vec3 Forward() const
    {
        Mat4 rMat = Mat4::CreateFromQuaternion(quaternion);
        return rMat.Backward();
    }

    Vec3 Right() const
    {
        Mat4 rMat = Mat4::CreateFromQuaternion(quaternion);
        return rMat.Right();
    }

    Vec3 Up() const
    {
        Mat4 rMat = Mat4::CreateFromQuaternion(quaternion);
        return rMat.Up();
    }

    Vec3 Backward() const
    {
        return -Forward();
    }

    Vec3 Left() const
    {
        return -Right();
    }

    Vec3 Down() const
    {
        return -Up();
    }

    Vec3 position   = Vec3::Zero;
    Vec3 scale      = Vec3::One;
    Quat quaternion = Quat::Identity;
};

// - Camera
enum class eProjectionType
{
    Orthographic,
    Perspective
};

struct CameraComponent
{
    Mat4 GetView(const Transform& in_transform) const
    {
        Mat4 rMat = Mat4::CreateFromQuaternion(in_transform.quaternion);
        return DirectX::XMMatrixLookToLH(in_transform.position, rMat.Backward(), rMat.Up());
    }

    Mat4 GetProj() const
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

    Mat4 GetViewProj(const Transform& in_transform) const
    {
        return GetView(in_transform) * GetProj();
    }

    eProjectionType projectionType = eProjectionType::Orthographic;
    float           aspect         = 1.f;   // in perspective
    float           nearZ          = 0.1f;
    float           farZ           = 100.0f;
    float           fov            = DirectX::XMConvertToRadians(45.f);
};

class Script;

struct ScriptComponent
{
    Scope<Script> script;
};

}   // namespace crab