#include "pch.h"

#include "1. TriangleDemo.h"
#include "GUIs.h"

void TriangleDemo::Init()
{
    std::vector<Vertex> vb = {
        Vertex { Vec2 { -0.5f, 0.f } },
        Vertex { Vec2 { 0.f, 0.5f } },
        Vertex { Vec2 { 0.5f, 0.f } },
    };

    std::vector<uint32> ib = {
        0, 1, 2
    };

    m_vertexBuffer = D11_VertexBuffer::Create(vb);
    m_indexBuffer  = D11_IndexBuffer::Create(ib);

    const char* shaderCode = R"(
        float4 VSmain(float2 in_pos : POSITION) : SV_POSITION
        {
            return float4(in_pos, 0.f, 1.f);
        }
        
        float4 PSmain(float4 in_pos : SV_POSITION) : SV_TARGET
        {
            return float4(1.f, 0.f, 0.f, 1.f);
        }
    )";

    D11_InputElements elem;
    elem.Add("POSITION", 0, eDataFormat::Float2);

    m_vs = D11_VertexShader::CreateFromString(shaderCode, "VSmain", elem);
    m_ps = D11_PixelShader::CreateFromString(shaderCode, "PSmain");
}

void TriangleDemo::OnEnter()
{
}

void TriangleDemo::OnExit()
{
}

void TriangleDemo::OnUpdate(TimeStamp& in_ts)
{
}

void TriangleDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;
    dx->SetBackBufferToFrameBuffer();
    dx->ClearFrameBuffer(color::BLACK, true, false);
    dx->SetTopology(eTopology::TriangleList);

    dx->SetVertexBuffer(m_vertexBuffer);
    dx->SetIndexBuffer(m_indexBuffer);

    dx->SetVertexShader(m_vs);
    dx->SetPixelShader(m_ps);

    dx->DrawIndexed(m_indexBuffer->indexCount);
}

void TriangleDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName());
}

void TriangleDemo::OnEvent(CrabEvent& in_event)
{
}
