#pragma once

namespace crab
{

//===================================================
// Utils
//===================================================

inline uint32 GetFormatByteStride(eFormat in_format)
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

inline uint32 GetFormatByteStride(DXGI_FORMAT in_format)
{
    return static_cast<uint32>(DirectX::BitsPerPixel(in_format) / 8);
}

//===================================================
// Shader
//===================================================

class ShaderMacros
{
public:
    struct Macro
    {
        std::string name;
        std::string value;
    };

    ShaderMacros()                    = default;
    ShaderMacros(const ShaderMacros&) = default;
    ShaderMacros(ShaderMacros&&)      = default;
    ShaderMacros(const std::initializer_list<Macro>& in_macros)
        : m_macros(in_macros)
    {
    }

    void ClearList()
    {
        m_macros.clear();
    }

    ShaderMacros& Add(const std::string_view in_name, const std::string_view in_value)
    {
        m_macros.emplace_back(std::string(in_name), std::string(in_value));
        return *this;
    }

    std::vector<D3D_SHADER_MACRO> Get() const
    {
        if (m_macros.empty())
            return {};

        std::vector<D3D_SHADER_MACRO> output;
        output.reserve(m_macros.size() + 1);

        for (const auto& macro: m_macros)
            output.push_back({ macro.name.c_str(), macro.value.c_str() });

        output.push_back({ nullptr, nullptr });

        return output;
    }

private:
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
        OPTIONAL const void*  in_initData);

    static ComPtr<ID3D11Texture2D> CreateTexture2DArray(
        const std::vector<ID3D11Texture2D*>& in_textures,
        eTextureCreationFlags                in_creationFlags);

    //===================================================
    // Read & Write
    //===================================================

    static void WriteToDefaultTexture(
        ID3D11Texture2D* in_defaultTexture,
        const void*      in_writeData,
        uint32           in_writeDataByteWidth);

    static void WriteToDynamicTexture(
        ID3D11Texture2D* in_dynamicTexture,
        const void*      in_writeData,
        uint32           in_writeDataByteWidth);

    static void WriteToStagingTexture(
        ID3D11Texture2D* in_stagingTexture,
        const void*      in_writeData,
        uint32           in_writeDataByteWidth);

    static void ReadFromStagingTexture(
        ID3D11Texture2D* in_stagingTexture,
        void*            out_readData,
        OPTIONAL uint32* out_readDataByteWidth);

    static void CopyBetween(
        ID3D11Texture2D* in_srcTexture,
        ID3D11Texture2D* in_dstTexture);

    static void CopyBetween(
        ID3D11Texture2D* in_srcTexture,
        uint32           in_srcMipLevel,
        uint32           in_srcArrayIndex,
        ID3D11Texture2D* in_dstTexture,
        uint32           in_dstMipLevel,
        uint32           in_dstArrayIndex);

    static void ResolveTexture2D(
        ID3D11Texture2D* in_MSTexture,
        ID3D11Texture2D* in_dstTexture);

    //===================================================
    // Util
    //===================================================

    static bool IsSameFormat(ID3D11Texture2D* in_texture1, ID3D11Texture2D* in_texture2);
    static bool IsSameFormat(const D3D11_TEXTURE2D_DESC& in_desc1, const D3D11_TEXTURE2D_DESC& in_desc2);
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
        uint32               in_bufferByteWidth,
        D3D11_USAGE          in_usage,
        eBindFlags           in_bindFlags,
        eCPUAccessFlags      in_cpuAccessFlags,
        OPTIONAL const void* in_initData);

    //===================================================
    // Read & Write
    //===================================================

    // using Map/Unmap -> dynamic buffer
    static void WriteToDynamicBuffer(
        ID3D11Buffer* in_buffer,
        const void*   in_writeData,
        uint32        in_writeDataByteWidth);

    // using Map/Unmap -> staging buffer
    static void WriteToStagingBuffer(
        ID3D11Buffer* in_buffer,
        const void*   in_writeData,
        uint32        in_writeDataByteWidth);

    // gpu to cpu
    // using Map/Unmap -> staging buffer
    static void ReadFromBuffer(
        ID3D11Buffer*    in_stagingBuffer,
        void*            out_readData,
        OPTIONAL uint32* out_readDataByteWidth);

    // gpu to gpu
    static void CopyBetween(
        ID3D11Buffer* in_srcBuffer,
        ID3D11Buffer* in_dstBuffer);
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
    static ComPtr<ID3D11ShaderResourceView> CreateSRV(
        ID3D11Resource* in_resource,
        eFormat         in_format);
};

//===================================================
// Depth Stencil View
//===================================================

struct ID3D11DepthStencilViewUtil
{
    static ComPtr<ID3D11DepthStencilView> CreateDepthStencilView(
        ID3D11Texture2D* in_texture,
        eFormat          in_format);
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