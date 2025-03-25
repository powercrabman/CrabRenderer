#include "InternalPCH.h"

#include "Input.h"

#include "AppWindow.h"
#include "Application.h"
#include "CrabEvent.h"
#include "EventDispatcher.h"

#include <SDL3/SDL_mouse.h>

namespace crab
{

Scope<Input::Impl> Input::m_impl = CreateScope<Input::Impl>();

struct Input::Impl
{
    // - Keyboard
    std::array<bool, eKey_Count> isKeyDown;
    std::array<bool, eKey_Count> prevKeyDown;

    // - Mouse
    std::array<bool, 3> isMouseDown;
    std::array<bool, 3> prevMouseDown;

    Vec2 mousePos;
    Vec2 mousePrevPos;
    Vec2 relativeMouseDeltaPos;

    Vec2 mouseDeltaScroll;
};

//===================================================
// Input
//===================================================

void Input::Update()
{
    // - Keyboard
    std::memcpy(m_impl->prevKeyDown.data(),
                m_impl->isKeyDown.data(),
                sizeof(bool) * m_impl->isKeyDown.size());

    // - Mouse
    std::memcpy(m_impl->prevMouseDown.data(),
                m_impl->isMouseDown.data(),
                sizeof(bool) * m_impl->isMouseDown.size());

    // - Mouse Pos

    m_impl->mousePrevPos = m_impl->mousePrevPos;

    SDL_GetMouseState(
        &m_impl->mousePos.x,
        &m_impl->mousePos.y);

    SDL_GetRelativeMouseState(
        &m_impl->relativeMouseDeltaPos.x,
        &m_impl->relativeMouseDeltaPos.y);
}

bool Input::IsKeyDown(eKey in_key)
{
    return m_impl->isKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyUp(eKey in_key)
{
    return !m_impl->isKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyHold(eKey in_key)
{
    return m_impl->isKeyDown[static_cast<uint64>(in_key)] && m_impl->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyPressed(eKey in_key)
{
    return m_impl->isKeyDown[static_cast<uint64>(in_key)] && !m_impl->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyReleased(eKey in_key)
{
    return !m_impl->isKeyDown[static_cast<uint64>(in_key)] && m_impl->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyAway(eKey in_key)
{
    return !m_impl->isKeyDown[static_cast<uint64>(in_key)] && !m_impl->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsMouseDown(eMouse in_mouse)
{
    return m_impl->isMouseDown[static_cast<uint32>(in_mouse) - 1];
}

bool Input::IsMouseUp(eMouse in_mouse)
{
    return !m_impl->isMouseDown[static_cast<uint32>(in_mouse) - 1];
}

bool Input::IsMousePressed(eMouse in_mouse)
{
    uint32 idx = static_cast<uint32>(in_mouse) - 1;
    return m_impl->isMouseDown[idx] && !m_impl->prevMouseDown[idx];
}

bool Input::IsMouseHold(eMouse in_mouse)
{
    uint32 idx = static_cast<uint32>(in_mouse) - 1;
    return m_impl->isMouseDown[idx] && m_impl->prevMouseDown[idx];
}

bool Input::IsMouseReleased(eMouse in_mouse)
{
    uint32 idx = static_cast<uint32>(in_mouse) - 1;
    return !m_impl->isMouseDown[idx] && m_impl->prevMouseDown[idx];
}

bool Input::IsMouseAway(eMouse in_mouse)
{
    uint32 idx = static_cast<uint32>(in_mouse) - 1;
    return !m_impl->isMouseDown[idx] && !m_impl->prevMouseDown[idx];
}

Vec2 Input::GetMousePos()
{
    return m_impl->mousePos;
}

Vec2 Input::GetPrevMousePos()
{
    return m_impl->mousePrevPos;
}

Vec2 Input::GetMouseDeltaPos()
{
    return m_impl->mousePos - m_impl->mousePrevPos;
}

Vec2 Input::GetMouseRelativeDeltaPos()
{
    return m_impl->relativeMouseDeltaPos;
}

void Input::SetMousePos(const Vec2& in_screenPos)
{
    SDL_WarpMouseInWindow(
        nullptr,
        in_screenPos.x,
        in_screenPos.y);
}

void Input::SetMousePosToCenter()
{
    Vec2 halfSize = GetAppWindow().GetResolution().ToVec2() * 0.5f;
    SetMousePos(halfSize);
}

// - Mouse
void Input::OnEvent(EventBase* in_event)
{
    EventDispatcher dispatcher { in_event };

    DISPATCH_EVENT(IOEvent_KeyDown,
                   [&](IOEvent_KeyDown* e)
                   {
                       m_impl->isKeyDown[static_cast<uint64>(e->GetKey())] = true;
                   });

    DISPATCH_EVENT(IOEvent_KeyUp,
                   [&](IOEvent_KeyUp* e)
                   {
                       m_impl->isKeyDown[static_cast<uint64>(e->GetKey())] = false;
                   });

    DISPATCH_EVENT(IOEvent_MouseScroll,
                   [&](IOEvent_MouseScroll* e)
                   {
                       m_impl->mouseDeltaScroll = e->GetDeltaScroll();
                   });

    DISPATCH_EVENT(IOEvent_MouseDown,
                   [&](IOEvent_MouseDown* e)
                   {
                       m_impl->isMouseDown[static_cast<uint64>(e->GetButton()) - 1] = true;
                   });

    DISPATCH_EVENT(IOEvent_MouseUp,
                   [&](IOEvent_MouseUp* e)
                   {
                       m_impl->isMouseDown[static_cast<uint64>(e->GetButton()) - 1] = false;
                   });
}

}   // namespace crab