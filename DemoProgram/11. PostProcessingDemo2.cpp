#include "pch.h"

#include "11. PostProcessingDemo2.h"

#include "D11_Model.h"
#include "GUIs.h"
#include "PostProcess.h"

void PostProcessingDemo2::Init()
{
    auto* dx = D11_API;

    auto backHoleShader = D11_PixelShader::CreateFromFile("BlackholePixelShader.hlsl", "main");
    auto sampleShader   = D11_PixelShader::CreateFromFile("SamplingPixelShader.hlsl", "main");
    auto vertexShader   = D11_VertexShader::CreateFromFile("PostProcessingVertexShader.hlsl",
                                                         "main",
                                                         PostProcessingVertex::s_inputElements);

    PostProcessSetting ppSetting;
    ppSetting.vertexShader = vertexShader;
    ppSetting.pixelShader  = backHoleShader;

    auto backHoleFilter = PostProcess::Create(ppSetting);
    auto backBufferTex  = dx->GetSwapChain()->GetBackBufferTexture();
    backHoleFilter->SetTexture(backBufferTex);
    m_postProcessStack.push_back(backHoleFilter);

    ppSetting.pixelShader = sampleShader;
    auto sampleFilter     = PostProcess::Create(ppSetting);
    sampleFilter->SetTexture(m_postProcessStack.back()->GetTexture());
    sampleFilter->SetRenderTarget(dx->GetSwapChain()->GetBackBuffer());
    m_postProcessStack.push_back(sampleFilter);
}

void PostProcessingDemo2::OnEnter()
{
}

void PostProcessingDemo2::OnExit()
{
}

void PostProcessingDemo2::OnUpdate(TimeStamp& in_ts)
{
}

void PostProcessingDemo2::OnRender(TimeStamp& in_ts)
{
    D11_API->SetTopology(eTopology::TriangleList);

    auto [width, height] = GetApplication().GetAppWindow().GetWindowSize();
    auto [mx, my]        = Input::GetMousePos();

    CB_PostProcess data;
    data.dx = width;
    data.dy = height;
    data.threshold = in_ts.deltaTime;
    data.userValues.x = mx;
    data.userValues.y = my;

    m_postProcessStack[0]->UpdateConstantData(data);

    for (auto& pp: m_postProcessStack)
    {
        pp->Render();
    }
}

void PostProcessingDemo2::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector("Post Processing 2");
}

void PostProcessingDemo2::OnEvent(CrabEvent& in_event)
{
}
