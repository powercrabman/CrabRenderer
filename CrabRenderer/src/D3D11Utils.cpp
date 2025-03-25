#include "InternalPCH.h"

#include "Renderer.h"
#include "Shaders.h"

#include <DirectXTex.h>
#include <d3dcompiler.h>

namespace crab
{
namespace d3d11
{
    namespace texture
    {

        ComPtr<ID3D11Texture2D> CreateTexture2D(
            uint32                in_width,
            uint32                in_height,
            eFormat               in_format,
            D3D11_USAGE           in_usage,
            eBindFlags            in_bindFlags,
            eCPUAccessFlags       in_cpuAccessFlags,
            MSAA                  in_MSAA,
            uint32                in_numberOfMipmap,
            uint32                in_textureArraySize,
            eTextureCreationFlags in_creationFlags,
            const void*           in_initData)
        {
            ComPtr<ID3D11Texture2D> texture;

            D3D11_TEXTURE2D_DESC desc;
            desc.Width              = in_width;
            desc.Height             = in_height;
            desc.MipLevels          = in_numberOfMipmap;
            desc.ArraySize          = in_textureArraySize;
            desc.Format             = static_cast<DXGI_FORMAT>(in_format);
            desc.SampleDesc.Count   = in_MSAA.sampleCount;
            desc.SampleDesc.Quality = in_MSAA.quality;
            desc.Usage              = in_usage;
            desc.BindFlags          = in_bindFlags;
            desc.CPUAccessFlags     = in_cpuAccessFlags;
            desc.MiscFlags          = in_creationFlags;

            D3D11_SUBRESOURCE_DATA initData = {};

            if (in_initData)
            {
                initData.pSysMem          = in_initData;
                initData.SysMemPitch      = in_width * GetFormatByteStride(in_format);
                initData.SysMemSlicePitch = in_height * initData.SysMemPitch;
            }

            d3d11::CheckOK(GetRenderer().GetDevice()->CreateTexture2D(&desc,
                                                                      initData.pSysMem ? &initData : nullptr,
                                                                      texture.GetAddressOf()),
                           "CreateSRV Fail.");

            return texture;
        }

        ComPtr<ID3D11Texture2D> CreateTexture2DArray(
            const std::vector<ID3D11Texture2D*>& in_textures,
            eTextureCreationFlags                in_creationFlags)
        {
            if (in_textures.empty())
            {
                DEBUG_BREAK("Texture Array is empty.");
                return nullptr;
            }

            D3D11_TEXTURE2D_DESC standardDesc;
            in_textures[0]->GetDesc(&standardDesc);

            // 유효성 검사
            for (uint32 i = 1; i < in_textures.size(); ++i)
            {
                D3D11_TEXTURE2D_DESC desc;
                in_textures[i]->GetDesc(&desc);

                if (!IsSameFormat(standardDesc, desc))
                {
                    DEBUG_BREAK("Texture2DArray Init Fail. Mismatch in texture dimensions or format.");
                    return nullptr;
                }
            }

            // 텍스처 배열 생성
            ComPtr<ID3D11Texture2D> textureArray = CreateTexture2D(
                standardDesc.Width,
                standardDesc.Height,
                static_cast<eFormat>(standardDesc.Format),
                standardDesc.Usage,
                standardDesc.BindFlags,
                standardDesc.CPUAccessFlags,
                MSAA { standardDesc.SampleDesc.Count, standardDesc.SampleDesc.Quality },
                standardDesc.MipLevels,
                static_cast<uint32>(in_textures.size()),
                in_creationFlags,
                nullptr);

            // 데이터 복사
            for (UINT i = 0; i < in_textures.size(); ++i)
            {
                for (UINT mipLevel = 0; mipLevel < standardDesc.MipLevels; ++mipLevel)
                    CopyBetween(in_textures[i],
                                mipLevel,
                                0,
                                textureArray.Get(),
                                mipLevel,
                                i);
            }

            return textureArray;
        }

