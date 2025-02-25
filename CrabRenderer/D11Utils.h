#pragma once
#include "D11Renderer.h"

namespace crab
{

//===================================================
// Shader
//===================================================

class ShaderMacros
{
public:
    void ClearList()
    {
        m_macros.clear();
    }

    ShaderMacros& Add(const std::string_view in_name, const std::string_view in_value)
    {
        m_macros.emplace_back(std::string(in_name), std::string(in_value));
        return *this;
    }

    const D3D_SHADER_MACRO* Get() const
    {
        if (m_macros.empty())
            return nullptr;

        std::vector<D3D_SHADER_MACRO> output;
        output.reserve(m_macros.size() + 1);

        for (const auto& macro: m_macros)
            output.push_back({ macro.name.c_str(), macro.value.c_str() });

        output.push_back({ nullptr, nullptr });

        return output.data();
    }

private:
    struct Macro
    {
        std::string name;
        std::string value;
    };

    std::vector<Macro> m_macros;
};

//===================================================
// ID3D11Texture2D
//===================================================

struct ID3D11Texture2DUtil
{
    //===================================================
    // Factory
    //===================================================

    static ComPtr<ID3D11Texture2D> CreateTexture2D(
        uint32          in_width,
        uint32          in_height,
        eFormat         in_format,
        D3D11_USAGE     in_usage,
        eBindFlags      in_bindFlags,
        eCPUAccessFlags in_cpuAccessFlags,
        void*           in_initData,
        uint32          in_sizeOfInitDataType,
        uint32          in_MSAASampleCount   = 1,
        uint32          in_MSAASampleQuality = 0,
        uint32          in_numberOfMipmap    = 1,
        uint32          in_textureArraySize  = 1);

    static ComPtr<ID3D11Texture2D> CreateTexture2D(
        uint32          in_width,
        uint32          in_height,
        eFormat         in_format,
        D3D11_USAGE     in_usage,
        eBindFlags      in_bindFlags,
        eCPUAccessFlags in_cpuAccessFlags,
        uint32          in_MSAASampleCount   = 1,
        uint32          in_MSAASampleQuality = 0,
        uint32          in_numberOfMipmap    = 1,
        uint32          in_textureArraySize  = 1);

    static ComPtr<ID3D11Texture2D> CreateStagingTexture2D(
        uint32  in_width,
        uint32  in_height,
        eFormat in_format,
        bool    in_cpuRead,
        bool    in_cpuWrite,
        uint32  in_arraySize = 1);

    static ComPtr<ID3D11Texture2D> CreateDefaultTexture2D(
        uint32     in_width,
        uint32     in_height,
        eFormat    in_format,
        eBindFlags in_bindFlags,
        uint32     in_MSAASampleCount   = 1,
        uint32     in_MSAASampleQuality = 0,
        uint32     in_numberOfMipmap    = 1,
        uint32     in_textureArraySize  = 1);

    static ComPtr<ID3D11Texture2D> CreateDynamicTexture2D(
        uint32     in_width,
        uint32     in_height,
        eFormat    in_format,
        eBindFlags in_bindFlags,
        uint32     in_MSAASampleCount   = 1,
        uint32     in_MSAASampleQuality = 0,
        uint32     in_numberOfMipmap    = 1,
        uint32     in_textureArraySize  = 1);

    //===================================================
    // Read & Write
    //===================================================

    static void WriteToDefaultTexture(
        ID3D11Texture2D* in_defaultTexture,
        void*            in_srcData,
        uint32           in_pixelStride,
        uint32           in_width);

    static void WriteToDynamicTexture(
        ID3D11Texture2D* in_dynamicTexture,
        void*            in_srcData,
        uint32           in_pixelStride,
        uint32           in_width,
        uint32           in_height);

    static void WriteToStagingTexture(
        ID3D11Texture2D* in_stagingTexture,
        void*            in_srcData,
        uint32           in_pixelStride,
        uint32           in_width,
        uint32           in_height);

    static void ReadFromStagingTexture(
        ID3D11Texture2D* in_stagingTexture,
        void*            out_data);

    static void ReadFromStagingTexture(
        ID3D11Texture2D* in_stagingTexture,
        void*            out_data,
        uint32           in_pixelStride,
        uint32           in_offsetX,
        uint32           in_offsetY,
        uint32           in_width,
        uint32           in_height);

