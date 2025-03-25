#pragma once

namespace crab
{

class ShaderMacros;

//===================================================
// Shader
//===================================================

namespace d3d11
{

    uint32 GetFormatByteStride(eFormat in_format);
    uint32 GetFormatByteStride(DXGI_FORMAT in_format);

    D3D11_FILTER GetSamplerFilter(
        eSamplerMode   in_mode,
        eSamplerFilter in_filter);

    inline DXGI_FORMAT GetFormat(eFormat in_format) { return static_cast<DXGI_FORMAT>(in_format); }

    const char* ConvertHLSLFormatString(eFormat in_format);

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
            OPTIONAL const void*  in_initData);

        ComPtr<ID3D11Texture2D> CreateTexture2DArray(
            const std::vector<ID3D11Texture2D*>& in_textures,
            eTextureCreationFlags                in_creationFlags);

        void WriteToDefaultTexture(
            ID3D11Texture2D* in_defaultTexture,
            const void*      in_writeData,
            uint32           in_writeDataByteWidth);

        void WriteToDynamicTexture(
            ID3D11Texture2D* in_dynamicTexture,
            const void*      in_writeData,
            uint32           in_writeDataByteWidth);

        void WriteToStagingTexture(
            ID3D11Texture2D* in_stagingTexture,
            const void*      in_writeData,
            uint32           in_writeDataByteWidth);

        void ReadFromStagingTexture(
            ID3D11Texture2D* in_stagingTexture,
            void*            out_readData,
            OPTIONAL uint32* out_readDataByteWidth);

        void CopyBetween(
            ID3D11Texture2D* in_srcTexture,
            ID3D11Texture2D* in_dstTexture);

        void CopyBetween(
            ID3D11Texture2D* in_srcTexture,
            uint32           in_srcMipLevel,
            uint32           in_srcArrayIndex,
            ID3D11Texture2D* in_dstTexture,
            uint32           in_dstMipLevel,
            uint32           in_dstArrayIndex);

        void ResolveTexture2D(
            ID3D11Texture2D* in_MSTexture,
            ID3D11Texture2D* in_dstTexture);

        bool IsSameFormat(
            ID3D11Texture2D* in_texture1,
            ID3D11Texture2D* in_texture2);

        bool IsSameFormat(
            const D3D11_TEXTURE2D_DESC& in_desc1,
            const D3D11_TEXTURE2D_DESC& in_desc2);

    }   // namespace texture

    namespace buffer
    {
        ComPtr<ID3D11Buffer> CreateBuffer(
            uint32               in_bufferByteWidth,
            D3D11_USAGE          in_usage,
            eBindFlags           in_bindFlags,
            eCPUAccessFlags      in_cpuAccessFlags,
            OPTIONAL const void* in_initData);

        // using Map/Unmap -> dynamic buffer
        void WriteToDynamicBuffer(
            ID3D11Buffer* in_buffer,
            const void*   in_writeData,
            uint32        in_writeDataByteWidth);

        // using Map/Unmap -> staging buffer
        void WriteToStagingBuffer(
            ID3D11Buffer* in_buffer,
            const void*   in_writeData,
            uint32        in_writeDataByteWidth);

        // gpu to cpu
        // using Map/Unmap -> staging buffer
        void ReadFromBuffer(
            ID3D11Buffer*    in_stagingBuffer,
            void*            out_readData,
            OPTIONAL uint32* out_readDataByteWidth);

        // gpu to gpu
        void CopyBetween(
            ID3D11Buffer* in_srcBuffer,
            ID3D11Buffer* in_dstBuffer);
    }   // namespace buffer

    namespace shader
    {
        ComPtr<ID3DBlob> CompileShaderCode(
            std::string_view    in_shaderCode,
            std::string_view    in_entryPoint,
            std::string_view    in_shaderModel,
            const ShaderMacros& in_shaderMacros);

        ComPtr<ID3DBlob> LoadFromHLSL(
            const std::filesystem::path& in_shaderPath,
            std::string_view             in_entryPoint,
            std::string_view             in_shaderModel,
            const ShaderMacros&          in_shaderMacros);

        ComPtr<ID3DBlob> LoadFromCSO(const std::filesystem::path& in_shaderPath);
    }   // namespace shader

    namespace views
    {
        ComPtr<ID3D11ShaderResourceView> CreateSRV(
            ID3D11Resource* in_resource,
            eFormat         in_srvFormat);

        ComPtr<ID3D11DepthStencilView> CreateDSV(
            ID3D11Texture2D* in_texture,
            eFormat          in_dsvFormat);

        ComPtr<ID3D11UnorderedAccessView> CreateUAV(
            ID3D11Buffer* in_buffer);
    }   // namespace views

};   // namespace d3d11

}   // namespace crab