        void WriteToDefaultTexture(
            ID3D11Texture2D* in_defaultTexture,
            const void*      in_writeData,
            uint32           in_writeDataByteWidth)
        {
            if (!in_defaultTexture)
            {
                DEBUG_BREAK("Texture is nullptr.");
                return;
            }

            D3D11_TEXTURE2D_DESC desc;
            in_defaultTexture->GetDesc(&desc);

            uint32 rowPitch   = GetFormatByteStride(desc.Format) * desc.Width;
            uint32 slicePitch = rowPitch * desc.Height;

            if (in_writeData && in_writeDataByteWidth == slicePitch)
            {
                GetRenderer().GetDeviceContext()->UpdateSubresource(
                    in_defaultTexture,
                    0,
                    nullptr,
                    in_writeData,
                    rowPitch,
                    slicePitch);
            }
            else
            {
                DEBUG_BREAK("SrcData is nullptr or SrcData size is not equal to texture size.");
            }
        }

        void WriteToDynamicTexture(
            ID3D11Texture2D* in_dynamicTexture,
            const void*      in_writeData,
            uint32           in_writeDataByteWidth)
        {
            if (!in_dynamicTexture)
            {
                DEBUG_BREAK("Texture is nullptr.");
                return;
            }

            D3D11_TEXTURE2D_DESC desc;
            in_dynamicTexture->GetDesc(&desc);

            if (in_writeData && in_writeDataByteWidth <= desc.Width * desc.Height * GetFormatByteStride(desc.Format))
            {
                D3D11_MAPPED_SUBRESOURCE mappedResource = {};
                if (d3d11::CheckOK(
                        GetRenderer().GetDeviceContext()->Map(
                            in_dynamicTexture,
                            0,
                            D3D11_MAP_WRITE_DISCARD,
                            0,
                            &mappedResource),
                        "Map Failed."))
                {

                    eBlendWriteFlags*       dst      = static_cast<eBlendWriteFlags*>(mappedResource.pData);
                    const eBlendWriteFlags* src      = static_cast<const eBlendWriteFlags*>(in_writeData);
                    uint32                  rowPitch = desc.Width * GetFormatByteStride(desc.Format);

                    for (uint32 y = 0; y < desc.Height; ++y)
                        ::memcpy(dst + y * mappedResource.RowPitch, src + y * rowPitch, rowPitch);

                    GetRenderer().GetDeviceContext()->Unmap(in_dynamicTexture, 0);
                }
            }
            else
            {
                DEBUG_BREAK("SrcData is nullptr or SrcData size is not equal to texture size.");
            }
        }

        void WriteToStagingTexture(
            ID3D11Texture2D* in_stagingTexture,
            const void*      in_writeData,
            uint32           in_writeDataByteWidth)
        {
            if (!in_stagingTexture)
            {
                DEBUG_BREAK("Staging texture is nullptr.");
                return;
            }

            D3D11_TEXTURE2D_DESC desc;
            in_stagingTexture->GetDesc(&desc);

            if (in_writeData && in_writeDataByteWidth <= desc.Width * desc.Height * GetFormatByteStride(desc.Format))
            {
                D3D11_MAPPED_SUBRESOURCE mappedResource = {};
                if (d3d11::CheckOK(
                        GetRenderer().GetDeviceContext()->Map(
                            in_stagingTexture,
                            0,
                            D3D11_MAP_WRITE,
                            0,
                            &mappedResource),
                        "Map Failed."))
                {

                    eBlendWriteFlags*       dst      = static_cast<eBlendWriteFlags*>(mappedResource.pData);
                    const eBlendWriteFlags* src      = static_cast<const eBlendWriteFlags*>(in_writeData);
                    uint32                  rowPitch = desc.Width * GetFormatByteStride(desc.Format);

                    for (uint32 y = 0; y < desc.Height; ++y)
                        ::memcpy(dst + y * mappedResource.RowPitch, src + y * rowPitch, rowPitch);

                    GetRenderer().GetDeviceContext()->Unmap(in_stagingTexture, 0);
                }
            }
            else
            {
                DEBUG_BREAK("SrcData is nullptr or SrcData size is not equal to texture size.");
            }
        }

        void ReadFromStagingTexture(
            ID3D11Texture2D* in_stagingTexture,
            void*            out_readData,
            uint32*          out_readDataByteWidth)
        {
            if (!in_stagingTexture)
            {
                DEBUG_BREAK("Staging texture is nullptr.");
                return;
            }

            if (!out_readData)
            {
                DEBUG_BREAK("out_readData is nullptr.");
                return;
            }

            D3D11_TEXTURE2D_DESC desc;
            in_stagingTexture->GetDesc(&desc);

            D3D11_MAPPED_SUBRESOURCE mappedResource = {};
            if (d3d11::CheckOK(
                    GetRenderer().GetDeviceContext()->Map(
                        in_stagingTexture,
                        0,
                        D3D11_MAP_READ,
                        0,
                        &mappedResource),
                    "Map Failed."))
            {

                eBlendWriteFlags* src = static_cast<eBlendWriteFlags*>(mappedResource.pData);
                ::memcpy(out_readData, src, desc.Width * desc.Height * GetFormatByteStride(desc.Format));

                if (out_readDataByteWidth)
                    *out_readDataByteWidth = desc.Width * desc.Height * GetFormatByteStride(desc.Format);

                GetRenderer().GetDeviceContext()->Unmap(in_stagingTexture, 0);
            }
        }

