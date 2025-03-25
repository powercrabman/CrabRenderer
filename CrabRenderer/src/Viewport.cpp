#include "InternalPCH.h"

#include "Viewport.h"

#include "Renderer.h"
#include "../../CrabEngine/inc/Application.h"
#include "../../CrabEngine/inc/AppWindow.h"

namespace crab
{
Viewport::Viewport()
    : x(0.0f)
    , y(0.0f)
    , width(0.0f)
    , height(0.0f)
    , minDepth(0.0f)
    , maxDepth(1.0f)
{
}

Viewport::Viewport(float in_width, float in_height)
    : x(0.0f)
    , y(0.0f)
    , width(in_width)
    , height(in_height)
    , minDepth(0.0f)
    , maxDepth(1.0f)
{
}

Viewport::Viewport(uint32 in_width, uint32 in_height)
    : x(0.0f)
    , y(0.0f)
    , width(static_cast<float>(in_width))
    , height(static_cast<float>(in_height))
    , minDepth(0.0f)
    , maxDepth(1.0f)
{
}

Viewport::Viewport(int32 in_width, int32 in_height)
    : x(0.0f)
    , y(0.0f)
    , width(static_cast<float>(in_width))
    , height(static_cast<float>(in_height))
    , minDepth(0.0f)
    , maxDepth(1.0f)
{
}

Viewport Viewport::CreateFullScreen()
{
    auto [width, height] = GetAppWindow().GetResolution();
    return Viewport(width, height);
}

Viewport::Viewport(float in_x, float in_y, float in_width, float in_height, float in_minDepth, float in_maxDepth)
    : x(in_x)
    , y(in_y)
    , width(in_width)
    , height(in_height)
    , minDepth(in_minDepth)
    , maxDepth(in_maxDepth)
{
}

void Viewport::Bind() const
{
    GetRenderer().SetViewport(*this);
}

const D3D11_VIEWPORT* Viewport::GetD3D11Viewport() const
{
    return reinterpret_cast<const D3D11_VIEWPORT*>(this);
}
}   // namespace crab