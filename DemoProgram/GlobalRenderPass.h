#pragma once

class GlobalRenderPass : public Singleton<GlobalRenderPass>
{
    friend class Singleton<GlobalRenderPass>;

public:
    ~GlobalRenderPass() = default;

    void Init();

    void BeginDrawNormalPass() const;

    void BeginPBRPass(uint32 in_stencilRef) const;
    void BeginPBRWireframePass(uint32 in_stencilRef) const;
    void BeginPBRReflectPass(uint32 in_stencilRef) const;
    void BeginPBRReflectWireframePass(uint32 in_stencilRef) const;

    void BeginSkyboxPass(uint32 in_stencilRef) const;
    void BeginSkyboxReflectPass(uint32 in_stencilRef) const;

    void BeginDepthOnlyPass() const;

    void BeginMirrorMaskingPass(uint32 in_stencilRef) const;

    [[nodiscard]] Ref<RenderPass> GetDrawNormalPass() { return m_drawNormalPass; }
    [[nodiscard]] Ref<RenderPass> GetPBRPass() { return m_pbrPass; }
    [[nodiscard]] Ref<RenderPass> GetPBRWireframePass() { return m_pbrWireframePass; }
    [[nodiscard]] Ref<RenderPass> GetPBRMirrorPass() { return m_pbrReflectPass; }
    [[nodiscard]] Ref<RenderPass> GetPBRMirrorWireframePass() { return m_pbrReflectWireframePass; }
    [[nodiscard]] Ref<RenderPass> GetSkyboxPass() { return m_skyboxPass; }
    [[nodiscard]] Ref<RenderPass> GetSkyboxMirrorPass() { return m_skyboxReflectPass; }
    [[nodiscard]] Ref<RenderPass> GetDepthOnlyState() { return m_depthOnlyPass; }
    [[nodiscard]] Ref<RenderPass> GetMirrorMaskingPass() { return m_mirrorMaskingPass; }

private:
    GlobalRenderPass() = default;

    Ref<RenderPass> m_drawNormalPass;

    Ref<RenderPass> m_pbrPass;
    Ref<RenderPass> m_pbrWireframePass;

    Ref<RenderPass> m_pbrReflectPass;
    Ref<RenderPass> m_pbrReflectWireframePass;

    Ref<RenderPass> m_skyboxPass;
    Ref<RenderPass> m_skyboxReflectPass;
    Ref<RenderPass> m_depthOnlyPass;

    Ref<RenderPass> m_mirrorMaskingPass;
};

inline GlobalRenderPass& GetGlobalRenderPass()
{
    return GlobalRenderPass::GetInstance();
}
