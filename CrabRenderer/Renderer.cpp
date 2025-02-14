#include "CrabPch.h"

#include "Renderer.h"

#include "D11Renderer.h"

namespace crab
{

Scope<D11Renderer> Renderer::m_renderer;

void Renderer::Init(const RendererSetting& in_setting)
{
    // todo..
    m_renderer = CreateScope<D11Renderer>();
    m_renderer->Init(in_setting);
}

void Renderer::Shutdown()
{
    m_renderer->Shutdown();
}

void Renderer::OnEvent(CrabEvent& in_event)
{
    m_renderer->OnEvent(in_event);
}

void* Renderer::GetNativeRenderingAPI()
{
    return m_renderer.get();
}

void Renderer::BeginRender()
{
    m_renderer->BeginGUI();
}

void Renderer::EndRender()
{
    m_renderer->EndGUI();
}

void Renderer::Present()
{
    m_renderer->Present();
}

}   // namespace crab