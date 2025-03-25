#include "InternalPCH.h"

#include "RenderFactory.h"

#include "CrabVertex.h"
#include "DepthBuffer.h"
#include "FrameBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "RenderStates.h"
#include "RenderTarget.h"
#include "RendererConfig.h"
#include "ShaderResource.h"
#include "Shaders.h"

namespace crab
{
namespace factory
{

    Ref<FrameBuffer> CreateDepthMapFrameBuffer(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        eFormat in_shaderResourceFormat,
        MSAA    in_MSAA)
    {
        Ref<FrameBuffer> output = CreateRef<FrameBuffer>();
        output->InitDepthOnly(
            CreateDepthBufferForDepthMap(
                in_width,
                in_height,
                in_depthBufferFormat,
                in_shaderResourceFormat,
                in_MSAA,
                1,
                eTextureCreationFlags_None));

        return output;
    }

    Ref<FrameBuffer> CreateDepthMapCubeFrameBuffer(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        eFormat in_shaderResourceFormat,
        MSAA    in_MSAA)
    {
        Ref<FrameBuffer> output = CreateRef<FrameBuffer>();
        output->InitDepthOnly(
            CreateDepthBufferForDepthMap(
                in_width,
                in_height,
                in_depthBufferFormat,
                in_shaderResourceFormat,
                in_MSAA,
                6,
                eTextureCreationFlags_CubeMap));

        return output;
    }

    Ref<FrameBuffer> CreateDepthMapArrayFrameBuffer(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        eFormat in_shaderResourceFormat,
        MSAA    in_MSAA,
        uint32  in_textureArraySize)
    {
        Ref<FrameBuffer> output = CreateRef<FrameBuffer>();
        output->InitDepthOnly(
            CreateDepthBufferForDepthMap(
                in_width,
                in_height,
                in_depthBufferFormat,
                in_shaderResourceFormat,
                in_MSAA,
                in_textureArraySize,
                eTextureCreationFlags_None));

        return output;
    }

    Ref<RenderTarget> CreateRenderTarget(
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

    Ref<DepthBuffer> CreateDepthBuffer(uint32                in_width,
                                       uint32                in_height,
                                       eFormat               in_depthBufferFormat,
                                       MSAA                  in_MSAA,
                                       uint32                in_textureArraySize,
                                       eTextureCreationFlags in_flags)
    {
        Ref<DepthBuffer> depthBuffer = CreateRef<DepthBuffer>();
        depthBuffer->Init(in_width,
                          in_height,
                          in_depthBufferFormat,
                          in_MSAA,
                          in_textureArraySize,
                          in_flags);
        return depthBuffer;
    }

    Ref<DepthBuffer> CreateDepthBufferForDepthMap(uint32                in_width,
                                                  uint32                in_height,
                                                  eFormat               in_depthBufferFormat,
                                                  eFormat               in_shaderResourceFormat,
                                                  MSAA                  in_MSAA,
                                                  uint32                in_textureArraySize,
                                                  eTextureCreationFlags in_flags)
    {
        Ref<DepthBuffer> depthBuffer = CreateRef<DepthBuffer>();
        depthBuffer->InitForDepthMap(in_width,
                                     in_height,
                                     in_depthBufferFormat,
                                     in_shaderResourceFormat,
                                     in_MSAA,
                                     in_textureArraySize,
                                     in_flags);
        return depthBuffer;
    }

    Ref<FrameBuffer> CreateFrameBuffer(
        const RenderTargetArray& in_renderTargets,
        const Ref<DepthBuffer>&  in_depthBuffer)
    {
        Ref<FrameBuffer> output = CreateRef<FrameBuffer>();
        output->Init(in_renderTargets, in_depthBuffer);
        return output;
    }

    Ref<FrameBuffer> CreateFrameBufferDepthOnly(
        const Ref<DepthBuffer>& in_depthBuffer)
    {
        Ref<FrameBuffer> output = CreateRef<FrameBuffer>();
        output->InitDepthOnly(in_depthBuffer);
        return output;
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

    Ref<TextureCube> CreateTextureCubeFromFile(
        const std::filesystem::path& in_path,
        eTextureLoadFlags            in_flags)
    {
        Ref<TextureCube> texture = CreateRef<TextureCube>();
        texture->LoadFromFile(in_path, in_flags);
        return texture;
    }

    Ref<Texture2D> CreateTexture2DFromFile(
        const std::filesystem::path& in_path,
        eTextureLoadFlags            in_flags)
    {
        Ref<Texture2D> texture = CreateRef<Texture2D>();
        texture->LoadFromFile(in_path, in_flags);
        return texture;
    }

    Ref<TextureShaderResource> CreateTextureShaderResource(
        ID3D11Texture2D* in_texture,
        eFormat          in_format)
    {
        D3D11_TEXTURE2D_DESC desc;
        in_texture->GetDesc(&desc);

        if (desc.ArraySize > 1)
        {
            if (desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE)
            {
                Ref<TextureCube> texCube = CreateRef<TextureCube>();
                texCube->Init(in_texture, in_format);
                return texCube;
            }
            else
            {
                Ref<Texture2DArray> texArr = CreateRef<Texture2DArray>();
                texArr->Init(in_texture, in_format);
                return texArr;
            }
        }
        else
        {
            Ref<Texture2D> tex = CreateRef<Texture2D>();
            tex->Init(in_texture, in_format);
            return tex;
        }
    }

    Ref<VertexShader> CreateVertexShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros,
        const Ref<InputLayout>&      in_inputLayout)
    {
        Ref<VertexShader> shader = CreateRef<VertexShader>();
        shader->LoadFromHLSL(in_path, in_entryPoint, in_macros, in_inputLayout);
        return shader;
    }

    Ref<PixelShader> CreatePixelShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros)
    {
        Ref<PixelShader> shader = CreateRef<PixelShader>();
        shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
        return shader;
    }

