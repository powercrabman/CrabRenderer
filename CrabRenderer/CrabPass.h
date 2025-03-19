#pragma once
#include "DepthBuffer.h"
#include "DepthMap.h"
#include "PassCommon.h"
#include "RenderFactory.h"

namespace crab
{

class CrabPass
{
public:
    template<StringLiteral Tag>
    static void PBRPass(
        Scene*                      in_scene,
        const PipelineBindArgument& in_args         = {},
        const Ref<PipelineState>&   in_userPipeline = nullptr)
    {
        using RenderGroup = RenderGroup<Tag>;

        if (in_userPipeline)
            in_userPipeline->Bind(in_args);
        else
            GetGlobalPipeline()->BeginPBR(in_args);

        in_scene->GetView<TransformComponent, ModelRenderer, RenderGroup>().each(
            [](const TransformComponent& t, const ModelRenderer& m)
            {
                GetGlobalConstants()->UpdateTransform(
                    TransformConstant {
                        .world             = t.GetWorld(),
                        .worldInvTranspose = t.GetWorld().Invert().Transpose() });

                // Material
                for (const auto& [mesh, material, _]: m.model->GetNodes())
                {
                    MaterialConstant mat;
                    mat.baseColor = material->GetBaseColor();
                    mat.metallic  = material->GetMetallic();
                    mat.emissive  = material->GetEmissive();
                    mat.roughness = material->GetRoughness();
                    mat.alpha     = material->GetAlpha();

                    mat.usingTextureFlags = eMaterialTextureUsingFlags_None;

                    if (const auto& tex = material->GetBaseColorTexture())
                    {
                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Albedo;
                        tex->Bind(0, eShaderFlags_PixelShader);
                    }

                    if (const auto& tex = material->GetNormalTexture())
                    {
                        if (material->GetNormalMapType() == eNormalMapType::OpenGL)
                            mat.usingTextureFlags |= eMaterialTextureUsingFlags_Normal_GL;
                        else
                            mat.usingTextureFlags |= eMaterialTextureUsingFlags_Normal_DX;

                        tex->Bind(1, eShaderFlags_PixelShader);
                    }

                    if (const auto& tex = material->GetAOTexture())
                    {
                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_AO;
                        tex->Bind(2, eShaderFlags_PixelShader);
                    }

                    if (const auto& tex = material->GetMetallicTexture())
                    {
                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Metallic;
                        tex->Bind(3, eShaderFlags_PixelShader);
                    }

                    if (const auto& tex = material->GetRoughnessTexture())
                    {
                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Roughness;
                        tex->Bind(4, eShaderFlags_PixelShader);
                    }

                    if (const auto& tex = material->GetEmissiveTexture())
                    {
                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Emissive;
                        tex->Bind(5, eShaderFlags_PixelShader);
                    }

                    if (const auto& tex = material->GetDisplacementTexture())
                    {
                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Displacement;
                        tex->Bind(0, eShaderFlags_VertexShader);
                    }

                    GetGlobalConstants()->UpdateMaterial(mat);

                    mesh->Draw();
                }
            });
    }

    template<StringLiteral Tag>
    static void DrawNormalPass(Scene* in_scene)
    {
        using RenderGroup = RenderGroup<Tag>;

        GetGlobalPipeline()->BeginDrawNormal();

        in_scene->GetView<TransformComponent, ModelRenderer, RenderGroup>().each(
            [](const TransformComponent& t, const ModelRenderer& m)
            {
                GetGlobalConstants()->UpdateTransform(
                    TransformConstant {
                        .world             = t.GetWorld(),
                        .worldInvTranspose = t.GetWorld().Invert().Transpose() });

                for (const auto& [mesh, _, __]: m.model->GetNodes())
                    mesh->Draw();
            });
    }

