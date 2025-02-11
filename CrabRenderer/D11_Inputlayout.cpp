#include "CrabPch.h"

#include "D11_Inputlayout.h"

#include "D11_Renderer.h"

namespace crab
{

//===================================================
// Input Elements
//===================================================

D11_InputElements& D11_InputElements::Add(const std::string_view in_sementic, uint32 in_sementicIndex, DXGI_FORMAT in_format)
{
    elements.emplace_back(
        in_sementic.data(),
        in_sementicIndex,
        in_format,
        0,
        D3D11_APPEND_ALIGNED_ELEMENT,
        D3D11_INPUT_PER_VERTEX_DATA,
        0);

    return *this;
}

D11_InputElements& D11_InputElements::Add(const std::string_view in_sementic, uint32 in_sementicIndex, eVertexFormat in_format)
{
    elements.emplace_back(
        in_sementic.data(),
        in_sementicIndex,
        ToDXGI_FORMAT(in_format),
        0,
        D3D11_APPEND_ALIGNED_ELEMENT,
        D3D11_INPUT_PER_VERTEX_DATA,
        0);

    return *this;
}

}   // namespace crab