#pragma once
#include "InputElements.h"

namespace crab
{

/*!
 * this support simple vertex types for 2D and 3D
 * if you need more complex vertex types, you should define your own vertex types
 * For this reason, there is no problem with handling your own vertices either.
 * This is just sample.
 */

//===================================================
// 2D
//===================================================

struct Vertex2D_PosColor
{
    Vec2 position;
    Vec4 color;

    inline static InputElements s_elements = []()
    {
        InputElements elements = {};
        elements
            .Add("POSITION", 0, eFormat::Float32x2)
            .Add("COLOR", 0, eFormat::Float32x4);

        return elements;
    }();
};

struct Vertex2D_PosTex
{
    Vec2 position;
    Vec2 texCoord;

    inline static InputElements s_elements = []()
    {
        InputElements elements = {};
        elements
            .Add("POSITION", 0, eFormat::Float32x2)
            .Add("TEXCOORD", 0, eFormat::Float32x2);

        return elements;
    }();
};

struct Vertex2D_PosTexColor
{
    Vec2 position;
    Vec2 texCoord;
    Vec4 color;

    inline static InputElements s_elements = []()
    {
        InputElements elements = {};
        elements
            .Add("POSITION", 0, eFormat::Float32x2)
            .Add("TEXCOORD", 0, eFormat::Float32x2)
            .Add("COLOR", 0, eFormat::Float32x4);

        return elements;
    }();
};

//===================================================
// 3D
//===================================================

struct Vertex3D_PosColor
{
    Vec3 position;
    Vec4 color;

    inline static InputElements s_elements = []()
    {
        InputElements elements = {};
        elements
            .Add("POSITION", 0, eFormat::Float32x3)
            .Add("COLOR", 0, eFormat::Float32x4);

        return elements;
    }();
};

struct Vertex3D_PosNormalColor
{
    Vec3 position;
    Vec3 normal;
    Vec4 color;

    inline static InputElements s_elements = []()
    {
        InputElements elements = {};
        elements
            .Add("POSITION", 0, eFormat::Float32x3)
            .Add("NORMAL", 0, eFormat::Float32x3)
            .Add("COLOR", 0, eFormat::Float32x4);

        return elements;
    }();
};

struct Vertex3D_PosNormalTex
{
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;

    inline static InputElements s_elements = []()
    {
        InputElements elements = {};
        elements
            .Add("POSITION", 0, eFormat::Float32x3)
            .Add("NORMAL", 0, eFormat::Float32x3)
            .Add("TEXCOORD", 0, eFormat::Float32x2);

        return elements;
    }();
};

struct Vertex3D_PosNormalTexTangent
{
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
    Vec3 tangent;

    inline static InputElements s_elements = []()
    {
        InputElements elements = {};
        elements
            .Add("POSITION", 0, eFormat::Float32x3)
            .Add("NORMAL", 0, eFormat::Float32x3)
            .Add("TEXCOORD", 0, eFormat::Float32x2)
            .Add("TANGENT", 0, eFormat::Float32x3);

        return elements;
    }();
};

}   // namespace crab