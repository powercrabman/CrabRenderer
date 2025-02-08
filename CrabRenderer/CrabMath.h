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


inline Vec2 Normalize(const Vec2& in_vec)
{
    Vec2 result = in_vec;
    result.Normalize();
    return result;
}

inline Vec3 Normalize(const Vec3& in_vec)
{
    Vec3 result = in_vec;
    result.Normalize();
    return result;
}

inline Vec4 Normalize(const Vec4& in_vec)
{
    Vec4 result = in_vec;
    result.Normalize();
    return result;
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

// Rect
struct Rect
{
    Rect() = default;
    Rect(uint32 in_left, uint32 in_top, uint32 in_right, uint32 in_bottom)
        : left(in_left)
        , top(in_top)
        , right(in_right)
        , bottom(in_bottom)
    {
    }

    uint32 Width() const { return right - left; }
    uint32 Height() const { return bottom - top; }
    void   Clamp(uint32 in_left, uint32 in_top, uint32 in_right, uint32 in_bottom) 
    { 
        crab::Clamp(left, in_left, in_right);
        crab::Clamp(top, in_top, in_bottom);
        crab::Clamp(right, in_left, in_right);
        crab::Clamp(bottom, in_top, in_bottom);
        left = std::min(left, right);
        top  = std::min(top, bottom);
    }

    uint32 left;
    uint32 top;
    uint32 right;
    uint32 bottom;
};

struct Vec4UInt
{
    uint32 x, y, z, w;
};

// Vector Cast Helper
inline Vec4 ToVec4(const Vec3& in_vec, float in_w)
{
    return Vec4(in_vec.x, in_vec.y, in_vec.z, in_w);
}

inline Vec4 ToVec4(const Vec2& in_vec, float in_z, float in_w)
{
    return Vec4(in_vec.x, in_vec.y, in_z, in_w);
}

inline Vec3 ToVec3(const Vec4& in_vec)
{
    return Vec3(in_vec.x, in_vec.y, in_vec.z);
}

inline Vec3 ToVec3(const Vec2& in_vec, float in_z)
{
    return Vec3(in_vec.x, in_vec.y, in_z);
}

inline Vec2 ToVec2(const Vec4& in_vec)
{
    return Vec2(in_vec.x, in_vec.y);
}

inline Vec2 ToVec2(const Vec3& in_vec)
{
    return Vec2(in_vec.x, in_vec.y);
}

}   // namespace crab