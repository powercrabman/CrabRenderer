#pragma once

class CameraDemo : public Scene
{
public:
    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(CameraDemo);

private:
    struct Vertex
    {
        Vec2 position;
        Vec4 color;
    };

    cbuffer CB_Transform
    {
        Mat4 world;
    };

    cbuffer CB_Camera
    {
        Mat4 viewProj;
    };

    Ref<D11_VertexBuffer> m_vertexBuffer;
    Ref<D11_IndexBuffer>  m_indexBuffer;

    Ref<D11_VertexShader> m_vs;
    Ref<D11_PixelShader>  m_ps;

    Ref<D11_ConstantBuffer<CB_Transform>> m_cbTransform;
    Ref<D11_ConstantBuffer<CB_Camera>>    m_cbCamera;

    Transform          m_transform;
    Transform          m_cmrTransform;
    CameraComponent    m_cmr;

    Viewport m_viewport;
};