        void CopyBetween(
            ID3D11Texture2D* in_srcTexture,
            ID3D11Texture2D* in_dstTexture)
        {
            CopyBetween(in_srcTexture, 0, 0, in_dstTexture, 0, 0);
        }

        void CopyBetween(
            ID3D11Texture2D* in_srcTexture,
            uint32           in_srcMipLevel,
            uint32           in_srcArrayIndex,
            ID3D11Texture2D* in_dstTexture,
            uint32           in_dstMipLevel,
            uint32           in_dstArrayIndex)
        {
            D3D11_TEXTURE2D_DESC srcDesc;
            in_srcTexture->GetDesc(&srcDesc);

            D3D11_TEXTURE2D_DESC dstDesc;
            in_dstTexture->GetDesc(&dstDesc);

            if (IsSameFormat(srcDesc, dstDesc))
            {
                uint32 srcSubresource = D3D11CalcSubresource(in_srcMipLevel, in_srcArrayIndex, srcDesc.MipLevels);
                uint32 dstSubresource = D3D11CalcSubresource(in_dstMipLevel, in_dstArrayIndex, dstDesc.MipLevels);

                GetRenderer().GetDeviceContext()->CopySubresourceRegion(
                    in_dstTexture, srcSubresource, 0, 0, 0, in_srcTexture, dstSubresource, nullptr);
            }
            else
            {
                DEBUG_BREAK("Mismatch in texture dimensions or format.");
            }
        }

        void ResolveTexture2D(
            ID3D11Texture2D* in_MSTexture,
            ID3D11Texture2D* in_dstTexture)
        {
            if (in_MSTexture && in_dstTexture)
            {
                D3D11_TEXTURE2D_DESC desc;
                in_dstTexture->GetDesc(&desc);

                GetRenderer().GetDeviceContext()->ResolveSubresource(in_dstTexture,
                                                                     0,
                                                                     in_MSTexture,
                                                                     0,
                                                                     desc.Format);
            }
            else
            {
                DEBUG_BREAK("One or both textures are nullptr.");
            }
        }

        bool IsSameFormat(ID3D11Texture2D* in_texture1,
                          ID3D11Texture2D* in_texture2)
        {
            if (!in_texture1 || !in_texture2)
            {
                DEBUG_BREAK("One or both textures are nullptr.");
                return false;
            }

            D3D11_TEXTURE2D_DESC desc1;
            in_texture1->GetDesc(&desc1);

            D3D11_TEXTURE2D_DESC desc2;
            in_texture2->GetDesc(&desc2);

            return IsSameFormat(desc1, desc2);
        }

        bool IsSameFormat(
            const D3D11_TEXTURE2D_DESC& in_desc1,
            const D3D11_TEXTURE2D_DESC& in_desc2)
        {
            return in_desc1.Format == in_desc2.Format && in_desc1.Width == in_desc2.Width && in_desc1.Height == in_desc2.Height;
        }
    }   // namespace texture

    namespace buffer
    {

        ComPtr<ID3D11Buffer> CreateBuffer(
            uint32          in_bufferByteWidth,
            D3D11_USAGE     in_usage,
            eBindFlags      in_bindFlags,
            eCPUAccessFlags in_cpuAccessFlags,
            const void*     in_initData)
        {
            ComPtr<ID3D11Buffer> buffer;

            D3D11_BUFFER_DESC desc;
            desc.Usage          = in_usage;
            desc.ByteWidth      = in_bufferByteWidth;
            desc.BindFlags      = in_bindFlags;
            desc.CPUAccessFlags = in_cpuAccessFlags;
            desc.MiscFlags      = 0;

            D3D11_SUBRESOURCE_DATA initData = {};

            if (in_initData)
                initData.pSysMem = in_initData;

            d3d11::CheckOK(
                GetRenderer().GetDevice()->CreateBuffer(
                    &desc,
                    &initData,
                    buffer.GetAddressOf()),
                "CreateBuffer Fail.");

            return buffer;
        }

