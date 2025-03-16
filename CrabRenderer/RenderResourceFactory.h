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
    const ConstantList&           in_buffers);

template<typename Ty>
Ref<ConstantBuffer<Ty>> CreateConstantBuffer(const Ty& in_initData)
{
    Ref<ConstantBuffer<Ty>> buffer = CreateRef<ConstantBuffer<Ty>>();
    buffer->Init(in_initData);
    return buffer;
}

template<typename Ty>
Ref<ConstantBuffer<Ty>> CreateConstantBuffer()
{
    Ref<ConstantBuffer<Ty>> buffer = CreateRef<ConstantBuffer<Ty>>();
    buffer->Init();
    return buffer;
}

Ref<Mesh>     CreateMesh(const GeometryData& in_geometryData);
Ref<Material> CreateMaterial(const MaterialData& in_materialData);
Ref<Model>    CreateModel(const Ref<Mesh>& in_mesh, const Ref<Material>& in_material);

Ref<TextureCube> CreateTextureCubeFromFile(
    const std::filesystem::path& in_filePath,
    bool                         in_generateMips = true);

Ref<Texture2D> CreateTexture2DFromFile(
    const std::filesystem::path& in_filePath,
    bool                         in_generateMips       = true,
    bool                         in_reverseToneMapping = false);

Ref<DepthMapTexture> CreateDepthMapTexture(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthFormat   = eFormat::Depth_Float32,
    eFormat in_textureFormat = eFormat::Float32);

Ref<DepthMapCube> CreateDepthMapCube(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthFormat   = eFormat::Depth_Float32,
    eFormat in_textureFormat = eFormat::Float32);

Ref<DepthMapArray> CreateDepthMapArray(
    uint32  in_width,
    uint32  in_height,
    uint32  in_arraySize,
    eFormat in_depthFormat   = eFormat::Depth_Float32,
    eFormat in_textureFormat = eFormat::Float32);

}   // namespace crab