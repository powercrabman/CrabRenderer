#pragma once

namespace crab
{
//
//class PBRGeometryPass : public PBRPassBase
//{
//public:
//    void Init(const Ref<RenderTarget>& in_positionRT,
//              const Ref<RenderTarget>& in_normalRT,
//              const Ref<RenderTarget>& in_emissiveRT,
//              const Ref<RenderTarget>& in_albedoRoughnessRT,
//              const Ref<RenderTarget>& in_metallicAORT,
//              const Ref<DepthBuffer>&  in_depthBuffer)
//    {
//        m_positionRT             = in_positionRT;
//        m_normalRT               = in_normalRT;
//        m_emissiveRT             = in_emissiveRT;
//        m_albedoRoughnessRT      = in_albedoRoughnessRT;
//        m_metallicAORT           = in_metallicAORT;
//        m_gBufferPassDepthBuffer = in_depthBuffer;
//        m_rts                    = { m_positionRT->GetRTV(),
//                                     m_normalRT->GetRTV(),
//                                     m_emissiveRT->GetRTV(),
//                                     m_albedoRoughnessRT->GetRTV(),
//                                     m_metallicAORT->GetRTV() };
//    }
//
//    template<StringLiteral... Tag>
//    void Draw(Scene*                      in_scene,
//              const Ref<PipelineState>&   in_pipelineState,
//              const PipelineBindArgument& in_args = {})
//    {
//        in_pipelineState->Bind(in_args);
//
//        // RTV & DSV Bind & Clear
//        auto& r = GetRenderer();
//        r.SetRenderTargets(m_rts.data(),
//                           static_cast<uint32>(m_rts.size()),
//                           m_gBufferPassDepthBuffer->GetDSV());
//
//        m_positionRT->Clear(color4::BLACK);
//        m_normalRT->Clear(color4::BLACK);
//        m_emissiveRT->Clear(color4::BLACK);
//        m_albedoRoughnessRT->Clear(color4::BLACK);
//        m_metallicAORT->Clear(color4::BLACK);
//        m_gBufferPassDepthBuffer->Clear(true, 1.f, true, 0);
//
//        // draw call
//        (DrawCall_Internal<Tag>(in_scene), ...);
//
//        r.ReleaseRenderTargets();
//    }
//
//private:
//    enum
//    {
//        USING_RENDER_TARGET_COUNT = 5
//    };
//
//    template<StringLiteral Tag>
//    void DrawCall_Internal(Scene* in_scene)
//    {
//        using RenderGroup = RenderGroup<Tag>;
//        in_scene->GetView<TransformComponent, ModelRenderer, RenderGroup>().each(
//            [](const TransformComponent& t, const ModelRenderer& m)
//            {
//                GetGlobalConstants()->UpdateTransform(
//                    CB_TRANSFORM {
//                        .world             = t.GetWorld(),
//                        .worldInvTranspose = t.GetWorld().Invert().Transpose() });
//
//                // Material
//                for (const auto& [mesh, material, _]: m.model->GetNodes())
//                {
//                    MaterialConstant mat;
//                    mat.baseColor = material->GetBaseColor();
//                    mat.metallic  = material->GetMetallic();
//                    mat.emissive  = material->GetEmissive();
//                    mat.roughness = material->GetRoughness();
//                    mat.alpha     = material->GetAlpha();
//
//                    mat.usingTextureFlags = eMaterialTextureUsingFlags_None;
//
//                    if (const auto& tex = material->GetBaseColorTexture())
//                    {
//                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Albedo;
//                        tex->Bind(ALBEDO_TEXTURE_SLOT, eShaderFlags_PixelShader);
//                    }
//
//                    if (const auto& tex = material->GetNormalTexture())
//                    {
//                        if (material->GetNormalMapType() == eNormalMapType::OpenGL)
//                            mat.usingTextureFlags |= eMaterialTextureUsingFlags_Normal_GL;
//                        else
//                            mat.usingTextureFlags |= eMaterialTextureUsingFlags_Normal_DX;
//
//                        tex->Bind(NORMAL_TEXTURE_SLOT, eShaderFlags_PixelShader);
//                    }
//
//                    if (const auto& tex = material->GetAOTexture())
//                    {
//                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_AO;
//                        tex->Bind(AO_TEXTURE_SLOT, eShaderFlags_PixelShader);
//                    }
//
//                    if (const auto& tex = material->GetMetallicTexture())
//                    {
//                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Metallic;
//                        tex->Bind(METALLIC_TEXTURE_SLOT, eShaderFlags_PixelShader);
//                    }
//
//                    if (const auto& tex = material->GetRoughnessTexture())
//                    {
//                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Roughness;
//                        tex->Bind(ROUGHNESS_TEXTURE_SLOT, eShaderFlags_PixelShader);
//                    }
//
//                    if (const auto& tex = material->GetEmissiveTexture())
//                    {
//                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Emissive;
//                        tex->Bind(EMISSIVE_TEXTURE_SLOT, eShaderFlags_PixelShader);
//                    }
//
//                    if (const auto& tex = material->GetDisplacementTexture())
//                    {
//                        mat.usingTextureFlags |= eMaterialTextureUsingFlags_Displacement;
//                        tex->Bind(DISPLACEMENT_TEXTURE_SLOT, eShaderFlags_VertexShader);
//                    }
//
//                    GetGlobalConstants()->UpdateMaterial(mat);
//
//                    mesh->Draw(TODO);
//                }
//            });
//    }
//
//    using RenderTargetArray = std::array<ID3D11RenderTargetView*, USING_RENDER_TARGET_COUNT>;
//
//    Ref<RenderTarget> m_positionRT;          // 0
//    Ref<RenderTarget> m_normalRT;            // 1
//    Ref<RenderTarget> m_emissiveRT;          // 2
//    Ref<RenderTarget> m_albedoRoughnessRT;   // 3
//    Ref<RenderTarget> m_metallicAORT;        // 4
//    Ref<DepthBuffer>  m_gBufferPassDepthBuffer;
//    RenderTargetArray m_rts = {};
//};

}   // namespace crab