        void WriteToDynamicBuffer(
            ID3D11Buffer* in_buffer,
            const void*   in_writeData,
            uint32        in_writeDataByteWidth)
        {
            if (!in_buffer)
            {
                DEBUG_BREAK("Buffer is nullptr.");
                return;
            }

            D3D11_BUFFER_DESC desc;
            in_buffer->GetDesc(&desc);

            if (in_writeData && in_writeDataByteWidth <= desc.ByteWidth)
            {
                D3D11_MAPPED_SUBRESOURCE mappedResource = {};
                if (d3d11::CheckOK(
                        GetRenderer().GetDeviceContext()->Map(
                            in_buffer,
                            0,
                            D3D11_MAP_WRITE_DISCARD,
                            0,
                            &mappedResource),
                        "Map Failed."))
                {

                    eBlendWriteFlags* dst = static_cast<eBlendWriteFlags*>(mappedResource.pData);
                    ::memcpy(dst, in_writeData, in_writeDataByteWidth);

                    GetRenderer().GetDeviceContext()->Unmap(in_buffer, 0);
                }
            }
            else
            {
                DEBUG_BREAK("SrcData is nullptr or SrcData size is not equal to buffer size.");
            }
        }

        void WriteToStagingBuffer(
            ID3D11Buffer* in_buffer,
            const void*   in_writeData,
            uint32        in_writeDataByteWidth)
        {
            if (!in_buffer)
            {
                DEBUG_BREAK("Buffer is nullptr.");
                return;
            }

            D3D11_BUFFER_DESC desc;
            in_buffer->GetDesc(&desc);

            if (in_writeData && in_writeDataByteWidth <= desc.ByteWidth)
            {
                D3D11_MAPPED_SUBRESOURCE mappedResource = {};
                if (d3d11::CheckOK(
                        GetRenderer().GetDeviceContext()->Map(
                            in_buffer,
                            0,
                            D3D11_MAP_WRITE,
                            0,
                            &mappedResource),
                        "Map Failed."))
                {

                    eBlendWriteFlags* dst = static_cast<eBlendWriteFlags*>(mappedResource.pData);
                    ::memcpy(dst, in_writeData, in_writeDataByteWidth);

                    GetRenderer().GetDeviceContext()->Unmap(in_buffer, 0);
                }
            }
            else
            {
                DEBUG_BREAK("SrcData is nullptr or SrcData size is not equal to buffer size.");
            }
        }

        void ReadFromBuffer(
            ID3D11Buffer* in_stagingBuffer,
            void*         out_readData,
            uint32*       out_readDataByteWidth)
        {
            if (!in_stagingBuffer)
            {
                DEBUG_BREAK("Staging buffer is nullptr.");
                return;
            }

            if (!out_readData)
            {
                DEBUG_BREAK("out_readData is nullptr.");
                return;
            }

            D3D11_BUFFER_DESC desc;
            in_stagingBuffer->GetDesc(&desc);

            D3D11_MAPPED_SUBRESOURCE mappedResource = {};
            if (d3d11::CheckOK(
                    GetRenderer().GetDeviceContext()->Map(
                        in_stagingBuffer,
                        0,
                        D3D11_MAP_READ,
                        0,
                        &mappedResource),
                    "Map Failed."))
            {

                eBlendWriteFlags* src = static_cast<eBlendWriteFlags*>(mappedResource.pData);
                ::memcpy(out_readData, src, desc.ByteWidth);

                if (out_readDataByteWidth)
                    *out_readDataByteWidth = desc.ByteWidth;

                GetRenderer().GetDeviceContext()->Unmap(in_stagingBuffer, 0);
            }
        }

        void CopyBetween(
            ID3D11Buffer* in_srcBuffer,
            ID3D11Buffer* in_dstBuffer)
        {
            D3D11_BUFFER_DESC srcDesc;
            in_srcBuffer->GetDesc(&srcDesc);

            D3D11_BUFFER_DESC dstDesc;
            in_dstBuffer->GetDesc(&dstDesc);

            if (srcDesc.ByteWidth != dstDesc.ByteWidth
                || srcDesc.Usage != D3D11_USAGE_DEFAULT
                || dstDesc.Usage != D3D11_USAGE_DEFAULT)
            {
                DEBUG_BREAK("Buffer size or usage is not equal.");
                return;
            }
            else
            {
                auto c = GetRenderer().GetDeviceContext();
                c->CopyResource(in_dstBuffer, in_srcBuffer);
            }
        }
    }   // namespace buffer

