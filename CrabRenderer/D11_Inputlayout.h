#pragma once
#include "D11_Utiles.h"

namespace crab
{


//===================================================
//                 Input Elements
//===================================================

class D11_InputElements
{
public:
    void Clear() { elements.clear(); }

    D11_InputElements& Add(const std::string_view in_sementic,
                           uint32                 in_sementicIndex,
                           eVertexFormat            in_format);

    D11_InputElements& Add(const std::string_view in_sementic,
                           uint32                 in_sementicIndex,
                           DXGI_FORMAT            in_format);

    const D3D11_INPUT_ELEMENT_DESC* Get() const { return elements.data(); }
    uint32                          GetSize() const { return (uint32)elements.size(); }

private:
    std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
};

}   // namespace crab