#include "pch.h"

#include "TestScene.h"

#include "GUIs.h"

TestScene::~TestScene()
{
}

void TestScene::Init()
{
    // IA
    m_vertex = {

        Vertex { Vec2 { -0.5f, 0.f }, Vec3 { 1.f, 0.f, 0.f } },
        Vertex { Vec2 { 0.f, 0.5f }, Vec3 { 0.f, 1.f, 0.f } },
        Vertex { Vec2 { 0.5f, 0.f }, Vec3 { 0.f, 0.f, 1.f } },
    };

    m_index = { 0, 1, 2 };

    m_vb = CreateRef<VertexBuffer>();
    m_vb->Init(m_vertex);

    m_ib = CreateRef<IndexBuffer>();
    m_ib->Init(m_index);

    // VS
    const char* shaderCode = R"(
    
    struct VSInput
    {
        float2 position : POSITION;
        float3 color : COLOR;
    };

    struct PSInput
    {
        float4 position : SV_POSITION;
        float3 color : COLOR;
    };

    PSInput VSmain(VSInput input)
    {
        PSInput output;
        output.position = float4(input.position, 0.f, 1.f);
        output.color = input.color;
        return output;
    }

    float4 PSmain(PSInput input) : SV_TARGET
    {
        return float4(input.color, 1.f);
    }
    )";

    InputLayoutBuilder builder;
    builder
        .AddElement("POSITION", 0, eFormat::Float32_2)
        .AddElement("COLOR", 0, eFormat::Float32_3);

    m_vs = CreateRef<VertexShader>();
    m_vs->CompileCode(
        shaderCode,
        "VSmain",
        ShaderMacros::EmptyMacro(),
        builder.Build());

    m_ps = CreateRef<PixelShader>();
    m_ps->CompileCode(
        shaderCode,
        "PSmain");

    // RenderTarget
    m_viewport    = Viewport::CreateFullScreen();
    m_frameBuffer = GetRenderer().GetBackBufferFB();
}

void TestScene::OnEnter()
{
}

void TestScene::OnExit()
{
}

void TestScene::OnUpdate(TimeStamp& in_ts)
{
    if (Input::IsKeyDown(eKey::Escape))
        GetApplication().Quit();
}

void TestScene::OnRender(TimeStamp& in_ts)
{
    GetRenderer().SetTopology(eTopology::TriangleList);

    m_vb->Bind();
    m_ib->Bind();

    m_vs->Bind();
    m_ps->Bind();

    m_frameBuffer->Bind();
    m_frameBuffer->ClearRenderTargets(color4::BLACK);
    m_frameBuffer->ClearDepthBuffer(true, 1.f, false, 0);

    m_viewport.Bind();

    GetRenderer().DrawIndexed(static_cast<uint32>(m_index.size()));
}

void TestScene::OnPostRender(TimeStamp& in_ts)
{
}

void TestScene::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName());
}

void TestScene::OnEvent(EventBase* in_event)
{
    EventDispatcher dispatcher { in_event };

    DISPATCH_EVENT(WindowEvent_WindowResize,
                   [&](WindowEvent_WindowResize* e)
                   {
                       m_frameBuffer.reset();
                   });

    DISPATCH_EVENT(RendererEvent_ResizeBackBuffer,
                   [&](RendererEvent_ResizeBackBuffer* e)
                   {
                       m_frameBuffer = GetRenderer().GetBackBufferFB();
                       m_viewport    = Viewport::CreateFullScreen();
                   });
}