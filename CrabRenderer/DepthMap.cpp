#include "CrabPch.h"

#include "DepthMap.h"

#include "DepthBuffer.h"
#include "Textures.h"

namespace crab
{

Ref<DepthMap> DepthMap::Create(
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

    Ref<DepthMap> depthMap  = CreateRef<DepthMap>();
    depthMap->m_texture     = Texture2D::CreateFromTexture(depthTex.Get(), in_textureFormat);
    depthMap->m_depthBuffer = DepthBuffer::Create(depthTex.Get(), in_depthFormat);

    return depthMap;
}

Ref<DepthMap> DepthMap::CreateDepthMapArray(
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

    Ref<DepthMap> depthMap  = CreateRef<DepthMap>();
    depthMap->m_texture     = Texture2DArray::CreateTextureArrayFromTexture(depthTex.Get(), in_textureFormat);
    depthMap->m_depthBuffer = DepthBuffer::Create(depthTex.Get(), in_depthFormat);

    return depthMap;
}

Ref<DepthMap> DepthMap::CreateDepthMapCube(
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

    Ref<DepthMap> depthMap  = CreateRef<DepthMap>();
    depthMap->m_texture     = TextureCube::CreateFromTexture(depthTex.Get(), in_textureFormat);
    depthMap->m_depthBuffer = DepthBuffer::Create(depthTex.Get(), in_depthFormat);

    return depthMap;
}

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

}   // namespace crab