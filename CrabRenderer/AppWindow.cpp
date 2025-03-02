#include "CrabPch.h"

#include "AppWindow.h"

#include "CrabEvent.h"

#include <SDL3/SDL.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_sdl3.h>

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

bool AppWindow::Init(const AppWindowSetting& in_setting)
{
    // SDL Init
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        CRAB_DEBUG_BREAK("Failed to initialize SDL.");
        return false;
    }

    Int2 windowSize = in_setting.windowSize;
    Int2 windowPos  = in_setting.windowPos;

    // - Create Window
    m_window = SDL_CreateWindow(
        in_setting.windowTitle.c_str(),
        windowSize.x,
        windowSize.y,
        0);

    // Center Alignment
    Int2 displaySize = GetDisplaySize();   // 고치기
    Int2 centerAlign = (displaySize - windowSize) * 0.5f;

    if (windowPos.x == AppWindowSetting::CENTER_ALIGNMENT)
        windowPos.x = centerAlign.x;

    if (windowPos.y == AppWindowSetting::CENTER_ALIGNMENT)
        windowPos.y = centerAlign.y;

    MoveWindow(windowPos);
    ResizeWindow(windowSize);

    if (!m_window)
    {
        CRAB_DEBUG_BREAK("Failed to create window.");
        return false;
    }

    return true;
}

void AppWindow::ResizeWindow(const Int2& in_size) const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    SDL_SetWindowSize(m_window, in_size.x, in_size.y);
}

void AppWindow::MoveWindow(const Int2& in_position) const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    SDL_SetWindowPosition(m_window, in_position.x, in_position.y);
}

void AppWindow::SetWindowTitle(const std::string_view in_title) const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    SDL_SetWindowTitle(m_window, in_title.data());
}

void AppWindow::EnableMouseGrabMode(bool in_clamp) const
{
    SDL_SetWindowKeyboardGrab(m_window, in_clamp);
}

void AppWindow::EnableMouseRelativeMode(bool in_enabled) const
{
    SDL_SetWindowRelativeMouseMode(m_window, in_enabled);
}

Int2 AppWindow::GetWindowSize() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");

    Int2 size;
    SDL_GetWindowSize(m_window, &size.x, &size.y);

    return size;
}

Int2 AppWindow::GetWindowPos() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");

    Int2 pos;
    SDL_GetWindowPosition(m_window, &pos.x, &pos.y);
    return pos;
}

Int2 AppWindow::GetDisplaySize() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");

    SDL_DisplayID id = SDL_GetDisplayForWindow(m_window);

    SDL_Rect displayRect;
    if (SDL_GetDisplayBounds(id, &displayRect))
    {
        return { displayRect.w, displayRect.h };
    }
    else
    {
        CRAB_DEBUG_BREAK("Failed to get display size.");
        return {};
    }
}

float AppWindow::GetAspect() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    auto [width, height] = GetWindowSize();
    return static_cast<float>(width) / static_cast<float>(height);
}

// - Getter
HWND AppWindow::GetWindowHandle() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");

    SDL_PropertiesID id = SDL_GetWindowProperties(m_window);

    void* defaultValue = nullptr;
    void* hwnd         = SDL_GetPointerProperty(
        id,
        SDL_PROP_WINDOW_WIN32_HWND_POINTER,
        defaultValue);

    CRAB_ASSERT(hwnd, "Failed to get native window handle.");
    return static_cast<HWND>(hwnd);
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
        ImGui_ImplSDL3_ProcessEvent(&event);

        switch (event.type)
        {
            case SDL_EVENT_QUIT:
            {
                AppClose_CoreEvent e;
                GetApplication().DispatchEventLater(e);
            }
            break;

            case SDL_EVENT_WINDOW_RESIZED:
            {
                Resize_WindowEvent e;
                e.size.x = event.window.data1;
                e.size.y = event.window.data2;
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_WINDOW_MOVED:
            {
                Move_WindowEvent e;
                e.position.x = event.window.data1;
                e.position.y = event.window.data2;
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_KEY_DOWN:
            {
                KeyDown_IOEvent e;
                e.key = event.key.scancode;
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_KEY_UP:
            {
                KeyUp_IOEvent e;
                e.key = event.key.scancode;
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_MOUSE_WHEEL:
            {
                MouseScroll_IOEvent e;
                e.deltaScroll.x = event.wheel.x;
                e.deltaScroll.y = event.wheel.y;
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                MouseDown_IOEvent e;
                e.button = event.button.button;
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                MouseUp_IOEvent e;
                e.button = event.button.button;
                GetApplication().DispatchEvent(e);
            }
            break;
        }
    }
}

}   // namespace crab