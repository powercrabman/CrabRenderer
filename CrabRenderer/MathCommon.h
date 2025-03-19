#pragma once

//================================
// Math
// ------------------------------
// trigonometry, wrap, clamp, etc.
// Very simple math functions
//================================

namespace crab
{

constexpr float PI      = 3.14159265358979323846f;
constexpr float PI_2    = PI * 2.0f;
constexpr float PI_DIV2 = PI / 2.0f;
constexpr float PI_DIV4 = PI / 4.0f;

constexpr float RAD2DEG = 180.0f / PI;
constexpr float DEG2RAD = PI / 180.0f;

constexpr float SMALL_NUMBER = 1e-5f;

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

//===================================================
// Structure
//===================================================

struct Int2
{
    Int2() = default;
    Int2(int32 in_x, int32 in_y)
        : x(in_x)
        , y(in_y)
    {
    }

    bool operator==(const Int2& in_rhs) const
    {
        return x == in_rhs.x && y == in_rhs.y;
    }

    bool operator!=(const Int2& in_rhs) const
    {
        return !(*this == in_rhs);
    }

    Int2 operator+(const Int2& in_rhs) const
    {
        return { x + in_rhs.x, y + in_rhs.y };
    }

    Int2 operator-(const Int2& in_rhs) const
    {
        return { x - in_rhs.x, y - in_rhs.y };
    }

    Int2 operator*(float in_rhs) const
    {
        return { static_cast<int32>(x * in_rhs),
                 static_cast<int32>(y * in_rhs) };
    }

    Int2 operator*(const Int2& in_rhs) const
    {
        return { x * in_rhs.x, y * in_rhs.y };
    }

    Int2 operator/(float in_rhs) const
    {
        return { static_cast<int32>(x / in_rhs),
                 static_cast<int32>(y / in_rhs) };
    }

    Int2 operator/(const Int2& in_rhs) const
    {
        return { static_cast<int32>(x / static_cast<float>(in_rhs.x)),
                 static_cast<int32>(y / static_cast<float>(in_rhs.y)) };
    }

    Vec2 ToVec2() const
    {
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    int32 x;
    int32 y;
};

struct Int3
{
    Int3() = default;
    Int3(int32 in_x, int32 in_y, int32 in_z)
        : x(in_x)
        , y(in_y)
        , z(in_z)
    {
    }

    int32 x;
    int32 y;
    int32 z;
};

struct Int4
{
    Int4() = default;
    Int4(int32 in_x, int32 in_y, int32 in_z, int32 in_w)
        : x(in_x)
        , y(in_y)
        , z(in_z)
        , w(in_w)
    {
    }

    int32 x;
    int32 y;
    int32 z;
    int32 w;
};

struct Rect
{
    Rect() = default;
    Rect(int32 in_x, int32 in_y, int32 in_width, int32 in_height)
        : position(in_x, in_y)
        , size(in_width, in_height)
    {
    }

    Rect(const Int2& in_position, const Int2& in_size)
        : position(in_position)
        , size(in_size)
    {
    }

    Int2 LeftTop() const
    {
        return position;
    }

    Int2 RightTop() const
    {
        return { position.x + size.x, position.y };
    }

    Int2 position;
    Int2 size;
};

}   // namespace crab