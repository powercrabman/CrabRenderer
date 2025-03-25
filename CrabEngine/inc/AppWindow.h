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
    bool Init(const AppWindowInitInfo& setting);

    // - Setter
    void ResizeWindow(uint32 in_width, uint32 in_height) const;
    void MoveWindow(uint32 in_x, uint32 in_y) const;
    void SetWindowTitle(const std::string_view in_title) const;
    void EnableMouseGrabMode(bool in_enable) const;
    void EnableMouseRelativeMode(bool in_enabled) const;

    // - Getter
    IVec2 GetResolution() const;
    IVec2 GetWindowPos() const;
    IVec2 GetDisplaySize() const;
    float GetAspect() const;

    HWND GetWindowHandle() const;
    SDL_Window* GetSDLWindow() const;

private:
    friend class Application;
    void PollEvent_Internal();

    SDL_Window* m_window;
};

}   // namespace crab