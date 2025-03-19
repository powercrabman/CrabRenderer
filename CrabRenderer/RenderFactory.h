#pragma once
#include "Buffers.h"
#include "D11ResourceList.h"
#include "Shaders.h"

//===================================================
// Rendering Resource Factory
// Not directX11 level more high level
// this will be used to create crab renderer resources
// maybe save your time
//===================================================

namespace crab
{

class Texture2DArray;
class BlendState;
class RasterizerState;
class DepthStencilState;
class DepthBuffer;
class RenderTarget;
class DepthMapArray;
class DepthMapCube;
class DepthMapTexture;
class Model;
struct MaterialData;
struct ModelNode;
class Material;
class TextureCube;
struct GeometryData;
class Mesh;
class PipelineState;
class DomainShader;
class HullShader;
class GeometryShader;
class SamplerList;

class RenderFactory
{
public:
    //===================================================
    // Pipeline State
    //===================================================

    static Ref<PipelineState> CreatePipelineState(
        const Ref<VertexShader>&      in_vertexShader,
        const Ref<PixelShader>&       in_pixelShader,
        const Ref<GeometryShader>&    in_geometryShader,
        const Ref<HullShader>&        in_hullShader,
        const Ref<DomainShader>&      in_domainShader,
        const Ref<DepthStencilState>& in_depthStencilState,
        const Ref<RasterizerState>&   in_rasterizerState,
        const Ref<BlendState>&        in_blendState,
        const SamplerList&            in_samplers,
        const ConstantList&           in_buffers);

    //===================================================
    // Buffers
    //===================================================

    template<typename Ty>
    static Ref<ConstantBuffer<Ty>> CreateConstantBuffer(const Ty& in_initData)
    {
        static Ref<ConstantBuffer<Ty>> buffer = CreateRef<ConstantBuffer<Ty>>();
        buffer->Init(in_initData);
        return buffer;
    }

    template<typename Ty>
    static Ref<ConstantBuffer<Ty>> CreateConstantBuffer()
    {
        static Ref<ConstantBuffer<Ty>> buffer = CreateRef<ConstantBuffer<Ty>>();
        buffer->Init();
        return buffer;
    }

    //===================================================
    // Mesh & Material
    //===================================================

    static Ref<Mesh>     CreateMesh(const GeometryData& in_geometryData);
    static Ref<Material> CreateMaterial(const MaterialData& in_materialData);
    static Ref<Model>    CreateModel(
           const Ref<Mesh>&     in_mesh,
           const Ref<Material>& in_material);

    //===================================================
    // Depth Map
    //===================================================

    static Ref<DepthMapTexture> CreateDepthMapTexture(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthFormat,
        eFormat in_textureFormat);

    static Ref<DepthMapCube> CreateDepthMapCube(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthFormat,
        eFormat in_textureFormat);

    static Ref<DepthMapArray> CreateDepthMapArray(
        uint32  in_width,
        uint32  in_height,
        uint32  in_arraySize,
        eFormat in_depthFormat,
        eFormat in_textureFormat);

    //===================================================
    // Frame Buffer
    //===================================================

    static Ref<RenderTarget> CreateRenderTarget(
        uint32     in_width,
        uint32     in_height,
        eFormat    in_format,
        eBindFlags in_bindFlags,
        MSAA       in_MSAA);

    static Ref<DepthBuffer> CreateDepthBuffer(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        MSAA    in_MSAA);

    //===================================================
    // Texture
    //===================================================

    static Ref<TextureCube> CreateTextureCubeFromFile(
        const std::filesystem::path& in_path,
        eTextureLoadFlags            in_flags = eTextureCreationFlags_None);

    static Ref<Texture2D> CreateTexture2DFromFile(
        const std::filesystem::path& in_path,
        eTextureLoadFlags            in_flags = eTextureCreationFlags_None);

    static Viewport CreateFullScreenViewport();

    //===================================================
    // Shader
    //===================================================

    static Ref<VertexShader> CreateVertexShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const InputElements&         in_inputElements,
        const ShaderMacros&          in_macros = {});

    static Ref<PixelShader> CreatePixelShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros = {});

    static Ref<GeometryShader> CreateGeometryShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros = {});

    static Ref<HullShader> CreateHullShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros = {});

    static Ref<DomainShader> CreateDomainShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros = {});

    static Ref<ComputeShader> CreateComputeShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros = {});
};

}   // namespace crab