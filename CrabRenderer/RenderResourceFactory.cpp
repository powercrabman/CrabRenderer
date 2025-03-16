#include "CrabPch.h"

#include "RenderResourceFactory.h"

#include "CrabVertex.h"
#include "DepthMap.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "PipelineState.h"
#include "Textures.h"

namespace crab
{

Ref<PipelineState> CreatePipelineState(
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

Ref<Texture2D> CreateTexture2DFromFile(const std::filesystem::path& in_filePath, bool in_generateMips, bool in_reverseToneMapping)
{
    auto tex = CreateRef<Texture2D>();
    tex->LoadFromFile(in_filePath, in_generateMips, in_reverseToneMapping);
    return tex;
}

Ref<DepthMapTexture> CreateDepthMapTexture(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthFormat,
    eFormat in_textureFormat)
{
    Ref<DepthMapTexture> depthMap = CreateRef<DepthMapTexture>();
    depthMap->Init(in_width, in_height, in_depthFormat, in_textureFormat);
    return depthMap;
}

Ref<DepthMapCube> CreateDepthMapCube(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthFormat,
    eFormat in_textureFormat)
{
    Ref<DepthMapCube> depthMap = CreateRef<DepthMapCube>();
    depthMap->Init(in_width, in_height, in_depthFormat, in_textureFormat);
    return depthMap;
}

Ref<DepthMapArray> CreateDepthMapArray(
    uint32  in_width,
    uint32  in_height,
    uint32  in_arraySize,
    eFormat in_depthFormat,
    eFormat in_textureFormat)
{
    Ref<DepthMapArray> depthMap = CreateRef<DepthMapArray>();
    depthMap->Init(in_width, in_height, in_arraySize, in_depthFormat, in_textureFormat);
    return depthMap;
}

Ref<Mesh> CreateMesh(const GeometryData& in_geometryData)
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

Ref<Material> CreateMaterial(const MaterialData& in_materialData)
{
    auto material = CreateRef<Material>();
    material->Init(in_materialData);
    return material;
}

Ref<Model> CreateModel(
    const Ref<Mesh>&     in_mesh,
    const Ref<Material>& in_material)
{
    Ref<Model> model = CreateRef<Model>();
    model->Init(in_mesh, in_material);
    return model;
}

Ref<TextureCube> CreateTextureCubeFromFile(const std::filesystem::path& in_filePath, bool in_generateMips)
{
    auto tex = CreateRef<TextureCube>();
    tex->LoadFromFile(in_filePath, in_generateMips);
    return tex;
}

}   // namespace crab