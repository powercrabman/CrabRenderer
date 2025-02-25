#pragma once
#include "D11Utils.h"

namespace crab
{

//===================================================
//                 Input Elements
//===================================================

class InputElements
{
public:
    void Clear() { elements.clear(); }

    InputElements& Add(const std::string_view in_sementic,
                       uint32                 in_sementicIndex,
                       eFormat                in_format);

    InputElements& Add(const std::string_view in_sementic,
                       uint32                 in_sementicIndex,
                       DXGI_FORMAT            in_format);

    const D3D11_INPUT_ELEMENT_DESC* Get() const { return elements.data(); }
    uint32                          GetSize() const { return static_cast<uint32>(elements.size()); }

private:
    std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
};

}   // namespace crab