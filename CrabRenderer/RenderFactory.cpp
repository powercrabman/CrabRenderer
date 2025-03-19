#include "CrabPch.h"

#include "RenderFactory.h"

#include "AppWindow.h"
#include "CrabVertex.h"
#include "DepthBuffer.h"
#include "DepthMap.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "PipelineState.h"
#include "RenderTarget.h"

namespace crab
{

Ref<PipelineState> RenderFactory::CreatePipelineState(
    const Ref<VertexShader>&      in_vertexShader,
    const Ref<PixelShader>&       in_pixelShader,
    const Ref<GeometryShader>&    in_geometryShader,
    const Ref<HullShader>&        in_hullShader,
    const Ref<DomainShader>&      in_domainShader,
    const Ref<DepthStencilState>& in_depthStencilState,
    const Ref<RasterizerState>&   in_rasterizerState,
    const Ref<BlendState>&        in_blendState,
    const SamplerList&            in_samplers,
    const ConstantList&           in_buffers)
{
    Ref<PipelineState> result = CreateRef<PipelineState>();
    result->Init(
        in_vertexShader,
        in_pixelShader,
        in_geometryShader,
        in_hullShader,
        in_domainShader,
        in_depthStencilState,
        in_rasterizerState,
        in_blendState,
        in_samplers,
        in_buffers);
    return result;
}

Ref<DepthMapTexture> RenderFactory::CreateDepthMapTexture(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthFormat,
    eFormat in_textureFormat)
{
    Ref<DepthMapTexture> depthMap = CreateRef<DepthMapTexture>();
    depthMap->Init(in_width, in_height, in_depthFormat, in_textureFormat);
    return depthMap;
}

Ref<DepthMapCube> RenderFactory::CreateDepthMapCube(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthFormat,
    eFormat in_textureFormat)
{
    Ref<DepthMapCube> depthMap = CreateRef<DepthMapCube>();
    depthMap->Init(in_width, in_height, in_depthFormat, in_textureFormat);
    return depthMap;
}

Ref<DepthMapArray> RenderFactory::CreateDepthMapArray(
    uint32  in_width,
    uint32  in_height,
    uint32  in_arraySize,
    eFormat in_depthFormat,
    eFormat in_textureFormat)
{
    Ref<DepthMapArray> depthMap = CreateRef<DepthMapArray>();
    depthMap->Init(in_width,
                   in_height,
                   in_arraySize,
                   in_depthFormat,
                   in_textureFormat);
    return depthMap;
}

Ref<RenderTarget> RenderFactory::CreateRenderTarget(
    uint32     in_width,
    uint32     in_height,
    eFormat    in_format,
    eBindFlags in_bindFlags,
    MSAA       in_MSAA)
{
    Ref<RenderTarget> rt = CreateRef<RenderTarget>();
    rt->Init(in_width,
             in_height,
             in_format,
             in_bindFlags,
             in_MSAA);

    return rt;
}

Ref<Mesh> RenderFactory::CreateMesh(const GeometryData& in_geometryData)
{
    std::vector<Vertex3D> vertices = {};
    vertices.reserve(in_geometryData.subDatas.size());

    for (size_t i = 0; i < in_geometryData.subDatas.size(); ++i)
    {
        const VertexData& vertexData = in_geometryData.subDatas[i];

        Vertex3D vertex = {};
        vertex.position = vertexData.position;
        vertex.normal   = vertexData.normal;
        vertex.texCoord = vertexData.texCoord;
        vertex.tangent  = vertexData.tangent;

        vertices.push_back(vertex);
    }

    Ref<Mesh> mesh = CreateRef<Mesh>();
    mesh->Init(
        vertices,
        in_geometryData.indices,
        in_geometryData.topology);

    return mesh;
}

Ref<Material> RenderFactory::CreateMaterial(const MaterialData& in_materialData)
{
    auto material = CreateRef<Material>();
    material->Init(in_materialData);
    return material;
}

Ref<Model> RenderFactory::CreateModel(
    const Ref<Mesh>&     in_mesh,
    const Ref<Material>& in_material)
{
    Ref<Model> model = CreateRef<Model>();
    model->Init(in_mesh, in_material);
    return model;
}

Ref<DepthBuffer> RenderFactory::CreateDepthBuffer(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthBufferFormat,
    MSAA    in_MSAA)
{
    Ref<DepthBuffer> depthBuffer = CreateRef<DepthBuffer>();
    depthBuffer->Init(in_width, in_height, in_depthBufferFormat, in_MSAA);
    return depthBuffer;
}

Ref<TextureCube> RenderFactory::CreateTextureCubeFromFile(
    const std::filesystem::path& in_path,
    eTextureLoadFlags            in_flags)
{
    Ref<TextureCube> texture = CreateRef<TextureCube>();
    texture->LoadFromFile(in_path, in_flags);
    return texture;
}

Ref<Texture2D> RenderFactory::CreateTexture2DFromFile(
    const std::filesystem::path& in_path,
    eTextureLoadFlags            in_flags)
{
    Ref<Texture2D> texture = CreateRef<Texture2D>();
    texture->LoadFromFile(in_path, in_flags);
    return texture;
}

Viewport RenderFactory::CreateFullScreenViewport()
{
    auto [width, height] = GetAppWindow().GetResolution();
    return Viewport(0.f, 0.f, static_cast<float>(width), static_cast<float>(height));
}

Ref<VertexShader> RenderFactory::CreateVertexShaderFromHLSL(
    const std::filesystem::path& in_path,
    std::string_view             in_entryPoint,
    const InputElements&         in_inputElements,
    const ShaderMacros&          in_macros)
{
    Ref<VertexShader> shader = CreateRef<VertexShader>();
    shader->LoadFromHLSL(in_path, in_entryPoint, in_inputElements, in_macros);
    return shader;
}

Ref<PixelShader> RenderFactory::CreatePixelShaderFromHLSL(
    const std::filesystem::path& in_path,
    std::string_view             in_entryPoint,
    const ShaderMacros&          in_macros)
{
    Ref<PixelShader> shader = CreateRef<PixelShader>();
    shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
    return shader;
}

Ref<GeometryShader> RenderFactory::CreateGeometryShaderFromHLSL(
    const std::filesystem::path& in_path,
    std::string_view             in_entryPoint,
    const ShaderMacros&          in_macros)
{
    Ref<GeometryShader> shader = CreateRef<GeometryShader>();
    shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
    return shader;
}

Ref<HullShader> RenderFactory::CreateHullShaderFromHLSL(
    const std::filesystem::path& in_path,
    std::string_view             in_entryPoint,
    const ShaderMacros&          in_macros)
{
    Ref<HullShader> shader = CreateRef<HullShader>();
    shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
    return shader;
}

Ref<DomainShader> RenderFactory::CreateDomainShaderFromHLSL(
    const std::filesystem::path& in_path,
    std::string_view             in_entryPoint,
    const ShaderMacros&          in_macros)
{
    Ref<DomainShader> shader = CreateRef<DomainShader>();
    shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
    return shader;
}

Ref<ComputeShader> RenderFactory::CreateComputeShaderFromHLSL(
    const std::filesystem::path& in_path,
    std::string_view             in_entryPoint,
    const ShaderMacros&          in_macros)
{
    Ref<ComputeShader> shader = CreateRef<ComputeShader>();
    shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
    return shader;
}

}   // namespace crab