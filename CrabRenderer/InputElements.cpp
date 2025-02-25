#include "CrabPch.h"

#include "InputElements.h"

#include "D11Renderer.h"

namespace crab
{

//===================================================
// Input Elements
//===================================================

InputElements& InputElements::Add(const std::string_view in_sementic, uint32 in_sementicIndex, DXGI_FORMAT in_format)
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

InputElements& InputElements::Add(const std::string_view in_sementic, uint32 in_sementicIndex, eFormat in_format)
{
    elements.emplace_back(
        in_sementic.data(),
        in_sementicIndex,
        DXGI_FORMAT(in_format),
        0,
        D3D11_APPEND_ALIGNED_ELEMENT,
        D3D11_INPUT_PER_VERTEX_DATA,
        0);

    return *this;
}

}   // namespace crab