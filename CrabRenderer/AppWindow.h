#pragma once
#include <SDL2/SDL.h>

namespace crab
{

//===================================================
// Application Window
//===================================================

class AppWindow
{
public:
    AppWindow();
    ~AppWindow();

    // - Core
    bool Init(const AppWindowSetting& setting);

    // - Setter
    void ResizeWindow(uint32 in_width, uint32 in_height);
    void MoveWindow(uint32 in_x, uint32 in_y);
    void SetWindowTitle(const std::string_view in_title);

    // - Getter
    auto GetWindowSize() const;
    auto GetWindowPos() const;
    float GetAspect() const;

    void*       GetNativeWindowHandle() const;
    SDL_Window* GetSDLWindow() const;

private:
    friend class Application;
    void _PollEvent();

    SDL_Window* m_window;
};


//===================================================
// Inline
//===================================================

inline auto AppWindow::GetWindowSize() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    int width, height;
    SDL_GetWindowSize(m_window, &width, &height);
    return std::make_pair(width, height);
}

inline auto AppWindow::GetWindowPos() const
{
    CRAB_ASSERT(m_window, "Window is not initialized.");
    int x, y;
    SDL_GetWindowPosition(m_window, &x, &y);
    return std::make_pair(x, y);
}		

}   // namespace crab