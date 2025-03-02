#include "CrabPch.h"

#include "AppWindow.h"
#include "ImguiGlue.h"

#include "D11Renderer.h"

#include <ImGuizmo.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_sdl3.h>

namespace crab
{

void ImguiGlue::Init(
    SDL_Window* in_sdlWindow, 
    ID3D11Device* in_device, 
    ID3D11DeviceContext* in_context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    ImGui_ImplSDL3_InitForD3D(in_sdlWindow);
    ImGui_ImplDX11_Init(in_device, in_context);
}

void ImguiGlue::BeginRender()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void ImguiGlue::EndRender()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void ImguiGlue::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

}   // namespace crab