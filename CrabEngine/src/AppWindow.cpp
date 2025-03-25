#include "InternalPCH.h"

#include "AppWindow.h"

#include "Application.h"
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

bool AppWindow::Init(const AppWindowInitInfo& in_setting)
{
    // Set MetaData
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, in_setting.windowTitle.c_str());

    // SDL Init
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        DEBUG_BREAK(fmt::format("Failed to initialize SDL. {0}", SDL_GetError()).c_str());
        return false;
    }

    uint32 width  = in_setting.windowWidth;
    uint32 height = in_setting.windowHeight;
    uint32 x      = in_setting.windowPosX;
    uint32 y      = in_setting.windowPosY;

    // - Create Window
    m_window = SDL_CreateWindow(in_setting.windowTitle.c_str(), x, y, 0);

    if (!m_window)
    {
        ASSERT(false, "Failed to create window. {0}");
        return false;
    }

    // Center Alignment
    if (in_setting.centerAlign)
    {
        auto [displayWidth, displayHeight] = GetDisplaySize();

        x = static_cast<uint32>((displayWidth - width) * 0.5f);
        y = static_cast<uint32>((displayHeight - height) * 0.5f);
    }

    MoveWindow(x, y);
    ResizeWindow(width, height);

    if (!m_window)
    {
        DEBUG_BREAK("Failed to create window.");
        return false;
    }

    return true;
}

void AppWindow::ResizeWindow(uint32 in_width, uint32 in_height) const
{
    ASSERT(m_window, "Window is not initialized.");
    SDL_SetWindowSize(m_window,
                      static_cast<int>(in_width),
                      static_cast<int>(in_height));
}

void AppWindow::MoveWindow(uint32 in_x, uint32 in_y) const
{
    ASSERT(m_window, "Window is not initialized.");
    SDL_SetWindowPosition(m_window,
                          static_cast<int>(in_x),
                          static_cast<int>(in_y));
}

void AppWindow::SetWindowTitle(const std::string_view in_title) const
{
    ASSERT(m_window, "Window is not initialized.");
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

IVec2 AppWindow::GetResolution() const
{
    ASSERT(m_window, "Window is not initialized.");

    int width, height;
    SDL_GetWindowSize(m_window, &width, &height);
    return { width, height };
}

IVec2 AppWindow::GetWindowPos() const
{
    ASSERT(m_window, "Window is not initialized.");

    int x, y;
    SDL_GetWindowPosition(m_window, &x, &y);

    return { x, y };
}

IVec2 AppWindow::GetDisplaySize() const
{
    ASSERT(m_window, "Window is not initialized.");

    IVec2         output = {};
    SDL_DisplayID id     = SDL_GetDisplayForWindow(m_window);

    SDL_Rect displayRect;
    if (SDL_GetDisplayBounds(id, &displayRect))
    {
        output.x = displayRect.w;
        output.y = displayRect.h;
    }
    else
    {
        DEBUG_BREAK("Failed to get display size.");
    }

    return output;
}

float AppWindow::GetAspect() const
{
    ASSERT(m_window, "Window is not initialized.");
    auto [width, height] = GetResolution();
    return static_cast<float>(width) / static_cast<float>(height);
}

// - Getter
HWND AppWindow::GetWindowHandle() const
{
    ASSERT(m_window, "Window is not initialized.");

    SDL_PropertiesID id = SDL_GetWindowProperties(m_window);

    void* defaultValue = nullptr;
    void* hwnd         = SDL_GetPointerProperty(
        id,
        SDL_PROP_WINDOW_WIN32_HWND_POINTER,
        defaultValue);

    ASSERT(hwnd, "Failed to get native window handle.");
    return static_cast<HWND>(hwnd);
}

SDL_Window* AppWindow::GetSDLWindow() const
{
    ASSERT(m_window, "Window is not initialized.");
    return m_window;
}

// - Getter

void AppWindow::PollEvent_Internal()
{
    SDL_Event event = {};

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);

        switch (event.type)
        {
            case SDL_EVENT_QUIT:
            {
                ApplicationEvent_AppClose e;
                GetApplication().DispatchEventLater(e);
            }
            break;

            case SDL_EVENT_WINDOW_RESIZED:
            {
                WindowEvent_WindowResize e(event.window.data1, event.window.data2);
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_WINDOW_MOVED:
            {
                WindowEvent_WindowMove e { event.window.data1, event.window.data2 };
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_KEY_DOWN:
            {
                IOEvent_KeyDown e(event.key.scancode);
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_KEY_UP:
            {
                IOEvent_KeyUp e(event.key.scancode);
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_MOUSE_WHEEL:
            {
                IOEvent_MouseScroll e { Vec2 { event.wheel.x, event.wheel.y } };
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                IOEvent_MouseDown e { event.button.button };
                GetApplication().DispatchEvent(e);
            }
            break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                IOEvent_MouseUp e { event.button.button };
                GetApplication().DispatchEvent(e);
            }
            break;
        }
    }
}

}   // namespace crab