    namespace shader
    {

        ComPtr<ID3DBlob> CompileShaderCode(
            std::string_view    in_shaderCode,
            std::string_view    in_entryPoint,
            std::string_view    in_shaderModel,
            const ShaderMacros& in_shaderMacros)
        {
            uint32 compileFlags = 0;
#ifdef _DEBUG
            compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else

            compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

            ComPtr<ID3DBlob> shaderBlob;
            ComPtr<ID3DBlob> errorBlob;

            std::vector<D3D_SHADER_MACRO> macros  = in_shaderMacros.Get();
            const D3D_SHADER_MACRO*       pMacros = macros.empty() ? nullptr : macros.data();

            d3d11::CheckOK(D3DCompile(in_shaderCode.data(),
                                      in_shaderCode.size(),
                                      nullptr,
                                      pMacros,
                                      D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                      in_entryPoint.data(),
                                      in_shaderModel.data(),
                                      compileFlags,
                                      0,
                                      shaderBlob.GetAddressOf(),
                                      errorBlob.GetAddressOf()),
                           "D3DCompile Fail.");

            if (errorBlob)
            {
                DEBUG_BREAK(fmt::format(
                                "Error Blob: {0}", static_cast<char*>(errorBlob->GetBufferPointer()))
                                .data());
                return nullptr;
            }

            return shaderBlob;
        }

        ComPtr<ID3DBlob> LoadFromHLSL(
            const std::filesystem::path& in_shaderPath,
            std::string_view             in_entryPoint,
            std::string_view             in_shaderModel,
            const ShaderMacros&          in_shaderMacros)
        {
            if (in_shaderPath.extension() != ".hlsl")
            {
                DEBUG_BREAK("File extension is not .hlsl.");
                return nullptr;
            }

            uint32 compileFlags = 0;
#ifdef _DEBUG
            compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
            compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
            ComPtr<ID3DBlob> shaderBlob;
            ComPtr<ID3DBlob> errorBlob;

            std::vector<D3D_SHADER_MACRO> macros  = in_shaderMacros.Get();
            const D3D_SHADER_MACRO*       pMacros = macros.empty() ? nullptr : macros.data();

            CheckOK(D3DCompileFromFile(in_shaderPath.c_str(),
                                       pMacros,
                                       D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                       in_entryPoint.data(),
                                       in_shaderModel.data(),
                                       compileFlags,
                                       0,
                                       shaderBlob.GetAddressOf(),
                                       errorBlob.GetAddressOf()),
                    "D3DCompileFromFile Fail.");

            if (errorBlob)
            {
                DEBUG_BREAK(fmt::format(
                                "Error Blob: {0}", static_cast<char*>(errorBlob->GetBufferPointer()))
                                .data());
                return nullptr;
            }

            return shaderBlob;
        }

        ComPtr<ID3DBlob> LoadFromCSO(const std::filesystem::path& in_shaderPath)
        {
            if (in_shaderPath.extension() != ".cso")
            {
                DEBUG_BREAK("File extension is not .cso.");
                return nullptr;
            }

            uint32 compileFlags = 0;
#ifdef _DEBUG
            compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
            compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
            ComPtr<ID3DBlob> shaderBlob;
            ComPtr<ID3DBlob> errorBlob;

            d3d11::CheckOK(D3DReadFileToBlob(
                               in_shaderPath.c_str(),
                               shaderBlob.GetAddressOf()),
                           "D3DReadFileToBlob Fail.");

            if (errorBlob)
            {
                DEBUG_BREAK(fmt::format(
                                "Error Blob: {0}", static_cast<char*>(errorBlob->GetBufferPointer()))
                                .data());
                return nullptr;
            }

            return shaderBlob;
        }

    }   // namespace shader

    namespace views
    {

