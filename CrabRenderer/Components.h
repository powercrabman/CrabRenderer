#pragma once
#include "Material.h"

namespace crab
{

// - Transform
struct Transform
{
    Mat4 GetWorld() const
    {
        return DirectX::XMMatrixAffineTransformation(scale,
                                                     Vec3::Zero,
                                                     Quat::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z),
                                                     position);
    }

    Mat4 GetWorldInvTranspose() const
    {
        return GetWorld().Invert().Transpose();
    }

    Vec3 Forward() const
    {
        Mat4 rMat = Mat4::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
        return rMat.Backward();
    }

    Vec3 Right() const
    {
        Mat4 rMat = Mat4::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
        return rMat.Right();
    }

    Vec3 Up() const
    {
        Mat4 rMat = Mat4::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
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

    Vec3 position = Vec3::Zero;
    Vec3 rotation = Vec3::Zero;   // pitch yaw roll
    Vec3 scale    = Vec3::One;
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
        Mat4 rMat = Mat4::CreateFromYawPitchRoll(in_transform.rotation.y, in_transform.rotation.x, in_transform.rotation.z);
        return DirectX::XMMatrixLookToLH(in_transform.position, rMat.Backward(), rMat.Up());
    }

    Mat4 GetProj() const
    {
        switch (projectionType)
        {
            case eProjectionType::Orthographic:
                return DirectX::XMMatrixOrthographicLH(aspect, 1, nearPlane, farPlane);
            case eProjectionType::Perspective:
                return DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearPlane, farPlane);
        }
        return Mat4::Identity;
    }

    Mat4 GetViewProj(const Transform& in_transform) const
    {
        return GetView(in_transform) * GetProj();
    }

    eProjectionType projectionType = eProjectionType::Orthographic;
    float           aspect         = 1.f;   // in perspective
    float           nearPlane      = 0.1f;
    float           farPlane       = 100.0f;
    float           fov            = DirectX::XMConvertToRadians(45.f);
};

// - Renderer

//struct D11_Mesh;
//struct D11_VertexShader;
//struct D11_PixelShader;
//struct Texture;
//struct D11_Model;
//
//struct MeshRenderer
//{
//    eTopology             topology = eTopology::TriangleList;
//    Ref<D11_Mesh>         mesh;
//    Ref<D11_VertexShader> vertexShader;
//    Ref<D11_PixelShader>  pixelShader;
//};
//
//struct ModelRenderer
//{
//    eTopology             topology = eTopology::TriangleList;
//    Ref<D11_Model>        model;
//    Ref<D11_VertexShader> vertexShader;
//    Ref<D11_PixelShader>  pixelShader;
//};
//
//struct MaterialComponent
//{
//    PhongMaterial material;
//    bool          useTexture;
//};
//
//struct NormalRenderer
//{
//    Ref<D11_Mesh>         mesh;
//    Ref<D11_VertexShader> vertexShader;
//    Ref<D11_PixelShader>  pixelShader;
//};

// - Light

enum class eLightType
{
    Directional,
    Point,
    Spot
};

// struct LightComponent
//{
//     Vec3       lightColor;
//     float      lightIntensity;
//     float      falloffStart;   // Point + Spot
//     float      falloffEnd;     // Point + Spot
//     float      spotPower;      // Spot
//     eLightType lightType;
// };

}   // namespace crab