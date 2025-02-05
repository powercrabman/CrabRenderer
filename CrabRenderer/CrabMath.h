#pragma once

namespace crab
{

constexpr float PI      = 3.14159265358979323846f;
constexpr float PI_2    = PI * 2.0f;
constexpr float PI_DIV2 = PI / 2.0f;
constexpr float PI_DIV4 = PI / 4.0f;

constexpr float RAD2DEG = 180.0f / PI;
constexpr float DEG2RAD = PI / 180.0f;

// Convert degree ~ radian
inline constexpr float ToRad(float in_deg)
{
    return in_deg * DEG2RAD;
}

inline constexpr float ToDeg(float in_rad)
{
    return in_rad * RAD2DEG;
}

// Random number generator
// [in_min, in_max)
int32 RandomInt(int32 in_min, int32 in_max);
float RandomFloat(float in_min, float in_max);

// sin cos
inline void FastCosSin(float in_rad, float& out_cos, float& out_sin)
{
    DirectX::XMScalarSinCos(&out_sin, &out_cos, in_rad);
}

// Vector
inline constexpr float FastSqrt(float number)
{
    // Fast inverse square root
    // https://en.wikipedia.org/wiki/Fast_inverse_square_root

    union
    {
        float    f;
        uint32_t i;
    } conv {};

    float           x2;
    constexpr float threehalfs = 1.5F;

    x2     = number * 0.5F;
    conv.f = number;
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
    return conv.f;
}

inline Vec2 NormalizedVec2(const Vec2& in_vec)
{
    return in_vec * FastSqrt(in_vec.LengthSquared());
}

inline Vec3 NormalizedVec3(const Vec3& in_vec)
{
    return in_vec * FastSqrt(in_vec.LengthSquared());
}

inline Vec4 NormalizedVec4(const Vec4& in_vec)
{
    return in_vec * FastSqrt(in_vec.LengthSquared());
}

inline Vec3 EulerFromLookAt(const Vec3& in_from, const Vec3& in_to)
{
    Vec3 lookVec = (in_to - in_from);
    lookVec.Normalize();

    Vec3 euler = Vec3::Zero;
    euler.y = atan2(lookVec.x, lookVec.z);
    euler.x = -asin(lookVec.y);

    return euler;
}

template <typename Ty>
inline void Clamp(Ty& in_val, const Ty& in_min, const Ty& in_max)
{
    if (in_val < in_min)
        in_val = in_min;
    else if (in_val > in_max)
        in_val = in_max;
}

template <typename Ty>
inline void Warp(Ty& in_val, const Ty& in_min, const Ty& in_max)
{
    Ty range = in_max - in_min;
    if (range == 0)
        return in_min;

    return in_min + std::fmod((in_val - in_min + range), range);
}

}   // namespace crab