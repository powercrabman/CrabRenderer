#pragma once

namespace crab
{

class ApplicationEvent_AppClose : public EventBase
{
public:
    EVENT_IMPL(ApplicationEvent_AppClose,
               "Application Close Event");
};

class WindowEvent_WindowMove : public EventBase
{
public:
    WindowEvent_WindowMove(int32 in_x, int32 in_y)
        : m_x(in_x)
        , m_y(in_y)
    {
    }

    int32 GetX() const { return m_x; }
    int32 GetY() const { return m_y; }

    EVENT_IMPL(WindowEvent_WindowMove,
               fmt::format("Window Move Event (x: {0}, y: {1})", m_x, m_y).c_str());

private:
    int32 m_x;
    int32 m_y;
};

class WindowEvent_WindowResize : public EventBase
{
public:
    WindowEvent_WindowResize(uint32 in_width, uint32 in_height)
        : m_width(in_width)
        , m_height(in_height)
    {
    }

    uint32 GetWidth() const { return m_width; }
    uint32 GetHeight() const { return m_height; }

    EVENT_IMPL(WindowEvent_WindowResize,
               fmt::format("Window Resize Event (width: {0}, height: {1})", m_width, m_height).c_str());

private:
    uint32 m_width;
    uint32 m_height;
};

class IOEvent_KeyUp : public EventBase
{
public:
    IOEvent_KeyUp(uint32 in_key)
        : m_key(in_key)
    {
    }
    uint32 GetKey() const { return m_key; }

    EVENT_IMPL(IOEvent_KeyUp,
               fmt::format("Key Up Event (key: {0})", m_key).c_str());

private:
    uint32 m_key;
};

class IOEvent_KeyDown : public EventBase
{
public:
    IOEvent_KeyDown(uint32 in_key)
        : m_key(in_key)
    {
    }
    uint32 GetKey() const { return m_key; }
    EVENT_IMPL(IOEvent_KeyDown,
               fmt::format("Key Down Event (key: {0})", m_key).c_str());

private:
    uint32 m_key;
};

class IOEvent_MouseScroll : public EventBase
{
public:
    IOEvent_MouseScroll(const Vec2& in_deltaScroll)
        : m_deltaScroll(in_deltaScroll)
    {
    }
    Vec2 GetDeltaScroll() const { return m_deltaScroll; }
    EVENT_IMPL(IOEvent_MouseScroll,
               fmt::format("Mouse Scroll Event (deltaScrollX {0}, deltaScrollY {1})",
                           m_deltaScroll.x,
                           m_deltaScroll.y)
                   .c_str());

private:
    Vec2 m_deltaScroll;
};

class IOEvent_MouseDown : public EventBase
{
public:
    IOEvent_MouseDown(int32 in_button)
        : m_button(in_button)
    {
    }
    int32 GetButton() const { return m_button; }
    EVENT_IMPL(IOEvent_MouseDown,
               fmt::format("Mouse Down Event (button: {0})", m_button).c_str());

private:
    int32 m_button;
};

class IOEvent_MouseUp : public EventBase
{
public:
    IOEvent_MouseUp(int32 in_button)
        : m_button(in_button)
    {
    }
    int32 GetButton() const { return m_button; }
    EVENT_IMPL(IOEvent_MouseUp,
               fmt::format("Mouse Up Event (button: {0})", m_button).c_str());

private:
    int32 m_button;
};


class RendererEvent_ResizeBackBuffer : public EventBase
{
public:
    RendererEvent_ResizeBackBuffer() = default;
    EVENT_IMPL(RendererEvent_ResizeBackBuffer,
               "Renderer Resize BackBuffer Event");

};

}   // namespace crab
