#include "CrabPch.h"

#include "D11Utils.h"

#include "D11Renderer.h"

namespace crab
{

ComPtr<ID3D11Texture2D> ID3D11Texture2DUtil::CreateTexture2D(
    uint32          in_width,
    uint32          in_height,
    eFormat         in_format,
    D3D11_USAGE     in_usage,
    eBindFlags      in_bindFlags,
    eCPUAccessFlags in_cpuAccessFlags,
    void*           in_initData,
    uint32          in_sizeOfInitDataType,
    uint32          in_MSAASampleCount,
    uint32          in_MSAASampleQuality,
    uint32          in_numberOfMipmap,
    uint32          in_textureArraySize)
{
    ComPtr<ID3D11Texture2D> texture;
    D3D11_TEXTURE2D_DESC    desc = {};
    desc.Width                   = in_width;
    desc.Height                  = in_height;
    desc.MipLevels               = in_numberOfMipmap;
    desc.ArraySize               = in_textureArraySize;
    desc.Format                  = static_cast<DXGI_FORMAT>(in_format);
    desc.SampleDesc.Count        = in_MSAASampleCount;
    desc.SampleDesc.Quality      = in_MSAASampleQuality;
    desc.Usage                   = in_usage;
    desc.BindFlags               = in_bindFlags;
    desc.CPUAccessFlags          = in_cpuAccessFlags;
    desc.MiscFlags               = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem                = in_initData;
    initData.SysMemPitch            = in_width * in_sizeOfInitDataType;
    initData.SysMemSlicePitch       = in_width * in_height * in_sizeOfInitDataType;

    CheckD3D11Result(GetRenderer().GetDevice()->CreateTexture2D(&desc,
                                                                &initData,
                                                                texture.GetAddressOf()),
                     "CreateTexture2D Fail.");

    return texture;
}

ComPtr<ID3D11Texture2D> ID3D11Texture2DUtil::CreateTexture2D(uint32 in_width, uint32 in_height, eFormat in_format, D3D11_USAGE in_usage, eBindFlags in_bindFlags, eCPUAccessFlags in_cpuAccessFlags, uint32 in_MSAASampleCount /*= 1*/, uint32 in_MSAASampleQuality /*= 0*/, uint32 in_numberOfMipmap /*= 1*/, uint32 in_textureArraySize /*= 1*/)
{
    ComPtr<ID3D11Texture2D> texture;
    D3D11_TEXTURE2D_DESC    desc = {};
    desc.Width                   = in_width;
    desc.Height                  = in_height;
    desc.MipLevels               = in_numberOfMipmap;
    desc.ArraySize               = in_textureArraySize;
    desc.Format                  = static_cast<DXGI_FORMAT>(in_format);
    desc.SampleDesc.Count        = in_MSAASampleCount;
    desc.SampleDesc.Quality      = in_MSAASampleQuality;
    desc.Usage                   = in_usage;
    desc.BindFlags               = in_bindFlags;
    desc.CPUAccessFlags          = in_cpuAccessFlags;
    desc.MiscFlags               = 0;

    CheckD3D11Result(GetRenderer().GetDevice()->CreateTexture2D(&desc,
                                                                nullptr,
                                                                texture.GetAddressOf()),
                     "CreateTexture2D Fail.");

    return texture;
}

ComPtr<ID3D11Texture2D> ID3D11Texture2DUtil::CreateStagingTexture2D(uint32 in_width, uint32 in_height, eFormat in_format, bool in_cpuRead, bool in_cpuWrite, uint32 in_arraySize /*= 1*/)
{
    eCPUAccessFlags flags = eCPUAccessFlags_None;

    if (in_cpuRead)
        flags |= eCPUAccessFlags_Read;

    if (in_cpuWrite)
        flags |= eCPUAccessFlags_Write;

    CheckD3D11Result(flags != eCPUAccessFlags_None, "CPU Access Flags is None.");

    return CreateTexture2D(
        in_width,
        in_height,
        in_format,
        D3D11_USAGE_STAGING,
        eBindFlags_None,
        flags,
        1,
        0,
        1,
        in_arraySize);
}

crab::ComPtr<ID3D11Texture2D> ID3D11Texture2DUtil::CreateDefaultTexture2D(uint32 in_width, uint32 in_height, eFormat in_format, eBindFlags in_bindFlags, uint32 in_MSAASampleCount /*= 1*/, uint32 in_MSAASampleQuality /*= 0*/, uint32 in_numberOfMipmap /*= 1*/, uint32 in_textureArraySize /*= 1*/)
{
    return CreateTexture2D(
        in_width,
        in_height,
        in_format,
        D3D11_USAGE_DEFAULT,
        in_bindFlags,
        eCPUAccessFlags_None,
        in_MSAASampleCount,
        in_MSAASampleQuality,
        in_numberOfMipmap,
        in_textureArraySize);
}

crab::ComPtr<ID3D11Texture2D> ID3D11Texture2DUtil::CreateDynamicTexture2D(uint32 in_width, uint32 in_height, eFormat in_format, eBindFlags in_bindFlags, uint32 in_MSAASampleCount /*= 1*/, uint32 in_MSAASampleQuality /*= 0*/, uint32 in_numberOfMipmap /*= 1*/, uint32 in_textureArraySize /*= 1*/)
{
    return CreateTexture2D(
        in_width,
        in_height,
        in_format,
        D3D11_USAGE_DYNAMIC,
        in_bindFlags,
        eCPUAccessFlags_Write,
        in_MSAASampleCount,
        in_MSAASampleQuality,
        in_numberOfMipmap,
        in_textureArraySize);
}

void ID3D11Texture2DUtil::WriteToDefaultTexture(ID3D11Texture2D* in_defaultTexture, void* in_srcData, uint32 in_pixelStride, uint32 in_width)
{
    if (!in_defaultTexture)
    {
        CRAB_DEBUG_BREAK("Texture is nullptr.");
        return;
    }

    GetRenderer().GetContext()->UpdateSubresource(

        in_defaultTexture,
        0,
        nullptr,
        in_srcData,
        in_pixelStride * in_width,
        0);
}

void ID3D11Texture2DUtil::WriteToDynamicTexture(ID3D11Texture2D* in_dynamicTexture, void* in_srcData, uint32 in_pixelStride, uint32 in_width, uint32 in_height)
{
    if (!in_dynamicTexture)
    {
        CRAB_DEBUG_BREAK("Texture is nullptr.");
        return;
    }

    auto c = GetRenderer().GetContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    if (CheckD3D11Result(c->Map(in_dynamicTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        uint8* dst      = static_cast<uint8*>(mappedResource.pData);
        uint8* src      = static_cast<uint8*>(in_srcData);
        uint32 rowPitch = in_pixelStride * in_width;

        for (uint32 y = 0; y < in_height; ++y)
        {
            ::memcpy(dst + y * mappedResource.RowPitch, src + y * rowPitch, rowPitch);
        }

        c->Unmap(in_dynamicTexture, 0);
    }
    else
    {
        CRAB_DEBUG_BREAK("Map Failed.");
    }
}

void ID3D11Texture2DUtil::WriteToStagingTexture(
    ID3D11Texture2D* in_stagingTexture,
    void*            in_srcData,
    uint32           in_pixelStride,
    uint32           in_width,
    uint32           in_height)
{
    if (!in_stagingTexture)
    {
        CRAB_DEBUG_BREAK("Texture is nullptr.");
        return;
    }

    auto c = GetRenderer().GetContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    if (CheckD3D11Result(c->Map(in_stagingTexture, 0, D3D11_MAP_WRITE, 0, &mappedResource)))
    {
        uint8* dst      = static_cast<uint8*>(mappedResource.pData);
        uint8* src      = static_cast<uint8*>(in_srcData);
        uint32 rowPitch = in_pixelStride * in_width;

        for (uint32 y = 0; y < in_height; ++y)
            ::memcpy(dst + y * mappedResource.RowPitch, src + y * rowPitch, rowPitch);

        c->Unmap(in_stagingTexture, 0);
    }
    else
    {
        CRAB_DEBUG_BREAK("Map Failed.");
    }
}

void ID3D11Texture2DUtil::ReadFromStagingTexture(ID3D11Texture2D* in_stagingTexture, void* out_data)
{
    if (!in_stagingTexture)
    {
        CRAB_DEBUG_BREAK("Staging texture is nullptr.");
        return;
    }

    if (!out_data)
    {
        CRAB_DEBUG_BREAK("Out data is nullptr.");
        return;
    }

    auto                 c = GetRenderer().GetContext();
    D3D11_TEXTURE2D_DESC desc;
    in_stagingTexture->GetDesc(&desc);

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};

    if (SUCCEEDED(c->Map(in_stagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource)))
    {
        uint8* dst      = static_cast<uint8*>(out_data);
        uint8* src      = static_cast<uint8*>(mappedResource.pData);
        uint32 rowPitch = desc.Width * desc.Format;
        for (uint32 y = 0; y < desc.Height; ++y)
        {
            ::memcpy(dst + y * rowPitch, src + y * mappedResource.RowPitch, rowPitch);
        }
        c->Unmap(in_stagingTexture, 0);
    }
    else
    {
        CRAB_DEBUG_BREAK("Map Failed.");
    }
}

void ID3D11Texture2DUtil::ReadFromStagingTexture(
    ID3D11Texture2D* in_stagingTexture,
    void*            out_data,
    uint32           in_pixelStride,
    uint32           in_offsetX,
    uint32           in_offsetY,
    uint32           in_width,
    uint32           in_height)
{
    if (!in_stagingTexture)
    {
        CRAB_DEBUG_BREAK("Staging texture is nullptr.");
    }
    else
    {
        auto c = GetRenderer().GetContext();

        D3D11_MAPPED_SUBRESOURCE mappedResource = {};
        if (SUCCEEDED(c->Map(in_stagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource)))
        {
            uint8* dst      = static_cast<uint8*>(out_data);
            uint8* src      = static_cast<uint8*>(mappedResource.pData);
            uint32 rowPitch = in_pixelStride * in_width;
            for (uint32 y = 0; y < in_height; ++y)
            {
                ::memcpy(dst + y * rowPitch, src + (y + in_offsetY) * mappedResource.RowPitch + in_offsetX * in_pixelStride, rowPitch);
            }
            c->Unmap(in_stagingTexture, 0);
        }
        else
        {
            CRAB_DEBUG_BREAK("Map Failed.");
        }
    }
}

void ID3D11Texture2DUtil::CopyBetween(ID3D11Texture2D* in_srcTexture, ID3D11Texture2D* in_dstTexture)
{
    if (in_srcTexture && in_dstTexture)
    {
        auto context = GetRenderer().GetContext();
        context->CopyResource(in_dstTexture, in_srcTexture);
    }
    else
    {
        CRAB_DEBUG_BREAK("One or both textures are nullptr.");
    }
}

void ID3D11Texture2DUtil::CopyBetween(
    ID3D11Texture2D* in_srcTexture,
    uint32           in_srcOffsetX,
    uint32           in_srcOffsetY,
    uint32           in_srcWidth,
    uint32           in_srcHeight,
    ID3D11Texture2D* in_dstTexture,
    uint32           in_dstOffsetX,
    uint32           in_dstOffsetY)
{
    if (in_srcTexture && in_dstTexture)
    {
        auto c = GetRenderer().GetContext();

        D3D11_BOX box = {};
        box.left      = in_srcOffsetX;
        box.top       = in_srcOffsetY;
        box.right     = in_srcOffsetX + in_srcWidth;
        box.bottom    = in_srcOffsetY + in_srcHeight;
        box.front     = 0;
        box.back      = 1;

        c->CopySubresourceRegion(
            in_dstTexture,
            0,
            in_dstOffsetX,
            in_dstOffsetY,
            0,
            in_srcTexture,
            0,
            &box);
    }
    else
    {
        CRAB_DEBUG_BREAK("One or both textures are nullptr.");
    }
}

void ID3D11Texture2DUtil::Texture2DMSToTexture2D(
    ID3D11Texture2D* in_MSTexture,
    ID3D11Texture2D* in_texture,
    eFormat          in_targetFormat)
{
    if (in_MSTexture && in_texture)
    {
        auto c = GetRenderer().GetContext();
        c->ResolveSubresource(in_texture,
                              0,
                              in_MSTexture,
                              0,
                              static_cast<DXGI_FORMAT>(in_targetFormat));
    }
    else
    {
        CRAB_DEBUG_BREAK("One or both textures are nullptr.");
    }
}

ComPtr<ID3D11Buffer> ID3D11BufferUtil::CreateBuffer(
    uint32          in_bufferByteWidth,
    D3D11_USAGE     in_usage,
    eBindFlags      in_bindFlags,
    eCPUAccessFlags in_cpuAccessFlags,
    uint32          in_structureDataStride,
    const void*     in_initialData)
{
    ComPtr<ID3D11Buffer> buffer;

    D3D11_BUFFER_DESC desc   = {};
    desc.Usage               = in_usage;
    desc.ByteWidth           = in_bufferByteWidth;
    desc.BindFlags           = in_bindFlags;
    desc.CPUAccessFlags      = in_cpuAccessFlags;
    desc.StructureByteStride = in_structureDataStride;

    if (in_initialData)
    {
        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem                = in_initialData;

        CheckD3D11Result(
            GetRenderer().GetDevice()->CreateBuffer(&desc, &data, buffer.GetAddressOf()),
            "CreateBuffer Fail.");
    }
    else
    {
        CheckD3D11Result(
            GetRenderer().GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf()),
            "CreateBuffer Fail.");
    }

    return buffer;
}

ComPtr<ID3D11Buffer> ID3D11BufferUtil::CreateConstantBuffer(uint32 in_bufferSize)
{
    return CreateBuffer(in_bufferSize,
                        D3D11_USAGE_DYNAMIC,
                        eBindFlags_ConstantBuffer,
                        eCPUAccessFlags_Write,
                        0,
                        nullptr);
}

ComPtr<ID3D11Buffer> ID3D11BufferUtil::CreateVertexBuffer(uint32 in_sizePerVertex, uint32 in_vertexCount, const void* in_data)
{
    return CreateBuffer(in_sizePerVertex * in_vertexCount,
                        D3D11_USAGE_IMMUTABLE,
                        eBindFlags_VertexBuffer,
                        eCPUAccessFlags_None,
                        0,
                        in_data);
}

ComPtr<ID3D11Buffer> ID3D11BufferUtil::CreateIndexBuffer(uint32 in_indexCount, const void* in_data)
{
    return CreateBuffer(sizeof(uint32) * in_indexCount,
                        D3D11_USAGE_IMMUTABLE,
                        eBindFlags_IndexBuffer,
                        eCPUAccessFlags_None,
                        0,
                        in_data);
}

ComPtr<ID3D11Buffer> ID3D11BufferUtil::CreateStructuredBuffer(
    uint32      in_itemMaxCount,
    uint32      in_itemByteStride,
    const void* in_data)
{
    auto d = GetRenderer().GetDevice();

    D3D11_BUFFER_DESC desc   = {};
    desc.ByteWidth           = in_itemMaxCount * in_itemByteStride;
    desc.Usage               = D3D11_USAGE_DEFAULT;
    desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    desc.CPUAccessFlags      = 0;
    desc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    desc.StructureByteStride = in_itemByteStride;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem                = in_data;

    ComPtr<ID3D11Buffer> buffer;
    CheckD3D11Result(d->CreateBuffer(&desc, &initData, buffer.GetAddressOf()), "CreateBuffer Fail.");

    return buffer;
}

ComPtr<ID3D11Buffer> ID3D11BufferUtil::CreateStagingBuffer(
    uint32 in_bufferByteWidth,
    bool   in_cpuWrite,
    bool   in_cpuRead)
{
    eCPUAccessFlags flags = eCPUAccessFlags_None;

    if (in_cpuWrite)
        flags |= eCPUAccessFlags_Write;

    if (in_cpuRead)
        flags |= eCPUAccessFlags_Read;

    return CreateBuffer(
        in_bufferByteWidth,
        D3D11_USAGE_STAGING,
        eBindFlags_None,
        flags,
        0,
        nullptr);
}

void ID3D11BufferUtil::WriteToDynamicBuffer(
    ID3D11Buffer* in_buffer,
    const void*   in_data,
    uint32        in_byteWidth)
{
    auto c = GetRenderer().GetContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    if (SUCCEEDED(c->Map(in_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        memcpy(mappedResource.pData, in_data, in_byteWidth);
        c->Unmap(in_buffer, 0);
    }
    else
    {
        CRAB_DEBUG_BREAK("Map Failed.");
    }
}

void ID3D11BufferUtil::WriteToStagingBuffer(ID3D11Buffer* in_buffer, const void* in_data, uint32 in_byteWidth)
{
    auto c = GetRenderer().GetContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    if (SUCCEEDED(c->Map(in_buffer, 0, D3D11_MAP_WRITE, 0, &mappedResource)))
    {
        memcpy(mappedResource.pData, in_data, in_byteWidth);
        c->Unmap(in_buffer, 0);
    }
    else
    {
        CRAB_DEBUG_BREAK("Map Failed.");
    }
}

void ID3D11BufferUtil::ReadFromBuffer(
    ID3D11Buffer* in_buffer,
    void*         out_data,
    uint32        in_byteWidth)
{
    auto c = GetRenderer().GetContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    if (SUCCEEDED(c->Map(in_buffer, 0, D3D11_MAP_READ, 0, &mappedResource)))
    {
        memcpy(out_data, mappedResource.pData, in_byteWidth);
        c->Unmap(in_buffer, 0);
    }
    else
    {
        CRAB_DEBUG_BREAK("Map Failed.");
    }
}

void ID3D11BufferUtil::CopyBetween(ID3D11Buffer* in_srcBuffer, ID3D11Buffer* in_dstBuffer, uint32 in_byteWidth)
{
    auto c = GetRenderer().GetContext();
    c->CopyResource(in_dstBuffer, in_srcBuffer);
}

ComPtr<ID3DBlob> ShaderUtil::LoadShaderCode(
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

    CheckD3D11Result(D3DCompile(in_shaderCode.data(),
                                in_shaderCode.size(),
                                nullptr,
                                in_shaderMacros.Get(),
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
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", static_cast<char*>(errorBlob->GetBufferPointer()));
        return nullptr;
    }

    return shaderBlob;
}

ComPtr<ID3DBlob> ShaderUtil::LoadShaderFile(
    const std::filesystem::path& in_shaderPath,
    std::string_view             in_entryPoint,
    std::string_view             in_shaderModel,
    const ShaderMacros&          in_shaderMacros)
{
    uint32 compileFlags = 0;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    auto extension = in_shaderPath.filename().extension();

    if (extension == ".hlsl")
    {
        CheckD3D11Result(D3DCompileFromFile(in_shaderPath.c_str(),
                                            in_shaderMacros.Get(),
                                            D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                            in_entryPoint.data(),
                                            in_shaderModel.data(),
                                            compileFlags,
                                            0,
                                            shaderBlob.GetAddressOf(),
                                            errorBlob.GetAddressOf()),
                         "D3DCompileFromFile Fail.");
    }
    else if (extension == ".cso")
    {
        CheckD3D11Result(D3DReadFileToBlob(
                             in_shaderPath.c_str(),
                             shaderBlob.GetAddressOf()),
                         "D3DReadFileToBlob Fail.");
    }
    else
    {
        CRAB_DEBUG_BREAK("Unknown file extension.");
    }

    if (errorBlob)
    {
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", static_cast<char*>(errorBlob->GetBufferPointer()));
        return nullptr;
    }

    return shaderBlob;
}

ComPtr<ID3D11ShaderResourceView> ID3D11ShaderResourceViewUtil::CreateTexture2DSRV(ID3D11Texture2D* in_texture, eFormat in_format)
{
    auto dx = GetRenderer().GetDevice();

    ComPtr<ID3D11ShaderResourceView> srv;

    D3D11_TEXTURE2D_DESC texDesc;
    in_texture->GetDesc(&texDesc);
    DXGI_FORMAT format = in_format == eFormat::Unknown ? texDesc.Format : static_cast<DXGI_FORMAT>(in_format);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format                          = format;

    if (texDesc.SampleDesc.Count > 1)
    {
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    }
    else
    {
        srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels       = texDesc.MipLevels;
    }

    CheckD3D11Result(dx->CreateShaderResourceView(
                         in_texture,
                         &srvDesc,
                         srv.GetAddressOf()),
                     "CreateShaderResourceView Fail.");

    return srv;
}

ComPtr<ID3D11ShaderResourceView> ID3D11ShaderResourceViewUtil::CreateBufferSRV(
    ID3D11Buffer* in_buffer)
{
    auto d = GetRenderer().GetDevice();

    D3D11_BUFFER_DESC bufferDesc = {};
    in_buffer->GetDesc(&bufferDesc);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.Format                = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension         = D3D11_SRV_DIMENSION_BUFFEREX;
    srvDesc.BufferEx.FirstElement = 0;
    srvDesc.BufferEx.NumElements  = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
    srvDesc.BufferEx.Flags        = 0;

    ComPtr<ID3D11ShaderResourceView> srv;
    CheckD3D11Result(
        d->CreateShaderResourceView(
            in_buffer,
            &srvDesc,
            srv.GetAddressOf()),
        "CreateShaderResourceView Fail.");
    return srv;
}

ComPtr<ID3D11DepthStencilView> ID3D11DepthStencilViewUtil::CreateDepthStencilView(ID3D11Texture2D* in_texture, eFormat in_format)
{
    ComPtr<ID3D11DepthStencilView> dsv;

    D3D11_TEXTURE2D_DESC texDesc;
    in_texture->GetDesc(&texDesc);

    D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
    desc.Format                        = in_format == eFormat::Unknown ? texDesc.Format : static_cast<DXGI_FORMAT>(in_format);

    if (texDesc.SampleDesc.Count > 1)
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    else
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    CheckD3D11Result(GetRenderer().GetDevice()->CreateDepthStencilView(
                         in_texture,
                         &desc,
                         dsv.GetAddressOf()),
                     "CreateDepthStencilView Fail.");

    return dsv;
}

ComPtr<ID3D11UnorderedAccessView> ID3D11UnorderedAccessViewUtil::CreateUAV(
    ID3D11Buffer* in_buffer)
{
    auto d = GetRenderer().GetDevice();

    D3D11_BUFFER_DESC bufferDesc = {};
    in_buffer->GetDesc(&bufferDesc);

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format                           = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension                    = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.FirstElement              = 0;
    uavDesc.Buffer.NumElements               = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
    uavDesc.Buffer.Flags                     = 0;

    ComPtr<ID3D11UnorderedAccessView> uav;
    CheckD3D11Result(
        d->CreateUnorderedAccessView(
            in_buffer,
            &uavDesc,
            uav.GetAddressOf()),
        "CreateUnorderedAccessView Fail.");

    return uav;
}

ComPtr<ID3D11UnorderedAccessView> ID3D11UnorderedAccessViewUtil::CreateUAVForAppendStructuredBuffer(
    ID3D11Buffer* in_buffer)
{
    auto d = GetRenderer().GetDevice();

    D3D11_BUFFER_DESC bufferDesc = {};
    in_buffer->GetDesc(&bufferDesc);

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format                           = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension                    = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.FirstElement              = 0;
    uavDesc.Buffer.NumElements               = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
    uavDesc.Buffer.Flags                     = D3D11_BUFFER_UAV_FLAG_APPEND;

    ComPtr<ID3D11UnorderedAccessView> uav;
    CheckD3D11Result(
        d->CreateUnorderedAccessView(
            in_buffer,
            &uavDesc,
            uav.GetAddressOf()),
        "CreateUnorderedAccessView Fail.");

    return uav;
}

}   // namespace crab