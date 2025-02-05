#include "CrabPch.h"

#include "AppWindow.h"

#include "CrabEvent.h"
#include <SDL2/SDL_syswm.h>

namespace crab
{

// - AppWindow
AppWindow::AppWindow()
    : m_window(nullptr)
{
}

AppWindow::~AppWindow()
{
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
    }
}

bool AppWindow::Init(const AppWindowSetting& setting)
{
    // - Center Alignment
    uint32 windowPosX = setting.windowPosX;
    uint32 windowPosY = setting.windowPosY;

    if (windowPosY == AppWindowSetting::CENTER_ALIGNMENT)
        windowPosY = SDL_WINDOWPOS_CENTERED;

    if (windowPosX == AppWindowSetting::CENTER_ALIGNMENT)
        windowPosX = SDL_WINDOWPOS_CENTERED;

    // - Create Window
    m_window = SDL_CreateWindow(
        setting.windowTitle.c_str(),
        windowPosX,
        windowPosY,
        setting.windowWidth,
        setting.windowHeight,
        SDL_WINDOW_SHOWN);

    if (!m_window)
    {
        CRAB_DEBUG_BREAK("Failed to create window.");
        return false;
    }

    return true;
}

void AppWindow::ResizeWindow(uint32 in_width, uint32 in_height)
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    SDL_SetWindowSize(m_window, in_width, in_height);
}

void AppWindow::MoveWindow(uint32 in_x, uint32 in_y)
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    SDL_SetWindowPosition(m_window, in_x, in_y);
}

void AppWindow::SetWindowTitle(const std::string_view in_title)
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    SDL_SetWindowTitle(m_window, in_title.data());
}

float AppWindow::GetAspect() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    auto [width, height] = GetWindowSize();
    return static_cast<float>(width) / static_cast<float>(height);
}

// - Getter

void* AppWindow::GetNativeWindowHandle() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");

    SDL_SysWMinfo wmInfo = {};
    if (!SDL_GetWindowWMInfo(m_window, &wmInfo))
    {
        CRAB_DEBUG_BREAK("Failed to get window manager info.");
        return nullptr;
    }

    return wmInfo.info.win.window;
}

SDL_Window* AppWindow::GetSDLWindow() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    return m_window;
}

// - Getter

void AppWindow::_PollEvent()
{
    SDL_Event event = {};

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
            {
                AppClose_CoreEvent e;
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_WINDOWEVENT:
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                    {
                        Resize_WindowEvent e;
                        e.width  = event.window.data1;
                        e.height = event.window.data2;
                        GetApplication().DispatchEvent(e);
                    }
                    break;

                    case SDL_WINDOWEVENT_MOVED:
                    {
                        Move_WindowEvent e;
                        e.x = event.window.data1;
                        e.y = event.window.data2;
                        GetApplication().DispatchEvent(e);
                    }
                    break;
                }
                break;
            }

            case SDL_KEYDOWN:
            {
                KeyDown_IOEvent e;
                e.key = event.key.keysym.scancode;
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_KEYUP:
            {
                KeyUp_IOEvent e;
                e.key = event.key.keysym.scancode;
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_MOUSEWHEEL:
            {
                MouseScroll_IOEvent e;
                e.dx = event.wheel.x;
                e.dy = event.wheel.y;
                GetApplication().DispatchEvent(e);
            }
            break;
        }
    }
}

}   // namespace crab