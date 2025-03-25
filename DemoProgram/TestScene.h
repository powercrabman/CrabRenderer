#pragma once

class TestScene : public Scene
{
public:
    ~TestScene() override;
    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;

    void OnPostRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(EventBase* in_event) override;

private:
    struct Vertex
    {
        Vec2 position;
        Vec3 color;
    };

    std::vector<Vertex> m_vertex;
    std::vector<Index>  m_index;

    Ref<VertexBuffer> m_vb;
    Ref<IndexBuffer>  m_ib;

    Ref<VertexShader> m_vs;
    Ref<PixelShader>  m_ps;

    Ref<FrameBuffer> m_frameBuffer;
    Viewport         m_viewport;
};
