#pragma once
#include "D11_Inputlayout.h"

namespace crab
{

/*!
 * this support simple vertex types for 2D and 3D
 * if you need more complex vertex types, you should define your own vertex types
 * For this reason, there is no problem with handling your own vertices either.
 * This is just sample.
 *
 * 2D, 3D 버텍스 타입을 지원합니다.
 * 더 복잡한 버텍스 타입이 필요하다면, 직접 정의해야 합니다.
 * 이런 이유가 아니더라도 직접 정의한 버텍스를 사용할 수 있습니다.
 * 이것은 단순한 샘플입니다.
 */

//===================================================
// 2D
//===================================================

struct Vertex_Pos2D
{
    Vec2 position;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float2);
        return elem;
    }();
};

struct Vertex_Pos2D_Color
{
    Vec2 position;
    Vec4 color;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float2)
            .Add("COLOR", 0, eVertexFormat::Float4);
        return elem;
    }();
};

struct Vertex_Pos2D_Tex
{
    Vec2 position;
    Vec2 texCoord;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float2)
            .Add("TEXCOORD", 0, eVertexFormat::Float2);
        return elem;
    }();
};

struct Vertex_Pos2D_Tex_Color
{
    Vec2 position;
    Vec2 texCoord;
    Vec4 color;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float2)
            .Add("TEXCOORD", 0, eVertexFormat::Float2)
            .Add("COLOR", 0, eVertexFormat::Float4);
        return elem;
    }();
};

//===================================================
// 3D
//===================================================

struct Vertex_Pos3D
{
    Vec3 position;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float3);
        return elem;
    }();
};

struct Vertex_Pos3D_Color
{
    Vec3 position;
    Vec4 color;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float3)
            .Add("COLOR", 0, eVertexFormat::Float4);
        return elem;
    }();
};

struct Vertex_Pos3D_Tex
{
    Vec3 position;
    Vec2 texCoord;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float3)
            .Add("TEXCOORD", 0, eVertexFormat::Float2);
        return elem;
    }();
};

struct Vertex_Pos3D_Tex_Color
{
    Vec3 position;
    Vec2 texCoord;
    Vec4 color;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float3)
            .Add("TEXCOORD", 0, eVertexFormat::Float2)
            .Add("COLOR", 0, eVertexFormat::Float4);
        return elem;
    }();
};

struct Vertex_Pos3D_Normal
{
    Vec3 position;
    Vec3 normal;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float3)
            .Add("NORMAL", 0, eVertexFormat::Float3);
        return elem;
    }();
};

struct Vertex_Pos3D_Normal_Tex
{
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float3)
            .Add("NORMAL", 0, eVertexFormat::Float3)
            .Add("TEXCOORD", 0, eVertexFormat::Float2);
        return elem;
    }();
};

struct Vertex_Pos3D_Normal_Tex_Color
{
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
    Vec4 color;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, eVertexFormat::Float3)
            .Add("NORMAL", 0, eVertexFormat::Float3)
            .Add("TEXCOORD", 0, eVertexFormat::Float2)
            .Add("COLOR", 0, eVertexFormat::Float4);
        return elem;
    }();
};

}   // namespace crab