    static void CopyBetween(
        ID3D11Texture2D* in_srcTexture,
        ID3D11Texture2D* in_dstTexture);

    static void CopyBetween(
        ID3D11Texture2D* in_srcTexture,
        uint32           in_srcOffsetX,
        uint32           in_srcOffsetY,
        uint32           in_srcWidth,
        uint32           in_srcHeight,
        ID3D11Texture2D* in_dstTexture,
        uint32           in_dstOffsetX,
        uint32           in_dstOffsetY);

    static void Texture2DMSToTexture2D(
        ID3D11Texture2D* in_MSTexture,
        ID3D11Texture2D* in_texture,
        eFormat          in_targetFormat);
};

//===================================================
// ID3D11Buffer
//===================================================

struct ID3D11BufferUtil
{
    //===================================================
    // Factory
    //===================================================

    static ComPtr<ID3D11Buffer> CreateBuffer(
        uint32          in_bufferByteWidth,
        D3D11_USAGE     in_usage,
        eBindFlags      in_bindFlags,
        eCPUAccessFlags in_cpuAccessFlags,
        uint32          in_structureDataStride,
        const void*     in_initialData);

    static ComPtr<ID3D11Buffer> CreateConstantBuffer(uint32 in_bufferSize);

    static ComPtr<ID3D11Buffer> CreateVertexBuffer(
        uint32      in_sizePerVertex,
        uint32      in_vertexCount,
        const void* in_data);

    static ComPtr<ID3D11Buffer> CreateIndexBuffer(
        uint32      in_indexCount,
        const void* in_data);

    static ComPtr<ID3D11Buffer> CreateStructuredBuffer(
        uint32      in_itemMaxCount,
        uint32      in_itemByteStride,
        const void* in_data);

    static ComPtr<ID3D11Buffer> CreateStagingBuffer(
        uint32 in_bufferByteWidth,
        bool   in_cpuWrite,
        bool   in_cpuRead);

    //===================================================
    // Read & Write
    //===================================================

    // using Map/Unmap -> dynamic buffer
    static void WriteToDynamicBuffer(
        ID3D11Buffer* in_buffer,
        const void*   in_data,
        uint32        in_byteWidth);

    // using Map/Unmap -> staging buffer
    static void WriteToStagingBuffer(
        ID3D11Buffer* in_buffer,
        const void*   in_data,
        uint32        in_byteWidth);

    // gpu to cpu
    // using Map/Unmap -> staging buffer
    static void ReadFromBuffer(
        ID3D11Buffer* in_buffer,
        void*         out_data,
        uint32        in_byteWidth);

    // gpu to gpu
    static void CopyBetween(
        ID3D11Buffer* in_srcBuffer,
        ID3D11Buffer* in_dstBuffer,
        uint32        in_byteWidth);
};

//===================================================
// Shader
//===================================================

struct ShaderUtil
{
    static ComPtr<ID3DBlob> LoadShaderCode(
        std::string_view    in_shaderCode,
        std::string_view    in_entryPoint,
        std::string_view    in_shaderModel,
        const ShaderMacros& in_shaderMacros = {});

    static ComPtr<ID3DBlob> LoadShaderFile(
        const std::filesystem::path& in_shaderPath,
        std::string_view             in_entryPoint,
        std::string_view             in_shaderModel,
        const ShaderMacros&          in_shaderMacros = {});
};

//===================================================
// Shader Resource View
//===================================================

struct ID3D11ShaderResourceViewUtil
{
    static ComPtr<ID3D11ShaderResourceView> CreateTexture2DSRV(
        ID3D11Texture2D* in_texture,
        eFormat          in_format = eFormat::Unknown);

    static ComPtr<ID3D11ShaderResourceView> CreateBufferSRV(ID3D11Buffer* in_buffer);
};

//===================================================
// Depth Stencil View
//===================================================

struct ID3D11DepthStencilViewUtil
{
    static ComPtr<ID3D11DepthStencilView> CreateDepthStencilView(
        ID3D11Texture2D* in_texture,
        eFormat          in_format = eFormat::Unknown);
};

//===================================================
// UAV
//===================================================

struct ID3D11UnorderedAccessViewUtil
{
    static ComPtr<ID3D11UnorderedAccessView> CreateUAV(
        ID3D11Buffer* in_buffer);

    static ComPtr<ID3D11UnorderedAccessView> CreateUAVForAppendStructuredBuffer(
        ID3D11Buffer* in_buffer);
};

}   // namespace crab