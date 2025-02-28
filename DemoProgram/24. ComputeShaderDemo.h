#pragma once

class ComputeShaderDemo final : public Scene
{
public:
    ComputeShaderDemo();
    virtual ~ComputeShaderDemo();

    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(ComputeShaderDemo);

private:
    Ref<Texture2D> m_texture;

    ComPtr<ID3D11UnorderedAccessView> m_uav;
    Ref<ComputeShader>                m_computeShader;

    Ref<VertexShader>  m_vs;
    Ref<PixelShader>   m_ps;
    Ref<ComputeShader> m_cs;

    Ref<VertexBuffer> m_vb;
    Ref<IndexBuffer>  m_ib;
};