    Ref<GeometryShader> CreateGeometryShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros)
    {
        Ref<GeometryShader> shader = CreateRef<GeometryShader>();
        shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
        return shader;
    }

    Ref<HullShader> CreateHullShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros)
    {
        Ref<HullShader> shader = CreateRef<HullShader>();
        shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
        return shader;
    }

    Ref<DomainShader> CreateDomainShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros)
    {
        Ref<DomainShader> shader = CreateRef<DomainShader>();
        shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
        return shader;
    }

    Ref<ComputeShader> CreateComputeShaderFromHLSL(
        const std::filesystem::path& in_path,
        std::string_view             in_entryPoint,
        const ShaderMacros&          in_macros)
    {
        Ref<ComputeShader> shader = CreateRef<ComputeShader>();
        shader->LoadFromHLSL(in_path, in_entryPoint, in_macros);
        return shader;
    }

    Ref<RasterizerState> CreateRasterizerState(
        eFillMode in_fillMode,
        eCullMode in_cullMode,
        bool      in_MSAAEnable)
    {
        Ref<RasterizerState> rasterizerState = CreateRef<RasterizerState>();
        rasterizerState->Init(
            in_fillMode,
            in_cullMode,
            0,
            0.f,
            0.f,
            true,
            false,
            in_MSAAEnable);

        return rasterizerState;
    }

    Ref<BlendState> CreateBlendState(
        bool           in_alphaToCoverageEnable,
        bool           in_blendEnable,
        eBlendFactor   in_srcBlendFactor,
        eBlendFactor   in_destBlendFactor,
        eBlendOperator in_blendOperator)
    {
        Ref<BlendState> blendState = CreateRef<BlendState>();
        blendState->Init(
            in_alphaToCoverageEnable,
            in_blendEnable,
            in_srcBlendFactor,
            in_destBlendFactor,
            in_blendOperator,
            eBlendFactor::One,
            eBlendFactor::Zero,
            eBlendOperator::Add,
            eBlendWriteFlags_All);

        return blendState;
    }

    Ref<DepthStencilState> CreateDepthStencilState(
        bool             in_depthEnable,
        bool             in_depthWriteEnable,
        eComparisonFunc  in_depthFunc,
        bool             in_stencilEnable,
        eStencilOperator in_frontFaceDepthPassStencilFail,
        eStencilOperator in_frontFaceDepthFailStencilPass,
        eStencilOperator in_frontFaceDepthPassStencilPass,
        eComparisonFunc  in_frontFaceFunc)
    {
        Ref<DepthStencilState> depthStencilState = CreateRef<DepthStencilState>();
        depthStencilState->Init(
            in_depthEnable,
            in_depthWriteEnable,
            in_depthFunc,
            in_stencilEnable,
            0b1111'1111,
            0b1111'1111,
            in_frontFaceFunc,
            in_frontFaceDepthPassStencilFail,
            in_frontFaceDepthFailStencilPass,
            in_frontFaceDepthPassStencilPass,
            in_frontFaceFunc,
            in_frontFaceDepthPassStencilFail,
            in_frontFaceDepthFailStencilPass,
            in_frontFaceDepthPassStencilPass);

        return depthStencilState;
    }

    Ref<SamplerState> CreateSamplerState(
        eSamplerMode        in_mode,
        eSamplerFilter      in_filter,
        eSamplerAddressMode in_addressMode,
        uint32              in_maxAnisotropy,
        eComparisonFunc     in_comparisonFunc,
        const Color4&       in_borderColor)
    {
        Ref<SamplerState> samplerState = CreateRef<SamplerState>();
        samplerState->Init(
            in_mode,
            in_filter,
            in_addressMode,
            in_addressMode,
            in_addressMode,
            0.f,
            in_maxAnisotropy,
            in_comparisonFunc,
            in_borderColor,
            -FLT_MAX,
            FLT_MAX);

        return samplerState;
    }
}   // namespace factory
}   // namespace crab