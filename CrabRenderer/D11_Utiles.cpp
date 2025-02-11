#include "CrabPch.h"

#include "D11_Renderer.h"
#include "D11_Utiles.h"

namespace crab
{

ComPtr<ID3D11Texture2D> ID3D11Texture2DFactory::CreateID3D11Texture2D(const D3D11_TEXTURE2D_DESC& in_desc)
{
    ComPtr<ID3D11Texture2D> texture;
    D11_ASSERT(D11_API->GetDevice()->CreateTexture2D(&in_desc, nullptr, texture.GetAddressOf()), "CreateTexture2D Fail.");
    return texture;
}

ComPtr<ID3D11Texture2D> ID3D11Texture2DFactory::CreateID3D11StagingTexture2D(uint32 in_width, uint32 in_height, DXGI_FORMAT in_format, uint32 in_MSAASampleCount, uint32 in_MSAASampleQuality, uint32 in_mipLevels, uint32 in_arraySize)
{
    D3D11_TEXTURE2D_DESC stagingDesc = {};
    stagingDesc.Width                = in_width;
    stagingDesc.Height               = in_height;
    stagingDesc.MipLevels            = in_mipLevels;
    stagingDesc.ArraySize            = in_arraySize;
    stagingDesc.Format               = DXGI_FORMAT_R8G8B8A8_UNORM;
    stagingDesc.Usage                = D3D11_USAGE_STAGING;
    stagingDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_READ;
    stagingDesc.BindFlags            = D3D11_BIND_SHADER_RESOURCE;
    stagingDesc.MiscFlags            = 0;
    stagingDesc.SampleDesc.Count     = in_MSAASampleCount;
    stagingDesc.SampleDesc.Quality   = in_MSAASampleQuality;
    return CreateID3D11Texture2D(stagingDesc);
}

}   // namespace crab