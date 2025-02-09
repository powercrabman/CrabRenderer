#include "CrabPch.h"

#include "Input.h"

#include "AppWindow.h"
#include "CrabEvent.h"
#include "EventDispatcher.h"
#include <SDL2/SDL.h>

namespace crab
{

crab::Scope<crab::CrabIO> Input::m_io = nullptr;

//===================================================
// Input
//===================================================

void Input::Init()
{
    m_io = CreateScope<CrabIO>();
}

void Input::Update()
{
    // - Keyboard
    std::memcpy(m_io->prevKeyDown.data(),
                m_io->isKeyDown.data(),
                sizeof(bool) * m_io->isKeyDown.size());

    // - Mouse
    std::memcpy(m_io->prevMouseDown.data(),
                m_io->isMouseDown.data(),
                sizeof(bool) * m_io->isMouseDown.size());

    // - Mouse Pos

    m_io->prevMousePosX = m_io->mousePosX;
    m_io->prevMousePosY = m_io->mousePosY;

    SDL_GetMouseState(&m_io->mousePosX, &m_io->mousePosY);
    SDL_GetRelativeMouseState(&m_io->relativeMouseDeltaX, &m_io->relativeMouseDeltaY);
}

bool Input::IsKeyDown(eKey in_key)
{
    return m_io->isKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyUp(eKey in_key)
{
    return !m_io->isKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyHold(eKey in_key)
{
    return m_io->isKeyDown[static_cast<uint64>(in_key)] && m_io->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyPressed(eKey in_key)
{
    return m_io->isKeyDown[static_cast<uint64>(in_key)] && !m_io->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyReleased(eKey in_key)
{
    return !m_io->isKeyDown[static_cast<uint64>(in_key)] && m_io->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyAway(eKey in_key)
{
    return !m_io->isKeyDown[static_cast<uint64>(in_key)] && !m_io->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsMouseDown(eMouse in_mouse)
{
    return m_io->isMouseDown[(uint32)in_mouse - 1];
}

bool Input::IsMouseUp(eMouse in_mouse)
{
    return !m_io->isMouseDown[(uint32)in_mouse - 1];
}

bool Input::IsMousePressed(eMouse in_mouse)
{
    uint32 idx = (uint32)in_mouse - 1;
    return m_io->isMouseDown[idx] && !m_io->prevMouseDown[idx];
}

bool Input::IsMouseHold(eMouse in_mouse)
{
    uint32 idx = (uint32)in_mouse - 1;
    return m_io->isMouseDown[idx] && m_io->prevMouseDown[idx];
}

bool Input::IsMouseReleased(eMouse in_mouse)
{
    uint32 idx = (uint32)in_mouse - 1;
    return !m_io->isMouseDown[idx] && m_io->prevMouseDown[idx];
}

bool Input::IsMouseAway(eMouse in_mouse)
{
    uint32 idx = (uint32)in_mouse - 1;
    return !m_io->isMouseDown[idx] && !m_io->prevMouseDown[idx];
}

void Input::SetMousePos(uint32 in_x, uint32 in_y)
{
    SDL_WarpMouseInWindow(nullptr, in_x, in_y);
}

void Input::SetMousePosToCenter()
{
    auto [w, h] = GetAppWindow().GetWindowSize();
    SetMousePos(w / 2, h / 2);
}

// - Mouse
void Input::OnEvent(CrabEvent& in_event)
{
    EventDispatcher dispatcher { in_event };

    HANDLE_EVENT(KeyDown_IOEvent,
                 [&](const KeyDown_IOEvent& e)
                 {
                     m_io->isKeyDown[static_cast<uint64>(e.key)] = true;
                 });

    HANDLE_EVENT(KeyUp_IOEvent,
                 [&](const KeyUp_IOEvent& e)
                 {
                     m_io->isKeyDown[static_cast<uint64>(e.key)] = false;
                 });

    HANDLE_EVENT(MouseScroll_IOEvent,
                 [&](const MouseScroll_IOEvent& e)
                 {
                     m_io->mouseDeltaScrollX = e.dx;
                     m_io->mouseDeltaScrollY = e.dy;
                 });

    HANDLE_EVENT(MouseDown_IOEvent,
                 [&](const MouseDown_IOEvent& e)
                 {
                     m_io->isMouseDown[static_cast<uint64>(e.button) - 1] = true;
                 });

    HANDLE_EVENT(MouseUp_IOEvent,
                 [&](const MouseUp_IOEvent& e)
                 {
                     m_io->isMouseDown[static_cast<uint64>(e.button) - 1] = false;
                 });
}

}   // namespace crab