#pragma once

//===================================================
// Rendering Resource Factory
// Not directX11 level more high level
// this will be used to create crab renderer resources
// maybe save your time
//===================================================

namespace crab
{
class TextureShaderResource;
}

namespace crab
{
class FrameBuffer;

class VertexShader;
class ComputeShader;
class DomainShader;
class InputLayout;
class PixelShader;
class GeometryShader;
class HullShader;
class SamplerState;
class RasterizerState;
class BlendState;
class DepthStencilState;
class TextureCube;
class DepthBuffer;
struct GeometryData;
struct MaterialData;
class Model;
class Material;
class Mesh;

namespace factory
{
    //===================================================
    // Buffers
    //===================================================

    template<typename Ty>
    Ref<ConstantBuffer<Ty>> CreateConstantBuffer(const Ty& in_initData = {})
    {
        Ref<ConstantBuffer<Ty>> buffer = CreateRef<ConstantBuffer<Ty>>();
        buffer->Init(in_initData);
        return buffer;
    }

    //===================================================
    // Mesh & Material
    //===================================================

    Ref<Mesh> CreateMesh(const GeometryData& in_geometryData);

    Ref<Material> CreateMaterial(const MaterialData& in_materialData);

    Ref<Model> CreateModel(const Ref<Mesh>&     in_mesh,
                           const Ref<Material>& in_material);

    //===================================================
    // Depth Map
    //===================================================

    Ref<FrameBuffer> CreateDepthMapFrameBuffer(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        eFormat in_shaderResourceFormat,
        MSAA    in_MSAA);

    Ref<FrameBuffer> CreateDepthMapCubeFrameBuffer(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        eFormat in_shaderResourceFormat,
        MSAA    in_MSAA);

    Ref<FrameBuffer> CreateDepthMapArrayFrameBuffer(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        eFormat in_shaderResourceFormat,
        MSAA    in_MSAA,
        uint32  in_textureArraySize);

    //===================================================
    // Frame Buffer
    //===================================================

    Ref<RenderTarget> CreateRenderTarget(
        uint32     in_width,
        uint32     in_height,
        eFormat    in_format,
        eBindFlags in_bindFlags,
        MSAA       in_MSAA);

    Ref<DepthBuffer> CreateDepthBuffer(
        uint32                in_width,
        uint32                in_height,
        eFormat               in_depthBufferFormat,
        MSAA                  in_MSAA,
        uint32                in_textureArraySize,
        eTextureCreationFlags in_flags);

    Ref<DepthBuffer> CreateDepthBufferForDepthMap(
        uint32                in_width,
        uint32                in_height,
        eFormat               in_depthBufferFormat,
        eFormat               in_shaderResourceFormat,
        MSAA                  in_MSAA,
        uint32                in_textureArraySize,
        eTextureCreationFlags in_flags);

    Ref<FrameBuffer> CreateFrameBuffer(
        const RenderTargetArray& in_renderTargets,
        const Ref<DepthBuffer>&  in_depthBuffer);

    Ref<FrameBuffer> CreateFrameBufferDepthOnly(
        const Ref<DepthBuffer>& in_depthBuffer);

    //===================================================
    // Texture
    //===================================================

    Ref<TextureCube> CreateTextureCubeFromFile(
        const std::filesystem::path& in_path,
        eTextureLoadFlags            in_flags = eTextureCreationFlags_None);

    Ref<Texture2D> CreateTexture2DFromFile(
        const std::filesystem::path& in_path,
        eTextureLoadFlags            in_flags = eTextureCreationFlags_None);

    Ref<TextureShaderResource> CreateTextureShaderResource(
        ID3D11Texture2D* in_texture,
        eFormat          in_format);

    //===================================================
    // Shader
    //===================================================

    Ref<VertexShader> CreateVertexShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros,
        const Ref<InputLayout>&      in_inputLayout);

    Ref<PixelShader> CreatePixelShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros);

    Ref<GeometryShader> CreateGeometryShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros);

    Ref<HullShader> CreateHullShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros);

    Ref<DomainShader> CreateDomainShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros);

    Ref<ComputeShader> CreateComputeShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros);

    //===================================================
    // Render State
    // can be used to create for common case
    // if you want more control
    // you must call xxxState::Init()
    //===================================================

    Ref<RasterizerState> CreateRasterizerState(
        eFillMode in_fillMode,
        eCullMode in_cullMode,
        bool      in_MSAAEnable);

    Ref<BlendState> CreateBlendState(
        bool           in_alphaToCoverageEnable,
        bool           in_blendEnable,
        eBlendFactor   in_srcBlendFactor,
        eBlendFactor   in_destBlendFactor,
        eBlendOperator in_blendOperator);

    Ref<DepthStencilState> CreateDepthStencilState(
        bool             in_depthEnable,
        bool             in_depthWriteEnable,
        eComparisonFunc  in_depthFunc,
        bool             in_stencilEnable,
        eStencilOperator in_frontFaceDepthPassStencilFail,
        eStencilOperator in_frontFaceDepthFailStencilPass,
        eStencilOperator in_frontFaceDepthPassStencilPass,
        eComparisonFunc  in_frontFaceFunc);

    Ref<SamplerState> CreateSamplerState(
        eSamplerMode        in_mode,
        eSamplerFilter      in_filter,
        eSamplerAddressMode in_addressMode,
        uint32              in_maxAnisotropy,
        eComparisonFunc     in_comparisonFunc,
        const Color4&       in_borderColor);

}   // namespace factory
}   // namespace crab