    template<StringLiteral Tag>
    static void SkyboxPass(
        Scene*                      in_scene,
        const PipelineBindArgument& in_args         = {},
        const Ref<PipelineState>&   in_userPipeline = nullptr)
    {
        using RenderGroup = RenderGroup<Tag>;

        if (in_userPipeline)
            in_userPipeline->Bind(in_args);
        else
            GetGlobalPipeline()->BeginSkybox(in_args);

        in_scene->GetView<TransformComponent, SkyboxRenderer, RenderGroup>().each(
            [](const TransformComponent& t, const SkyboxRenderer& s)
            {
                GetGlobalConstants()->UpdateTransform(
                    TransformConstant {
                        .world             = t.GetWorld(),
                        .worldInvTranspose = t.GetWorld().Invert().Transpose() });

                if (s.envCubemap)
                    s.envCubemap->Bind(10, eShaderFlags_PixelShader);

                if (s.irrCubemap)
                    s.irrCubemap->Bind(11, eShaderFlags_PixelShader);

                if (s.specCubemap)
                    s.specCubemap->Bind(12, eShaderFlags_PixelShader);

                if (s.brdfImage)
                    s.brdfImage->Bind(13, eShaderFlags_PixelShader);

                s.mesh->Draw();
            });
    }

    template<StringLiteral Tag>
    static void MaskPass(
        Scene*                      in_scene,
        const PipelineBindArgument& in_args,
        const Ref<PipelineState>&   in_userPipeline = nullptr)
    {
        using RenderGroup = RenderGroup<Tag>;

        if (in_userPipeline)
            in_userPipeline->Bind(in_args);
        else
            GetGlobalPipeline()->BeginMask(in_args);

        in_scene->GetView<TransformComponent, ModelRenderer, RenderGroup>().each(
            [](const TransformComponent& t, const ModelRenderer& m)
            {
                GetGlobalConstants()->UpdateTransform(
                    TransformConstant {
                        .world             = t.GetWorld(),
                        .worldInvTranspose = t.GetWorld().Invert().Transpose() });

                for (const auto& [mesh, _, __]: m.model->GetNodes())
                    mesh->Draw();
            });
    }

    template<StringLiteral MirrorTag>
    static void MirrorPass(Scene*                             in_scene,
                           const Ref<DepthBuffer>&            in_depthBuffer,
                           Entity                             in_cameraEntity,
                           uint32                             in_mirrorMaskRef,
                           const std::function<void(Scene*)>& in_reflectedObjectDrawCall)
    {
        using RenderGroup = RenderGroup<MirrorTag>;

        CrabPass::MaskPass<MirrorTag>(in_scene, PipelineBindArgument { in_mirrorMaskRef });
        in_depthBuffer->Clear(true, 1.f, false, 0);

        in_scene->GetView<TransformComponent, RenderGroup>().each(
            [&](const TransformComponent& t)
            {
                auto& cmrT   = in_cameraEntity.GetTransform();
                auto& cmrCmr = in_cameraEntity.GetComponent<CameraComponent>();

                Mat4 reflectMatrix = Mat4::CreateReflection(Plane { t.position, t.Forward() });
                Mat4 viewProj      = reflectMatrix * cmrCmr.GetViewProj(cmrT);

                GetGlobalConstants()->UpdateReflectCamera(CameraConstant {
                    .view           = Mat4::Identity,
                    .viewProj       = viewProj,
                    .invViewProj    = Mat4::Identity,
                    .cameraPosition = t.position });

                in_reflectedObjectDrawCall(in_scene);
            });
    }

