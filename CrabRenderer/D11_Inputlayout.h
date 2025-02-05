#pragma once
#include "D11_Utiles.h"

namespace crab
{

struct D11_InputElements
{
    void Clear() { elements.clear(); }

    D11_InputElements& Add(const std::string_view in_sementic,
                           uint32                 in_sementicIndex,
                           eDataFormat              in_format);

    D11_InputElements& Add(const std::string_view in_sementic,
                           uint32                 in_sementicIndex,
                           DXGI_FORMAT            in_format);

    std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
};

}   // namespace crab