        ComPtr<ID3D11ShaderResourceView> CreateSRV(
            ID3D11Resource* in_resource,
            eFormat         in_srvFormat)
        {
            D3D11_RESOURCE_DIMENSION resDim = D3D11_RESOURCE_DIMENSION_UNKNOWN;
            in_resource->GetType(&resDim);

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            ZeroMemory(&srvDesc, sizeof(srvDesc));

            switch (resDim)
            {
                case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
                {
                    D3D11_TEXTURE1D_DESC tex1D = {};
                    static_cast<ID3D11Texture1D*>(in_resource)->GetDesc(&tex1D);

                    srvDesc.Format = static_cast<DXGI_FORMAT>(in_srvFormat);
                    if (tex1D.ArraySize > 1)
                    {
                        srvDesc.ViewDimension                  = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
                        srvDesc.Texture1DArray.MostDetailedMip = 0;
                        srvDesc.Texture1DArray.MipLevels       = tex1D.MipLevels;
                        srvDesc.Texture1DArray.FirstArraySlice = 0;
                        srvDesc.Texture1DArray.ArraySize       = tex1D.ArraySize;
                    }
                    else
                    {
                        srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE1D;
                        srvDesc.Texture1D.MostDetailedMip = 0;
                        srvDesc.Texture1D.MipLevels       = tex1D.MipLevels;
                    }
                }
                break;

                case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
                {
                    D3D11_TEXTURE2D_DESC tex2D = {};
                    static_cast<ID3D11Texture2D*>(in_resource)->GetDesc(&tex2D);

                    srvDesc.Format = static_cast<DXGI_FORMAT>(in_srvFormat);

                    if (tex2D.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE)
                    {
                        if (tex2D.ArraySize > 6)
                        {
                            srvDesc.ViewDimension                     = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
                            srvDesc.TextureCubeArray.MostDetailedMip  = 0;
                            srvDesc.TextureCubeArray.MipLevels        = tex2D.MipLevels;
                            srvDesc.TextureCubeArray.First2DArrayFace = 0;
                            srvDesc.TextureCubeArray.NumCubes         = tex2D.ArraySize / 6;
                        }
                        else
                        {
                            srvDesc.ViewDimension               = D3D11_SRV_DIMENSION_TEXTURECUBE;
                            srvDesc.TextureCube.MostDetailedMip = 0;
                            srvDesc.TextureCube.MipLevels       = tex2D.MipLevels;
                        }
                    }
                    else if (tex2D.SampleDesc.Count > 1)
                    {
                        if (tex2D.ArraySize > 1)
                        {
                            srvDesc.ViewDimension                    = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
                            srvDesc.Texture2DMSArray.FirstArraySlice = 0;
                            srvDesc.Texture2DMSArray.ArraySize       = tex2D.ArraySize;
                        }
                        else
                        {
                            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
                        }
                    }
                    else
                    {
                        if (tex2D.ArraySize > 1)
                        {
                            srvDesc.ViewDimension                  = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                            srvDesc.Texture2DArray.MostDetailedMip = 0;
                            srvDesc.Texture2DArray.MipLevels       = tex2D.MipLevels;
                            srvDesc.Texture2DArray.FirstArraySlice = 0;
                            srvDesc.Texture2DArray.ArraySize       = tex2D.ArraySize;
                        }
                        else
                        {
                            srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
                            srvDesc.Texture2D.MostDetailedMip = 0;
                            srvDesc.Texture2D.MipLevels       = tex2D.MipLevels;
                        }
                    }
                }
                break;

                case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
                {
                    D3D11_TEXTURE3D_DESC tex3D = {};
                    static_cast<ID3D11Texture3D*>(in_resource)->GetDesc(&tex3D);

                    srvDesc.Format                    = static_cast<DXGI_FORMAT>(in_srvFormat);
                    srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE3D;
                    srvDesc.Texture3D.MostDetailedMip = 0;
                    srvDesc.Texture3D.MipLevels       = tex3D.MipLevels;
                }
                break;

                case D3D11_RESOURCE_DIMENSION_BUFFER:
                {
                    D3D11_BUFFER_DESC bufferDesc = {};
                    static_cast<ID3D11Buffer*>(in_resource)->GetDesc(&bufferDesc);

                    srvDesc.Format                = DXGI_FORMAT_UNKNOWN;
                    srvDesc.ViewDimension         = D3D11_SRV_DIMENSION_BUFFEREX;
                    srvDesc.BufferEx.FirstElement = 0;
                    srvDesc.BufferEx.NumElements  = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
                    srvDesc.BufferEx.Flags        = 0;
                }
                break;

                default:
                    DEBUG_BREAK("Unknown Resource Dimension.");
                    break;
            }

            ComPtr<ID3D11ShaderResourceView> srv;
            HRESULT                          hr = GetRenderer().GetDevice()->CreateShaderResourceView(in_resource, &srvDesc, srv.GetAddressOf());
            d3d11::CheckOK(hr, "CreateShaderResourceView Fail.");
            return srv;
        }