    template<StringLiteral... ShadowCastObjectTags>
    static void LightShadowPass(
        Scene* in_scene,
        Entity in_cameraEntity)
    {
        auto&                  r = GetRenderer();
        LightConstant          lightConst;
        ShadowReceiverConstant shadowConst;
        uint32                 lightIndex = 0;

        in_scene->GetView<LightComponent, TransformComponent>().each(
            [&](LightComponent& l, const TransformComponent& t)
            {
                if (lightIndex >= MAX_LIGHTS)
                    return;

                lightConst.light[lightIndex] = LightConstantData {
                    .lightPosition  = t.position,
                    .lightType      = l.lightType,
                    .lightDirection = t.Forward(),
                    .useShadow      = l.useShadow,
                    .lightRadiance  = l.lightRadiance,
                    .fallOffStart   = l.fallOffStart,
                    .fallOffEnd     = l.fallOffEnd,
                    .lightStrength  = l.lightStrength,
                    .innerConeAngle = l.innerConeAngle,
                    .outerConeAngle = l.outerConeAngle,
                };

                if (l.useShadow)
                {
                    Ref<DepthMap>& shadowMap = l.shadowMap;

                    switch (l.lightType)
                    {
                        case eLightType::Directional:
                        {
                            if (!shadowMap || shadowMap->GetTextureType() != eResourceType::Texture2DArray)
                                shadowMap = CreateDepthMapArray(SHADOW_MAP_LENGTH,
                                                                SHADOW_MAP_LENGTH,
                                                                MAX_CASCADE_SHADOW_LEVEL,
                                                                eFormat::Depth_Float32,
                                                                eFormat::Float32_1);

                            // update constant buffer
                            CameraComponent& cmr = in_cameraEntity.GetComponent<CameraComponent>();

                            // Camera frustum in world space
                            Frustum viewFrustum = cmr.GetViewFrustumInWorld(in_cameraEntity.GetTransform());

                            // Cascade frustums in world space
                            std::vector<float> cascadeRanges;
                            cascadeRanges.reserve(MAX_CASCADE_SHADOW_LEVEL + 1);
                            cascadeRanges.push_back(cmr.nearZ);
                            cascadeRanges.insert(cascadeRanges.end(), l.cascadeRange.begin(), l.cascadeRange.end());
                            cascadeRanges.push_back(cmr.farZ);

                            std::vector<Mat4> shadowViewProj = MatrixUtil::CreateOrthographicForCSM(viewFrustum,
                                                                                                    cascadeRanges,
                                                                                                    t.Forward(),
                                                                                                    50.f);

                            CascadeShadowCasterConstant cascadeShadowCasterConst;
                            for (size_t i = 0; i < MAX_CASCADE_SHADOW_LEVEL; ++i)
                            {
                                cascadeShadowCasterConst.shadowViewProj[i] = shadowViewProj[i];
                                shadowConst.cascadeShadowViewProj[i]       = shadowViewProj[i];
                            }

                            GetGlobalConstants()->UpdateCascadeShadowCaster(cascadeShadowCasterConst);

                            // update cascade constant
                            shadowConst.cascadeRange1 = l.cascadeRange[0];
                            shadowConst.cascadeRange2 = l.cascadeRange[1];
                            shadowConst.cascadeRange3 = l.cascadeRange[2];

                            // bind shadow map
                            r.SetShaderResourceView(nullptr, CASCADE_SHADOW_SLOT + lightIndex, eShaderFlags_PixelShader);
                            shadowMap->BindDepthBuffer();
                            shadowMap->Clear(1.f);
                            r.SetViewport(0, 0, shadowMap->GetResolution().x, shadowMap->GetResolution().y);

                            // cast object
                            (MaskPass<ShadowCastObjectTags>(
                                 in_scene, {}, GetGlobalPipeline()->GetCascadeShadowCaster()),
                             ...);

                            // bind resource
                            r.SetRenderTarget(nullptr, nullptr);
                            shadowMap->BindDepthMapTexture(CASCADE_SHADOW_SLOT + lightIndex, eShaderFlags_PixelShader);
                        }
                        break;

                        case eLightType::Point:
                        {
                            if (!shadowMap || shadowMap->GetTextureType() != eResourceType::TextureCube)
                                shadowMap = CreateDepthMapCube(SHADOW_MAP_LENGTH,
                                                               SHADOW_MAP_LENGTH,
                                                               eFormat::Depth_Float32,
                                                               eFormat::Float32_1);

                            // update constant buffer
                            Mat4 proj = MatrixUtil::CreatePerspective(PI_DIV2, 1.f, LIGHT_NEAR_PLANE, l.fallOffEnd);
                            GetGlobalConstants()->UpdateOmniShadowCaster(OmniShadowCasterConstant {
                                .shadowViewProj = {
                                    MatrixUtil::CreateViewLookDirection(t.position, Vec3 { 1, 0, 0 }) * proj,
                                    MatrixUtil::CreateViewLookDirection(t.position, Vec3 { -1, 0, 0 }) * proj,
                                    MatrixUtil::CreateViewLookDirection(t.position, Vec3 { 0, 1, 0 }, Vec3::Forward) * proj,
                                    MatrixUtil::CreateViewLookDirection(t.position, Vec3 { 0, -1, 0 }, Vec3::Backward) * proj,
                                    MatrixUtil::CreateViewLookDirection(t.position, Vec3 { 0, 0, 1 }) * proj,
                                    MatrixUtil::CreateViewLookDirection(t.position, Vec3 { 0, 0, -1 }) * proj,
                                },
                                .lightPosition = t.position,
                                .fallOffEnd    = l.fallOffEnd,
                            });

                            // bind shadow map
                            r.SetShaderResourceView(nullptr, OMNI_SHADOW_SLOT + lightIndex, eShaderFlags_PixelShader);
                            shadowMap->BindDepthBuffer();
                            shadowMap->Clear(1.f);
                            r.SetViewport(0, 0, shadowMap->GetResolution().x, shadowMap->GetResolution().y);

                            // cast object
                            (MaskPass<ShadowCastObjectTags>(
                                 in_scene, {}, GetGlobalPipeline()->GetOmniShadowCaster()),
                             ...);

                            // bind resource
                            r.SetRenderTarget(nullptr, nullptr);
                            shadowMap->BindDepthMapTexture(OMNI_SHADOW_SLOT + lightIndex, eShaderFlags_PixelShader);
                        }
                        break;

                        case eLightType::Spot:
                        {
                            if (!shadowMap || shadowMap->GetTextureType() != eResourceType::Texture2D)
                                shadowMap = CreateDepthMapTexture(SHADOW_MAP_LENGTH, SHADOW_MAP_LENGTH, eFormat::Depth_Float32, eFormat::Float32_1);

                            // update constant buffer
                            Mat4 lightViewProj = MatrixUtil::CreateViewFromQuaternion(t.position, t.rotate)
                                                 * MatrixUtil::CreatePerspective(l.outerConeAngle, 1.f, LIGHT_NEAR_PLANE, l.fallOffEnd);
                            GetGlobalConstants()->UpdateBasicShadowCaster(BasicShadowCasterConstant { .shadowViewProj = lightViewProj });
                            shadowConst.basicShadowViewProj[lightIndex] = lightViewProj;

                            // bind shadow map
                            r.SetShaderResourceView(nullptr, BASIC_SHADOW_SLOT + lightIndex, eShaderFlags_PixelShader);
                            shadowMap->BindDepthBuffer();
                            shadowMap->Clear(1.f);
                            r.SetViewport(0, 0, shadowMap->GetResolution().x, shadowMap->GetResolution().y);

                            // cast object
                            (MaskPass<ShadowCastObjectTags>(
                                 in_scene, {}, GetGlobalPipeline()->GetBasicShadowCaster()),
                             ...);

                            // bind resource
                            r.SetRenderTarget(nullptr, nullptr);
                            shadowMap->BindDepthMapTexture(BASIC_SHADOW_SLOT + lightIndex, eShaderFlags_PixelShader);
                        }
                        break;

                        default:
                            DEBUG_BREAK("Light type is not supported for shadow mapping");
                            break;
                    }
                }

                ++lightIndex;
            });

        GetGlobalConstants()->UpdateLight(lightConst);
        GetGlobalConstants()->UpdateShadowReceiver(shadowConst);
    }
};

}   // namespace crab