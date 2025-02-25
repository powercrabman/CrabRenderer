#pragma once

#include "D11ResourceList.h"

namespace crab
{
// ID
struct IDComponent
{
    uint32 id;
};

// Name
struct TagComponent
{
    std::string name;
};

// TransformComponent
struct TransformComponent
{
    Mat4 GetWorld() const;
    Mat4 GetWorldInvTranspose() const;

    Vec3 Forward() const;
    Vec3 Backward() const;
    Vec3 Right() const;
    Vec3 Left() const;
    Vec3 Up() const;
    Vec3 Down() const;

    void AddRotatePitch(float in_degree);
    void AddRotateYaw(float in_degree);
    void AddRotateRoll(float in_degree);

    void SetRotatePitch(float in_degree);
    void SetRotateYaw(float in_degree);
    void SetRotateRoll(float in_degree);

    Vec3 position = Vec3::Zero;
    Quat rotate   = Quat::Identity;
    Vec3 scale    = Vec3::One;
};

// Camera
enum class eProjectionType
{
    Orthographic,
    Perspective
};

struct CameraComponent
{
    Mat4 GetView(const Vec3& in_eyePosition, const Vec3& in_pitchYawRoll) const;
    Mat4 GetView(const Vec3& in_eyePosition, const Quat& in_quaternion) const;
    Mat4 GetProj() const;

    Mat4 GetViewProj(const Vec3& in_eyePosition, const Quat& in_quaternion) const;
    Mat4 GetViewProj(const Vec3& in_eyePosition, const Vec3& in_pitchYawRoll) const;

    eProjectionType projectionType = eProjectionType::Perspective;
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