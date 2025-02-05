#pragma once

class Texture2DDemo : public Scene
{
public:
    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(Texture2DDemo);

private:
    void _CreateRasterizerState();

    using Vertex = Vertex_Pos2D_Tex_Color;

    cbuffer CB_Transform
    {
        Mat4 world;
    };

    cbuffer CB_Camera
    {
        Mat4 viewProj;
    };

    Ref<D11_Mesh> m_mesh;
    Ref<Texture>  m_tex;

    Ref<D11_VertexShader> m_vs;
    Ref<D11_PixelShader>  m_ps;

    Ref<D11_ConstantBuffer<CB_Transform>> m_cbTransform;
    Ref<D11_ConstantBuffer<CB_Camera>>    m_cbCamera;

    Ref<D11_RasterizerState> m_rsState;
    bool                     m_wireframe  = false;
    bool                     m_cullenable = true;
    bool                     m_rsDirty    = false;

    // - Camera
    Transform                   m_transform;
    Transform                   m_cmrTransform;
    CameraComponent             m_cmr;
};
