#include "CrabPch.h"

#include "Draw2D.h"

#include "Mesh.h"

namespace crab
{

namespace detail
{
    struct PrimitiveVertex
    {
        Vec2 position;
        Vec4 color;
    };

    struct SpriteVertex
    {
        Vec2 position;
        Vec2 uv;
    };

    cbuffer Draw2DConstant
    {
        float alpha;
    };

    cbuffer Draw2DCameraConstant
    {
        Mat4 viewProj;
    };

    cbuffer Draw2DTransformConstant
    {
        Mat4 world;
    };

}   // namespace

struct Draw2D::Impl
{
    // maybe
    // triangle path
    // point path
    // line path will need
};

Scope<Draw2D::Impl> Draw2D::m_impl = CreateScope<Impl>();

void Draw2D::Init()
{
}

}   // namespace crab