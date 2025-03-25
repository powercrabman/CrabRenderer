#pragma once

#include <directxtk/SimpleMath.h>
#include <directxtk/SimpleMath.inl>
#include <numbers>

namespace crab
{

using Vec2    = DirectX::SimpleMath::Vector2;
using Vec3    = DirectX::SimpleMath::Vector3;
using Vec4    = DirectX::SimpleMath::Vector4;
using Mat4    = DirectX::SimpleMath::Matrix;
using Quat    = DirectX::SimpleMath::Quaternion;
using Frustum = DirectX::BoundingFrustum;

using AABB           = DirectX::BoundingBox;
using OBB            = DirectX::BoundingOrientedBox;
using BoundingSphere = DirectX::BoundingSphere;
using Ray            = DirectX::SimpleMath::Ray;
using Plane          = DirectX::SimpleMath::Plane;

using Color4 = DirectX::SimpleMath::Color;
using Color3 = DirectX::SimpleMath::Vector3;

constexpr float PI      = std::numbers::pi_v<float>;
constexpr float PI_2    = PI * 2.0f;
constexpr float PI_DIV2 = PI / 2.0f;
constexpr float PI_DIV4 = PI / 4.0f;

constexpr float RAD2DEG = 180.0f / PI;
constexpr float DEG2RAD = PI / 180.0f;

constexpr float SMALL_NUMBER = 1.e-6f;

//===================================================
// Trigonometry
//===================================================

inline constexpr float ToRad(float in_deg)
{
    return in_deg * DEG2RAD;
}

inline constexpr float ToDeg(float in_rad)
{
    return in_rad * RAD2DEG;
}

inline void FastCosSin(float in_rad, float& out_cos, float& out_sin)
{
    DirectX::XMScalarSinCos(&out_sin, &out_cos, in_rad);
}

template<typename Ty>
inline void Clamp(Ty& in_val, const Ty& in_min, const Ty& in_max)
{
    if (in_val < in_min)
        in_val = in_min;
    else if (in_val > in_max)
        in_val = in_max;
}

template<typename Ty>
inline Ty Warp(Ty& in_val, const Ty& in_min, const Ty& in_max)
{
    Ty range = in_max - in_min;
    if (range == 0)
        return in_min;

    return in_min + std::fmod((in_val - in_min + range), range);
}

struct IVec2
{
    bool operator==(const IVec2& in_other) const
    {
        return x == in_other.x && y == in_other.y;
    }

    bool operator!=(const IVec2& in_other) const
    {
        return x != in_other.x || y != in_other.y;
    }

    IVec2 operator+(const IVec2& in_other) const
    {
        return { x + in_other.x, y + in_other.y };
    }

    IVec2 operator-(const IVec2& in_other) const
    {
        return { x - in_other.x, y - in_other.y };
    }

    IVec2 operator*(int in_scalar) const
    {
        return { x * in_scalar, y * in_scalar };
    }

    IVec2 operator*(float in_scalar) const
    {
        return { static_cast<int>(x * in_scalar), static_cast<int>(y * in_scalar) };
    }

    IVec2 operator/(float in_scalar) const
    {
        return { static_cast<int>(x / in_scalar), static_cast<int>(y / in_scalar) };
    }

    IVec2 operator/(int in_scalar) const
    {
        IVec2 output;
        output.x = static_cast<int32>(x / static_cast<float>(in_scalar));
        output.y = static_cast<int32>(y / static_cast<float>(in_scalar));
        return output;
    }
    IVec2() = default;
    IVec2(int in_x, int in_y)
        : x(in_x)
        , y(in_y)
    {
    }

    IVec2(uint32 in_x, uint32 in_y)
        : x(static_cast<int>(in_x))
        , y(static_cast<int>(in_y))
    {
    }

    Vec2 ToVec2() const
    {
        return Vec2(static_cast<float>(x), static_cast<float>(y));
    }

    int32 x = 0;
    int32 y = 0;
};

}   // namespace crab