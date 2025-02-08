#include "CrabPch.h"

#include "D11_Renderer.h"
#include "D11_Utiles.h"

namespace crab
{

ComPtr<ID3D11Texture2D> CreateID3D11Texture2D(const D3D11_TEXTURE2D_DESC& in_desc)
{
    ComPtr<ID3D11Texture2D> texture;
    D11_ASSERT(D11_API->GetDevice()->CreateTexture2D(&in_desc, nullptr, texture.GetAddressOf()), "CreateTexture2D Fail.");
    return texture;
}

}   // namespace crab