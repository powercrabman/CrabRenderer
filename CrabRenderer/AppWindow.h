#pragma once

struct SDL_Window;

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
    void ResizeWindow(const Int2& in_size) const;
    void MoveWindow(const Int2& in_position) const;
    void SetWindowTitle(const std::string_view in_title) const;
    void EnableMouseGrabMode(bool in_enable) const;
    void EnableMouseRelativeMode(bool in_enabled) const;

    // - Getter
    Int2  GetWindowSize() const;
    Int2  GetWindowPos() const;
    Int2  GetDisplaySize() const;
    float GetAspect() const;

    HWND GetWindowHandle() const;
    SDL_Window* GetSDLWindow() const;

private:
    friend class Application;
    void _PollEvent();

    SDL_Window* m_window;
};

}   // namespace crab