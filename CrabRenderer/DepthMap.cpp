#include "CrabPch.h"

#include "DepthMap.h"

#include "DepthBuffer.h"
#include "Textures.h"

namespace crab
{

void DepthMap::Clear(float in_clearValue) const
{
    m_depthBuffer->Clear(true, in_clearValue, false, 0);
}

void DepthMap::BindDepthBuffer() const
{
    GetRenderer().BindOnlyDepthStencilView(m_depthBuffer->Get());
}

void DepthMap::BindDepthMapTexture(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    m_texture->Bind(in_slot, in_bindFlags);
}

Int2 DepthMap::GetResolution() const
{
    return m_depthBuffer->GetResolution();
}

void DepthMapTexture::Init(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthFormat,
    eFormat in_textureFormat)
{
    auto depthTex = ID3D11Texture2DUtil::CreateTexture2D(
        in_width,
        in_height,
        eFormat::Float32_Typeless,
        D3D11_USAGE_DEFAULT,
        eBindFlags_DepthStencil | eBindFlags_ShaderResource,
        eCPUAccessFlags_None,
        1,
        0,
        1,
        1);

    auto tex = CreateRef<Texture2D>();
    tex->Init(depthTex.Get(), in_textureFormat);
    m_texture = tex;

    m_depthBuffer = CreateRef<DepthBuffer>();
    m_depthBuffer->Init(depthTex.Get(), in_depthFormat);
}

void DepthMapArray::Init(
    uint32  in_width,
    uint32  in_height,
    uint32  in_arraySize,
    eFormat in_depthFormat,
    eFormat in_textureFormat)
{
    auto depthTex = ID3D11Texture2DUtil::CreateTexture2D(
        in_width,
        in_height,
        eFormat::Float32_Typeless,
        D3D11_USAGE_DEFAULT,
        eBindFlags_DepthStencil | eBindFlags_ShaderResource,
        eCPUAccessFlags_None,
        1,
        0,
        1,
        in_arraySize);

    auto texArray = CreateRef<Texture2DArray>();
    texArray->Init(depthTex.Get(), in_textureFormat);
    m_texture = texArray;

    m_depthBuffer = CreateRef<DepthBuffer>();
    m_depthBuffer->Init(depthTex.Get(), in_depthFormat);
}

void DepthMapCube::Init(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthFormat,
    eFormat in_textureFormat)
{
    auto depthTex = ID3D11Texture2DUtil::CreateTexture2D(
        in_width,
        in_height,
        eFormat::Float32_Typeless,
        D3D11_USAGE_DEFAULT,
        eBindFlags_DepthStencil | eBindFlags_ShaderResource,
        eCPUAccessFlags_None,
        1,
        0,
        1,
        6,
        true);

    auto texCube = CreateRef<TextureCube>();
    texCube->Init(depthTex.Get(), in_textureFormat);
    m_texture = texCube;

    m_depthBuffer = CreateRef<DepthBuffer>();
    m_depthBuffer->Init(depthTex.Get(), in_depthFormat);
}

}   // namespace crab