        ComPtr<ID3D11DepthStencilView> CreateDSV(
            ID3D11Texture2D* in_texture,
            eFormat          in_dsvFormat)
        {
            ComPtr<ID3D11DepthStencilView> dsv;

            D3D11_TEXTURE2D_DESC texDesc;
            in_texture->GetDesc(&texDesc);

            D3D11_DEPTH_STENCIL_VIEW_DESC desc;
            desc.Format = static_cast<DXGI_FORMAT>(in_dsvFormat);
            desc.Flags  = 0;

            if (texDesc.ArraySize > 1)
            {
                if (texDesc.SampleDesc.Count > 1)
                {
                    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
                }
                else
                {
                    desc.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
                    desc.Texture2DArray.MipSlice        = 0;
                    desc.Texture2DArray.FirstArraySlice = 0;
                    desc.Texture2DArray.ArraySize       = texDesc.ArraySize;
                }
            }
            else
            {
                if (texDesc.SampleDesc.Count > 1)
                {
                    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
                }
                else
                {
                    desc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
                    desc.Texture2D.MipSlice = 0;
                }
            }

            d3d11::CheckOK(GetRenderer().GetDevice()->CreateDepthStencilView(
                               in_texture,
                               &desc,
                               dsv.GetAddressOf()),
                           "CreateDSV Fail.");

            return dsv;
        }

        ComPtr<ID3D11UnorderedAccessView> CreateUAV(
            ID3D11Buffer* in_buffer)
        {
            D3D11_BUFFER_DESC bufferDesc = {};
            in_buffer->GetDesc(&bufferDesc);

            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            uavDesc.Format                           = DXGI_FORMAT_UNKNOWN;
            uavDesc.ViewDimension                    = D3D11_UAV_DIMENSION_BUFFER;
            uavDesc.Buffer.FirstElement              = 0;
            uavDesc.Buffer.NumElements               = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
            uavDesc.Buffer.Flags                     = 0;

            ComPtr<ID3D11UnorderedAccessView> uav;
            d3d11::CheckOK(
                GetRenderer().GetDevice()->CreateUnorderedAccessView(
                    in_buffer,
                    &uavDesc,
                    uav.GetAddressOf()),
                "CreateUnorderedAccessView Fail.");

            return uav;
        }
    }   // namespace views

    uint32 GetFormatByteStride(eFormat in_format)
    {
        switch (in_format)
        {
            case eFormat::Float32_1:
            case eFormat::Typeless32_1:
                return 4;

            case eFormat::Float32_2:
                return 4 * 2;
            case eFormat::Float32_3:
                return 4 * 3;
            case eFormat::Float32_4:
                return 4 * 4;

            case eFormat::Float16_1:
                return 2;
            case eFormat::Float16_2:
                return 2 * 2;
            case eFormat::Float16_4:
                return 2 * 4;

            case eFormat::UNorm8_1:
                return 1;
            case eFormat::UNorm8_2:
                return 1 * 2;
            case eFormat::UNorm8_4:
                return 1 * 4;

            case eFormat::Depth_UNorm16:
                return 2;

            case eFormat::Depth_UNorm24_Stencil_UInt8:
            case eFormat::Depth_Float32:
                return 4;

            case eFormat::Depth_Float32_Stencil_UInt8:
                return 4 * 2;

            default:
                DEBUG_BREAK("Unknown Format.");
                return 0;
        }
    }

    uint32 GetFormatByteStride(DXGI_FORMAT in_format)
    {
        return static_cast<uint32>(DirectX::BitsPerPixel(in_format) / 8);
    }

    D3D11_FILTER GetSamplerFilter(eSamplerMode in_mode, eSamplerFilter in_filter)
    {
        if (in_mode == eSamplerMode::Default)
        {
            switch (in_filter)
            {
                case eSamplerFilter::MinMagMipPoint:
                    return D3D11_FILTER_MIN_MAG_MIP_POINT;
                case eSamplerFilter::MinMagPoint_MipLinear:
                    return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
                case eSamplerFilter::MinPoint_MagLinear_MipPoint:
                    return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
                case eSamplerFilter::MinPoint_MagMipLinear:
                    return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
                case eSamplerFilter::MinLinear_MagMipPoint:
                    return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
                case eSamplerFilter::MinLinear_MagPoint_MipLinear:
                    return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                case eSamplerFilter::MinMagLinear_MipPoint:
                    return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
                case eSamplerFilter::MinMagMipLinear:
                    return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                case eSamplerFilter::Anisotropic:
                    return D3D11_FILTER_ANISOTROPIC;
                default:
                    DEBUG_BREAK("Invalid Sampler Filter");
                    return D3D11_FILTER_MIN_MAG_MIP_POINT;
            }
        }
        else if (in_mode == eSamplerMode::Comparison)
        {
            switch (in_filter)
            {
                case eSamplerFilter::MinMagMipPoint:
                    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
                case eSamplerFilter::MinMagPoint_MipLinear:
                    return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
                case eSamplerFilter::MinPoint_MagLinear_MipPoint:
                    return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
                case eSamplerFilter::MinPoint_MagMipLinear:
                    return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
                case eSamplerFilter::MinLinear_MagMipPoint:
                    return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
                case eSamplerFilter::MinLinear_MagPoint_MipLinear:
                    return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                case eSamplerFilter::MinMagLinear_MipPoint:
                    return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
                case eSamplerFilter::MinMagMipLinear:
                    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
                case eSamplerFilter::Anisotropic:
                    return D3D11_FILTER_COMPARISON_ANISOTROPIC;
                default:
                    DEBUG_BREAK("Invalid Sampler Filter");
                    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
            }
        }
        else if (in_mode == eSamplerMode::Minimum)
        {
            switch (in_filter)
            {
                case eSamplerFilter::MinMagMipPoint:
                    return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
                case eSamplerFilter::MinMagPoint_MipLinear:
                    return D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
                case eSamplerFilter::MinPoint_MagLinear_MipPoint:
                    return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
                case eSamplerFilter::MinPoint_MagMipLinear:
                    return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
                case eSamplerFilter::MinLinear_MagMipPoint:
                    return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
                case eSamplerFilter::MinLinear_MagPoint_MipLinear:
                    return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                case eSamplerFilter::MinMagLinear_MipPoint:
                    return D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
                case eSamplerFilter::MinMagMipLinear:
                    return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;
                case eSamplerFilter::Anisotropic:
                    return D3D11_FILTER_MINIMUM_ANISOTROPIC;
                default:
                    DEBUG_BREAK("Invalid Sampler Filter");
                    return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
            }
        }
        else if (in_mode == eSamplerMode::Maximum)
        {
            switch (in_filter)
            {
                case eSamplerFilter::MinMagMipPoint:
                    return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
                case eSamplerFilter::MinMagPoint_MipLinear:
                    return D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
                case eSamplerFilter::MinPoint_MagLinear_MipPoint:
                    return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
                case eSamplerFilter::MinPoint_MagMipLinear:
                    return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;
                case eSamplerFilter::MinLinear_MagMipPoint:
                    return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
                case eSamplerFilter::MinLinear_MagPoint_MipLinear:
                    return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                case eSamplerFilter::MinMagLinear_MipPoint:
                    return D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
                case eSamplerFilter::MinMagMipLinear:
                    return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
                case eSamplerFilter::Anisotropic:
                    return D3D11_FILTER_MAXIMUM_ANISOTROPIC;
                default:
                    DEBUG_BREAK("Invalid Sampler Filter");
                    return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
            }
        }

        return D3D11_FILTER_MIN_MAG_MIP_POINT;
    }

    const char* ConvertHLSLFormatString(eFormat in_format)
    {
        switch (in_format)
        {
            case eFormat::Float32_1:
                return "float";

            case eFormat::Float32_2:
                return "float2";

            case eFormat::Float32_3:
                return "float3";

            case eFormat::Float32_4:
                return "float4";

            case eFormat::UInt32_1:
                return "uint";

            case eFormat::UInt32_2:
                return "uint2";

            case eFormat::UInt32_4:
                return "uint4";

            case eFormat::SInt32_1:
                return "int";

            case eFormat::SInt32_2:
                return "int2";

            case eFormat::SInt32_4:
                return "int4";

            default:
                DEBUG_BREAK("Unknown Format.");
                return "unknown";
        }
    }
}   // namespace d3d11

}   // namespace crab
