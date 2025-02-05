#include "CrabPch.h"

#include "Input.h"

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
    m_io->prevMousePosX = m_io->mousePosX;
    m_io->prevMousePosY = m_io->mousePosY;

    SDL_GetMouseState(&m_io->mousePosX,
                      &m_io->mousePosY);
}

bool Input::IsKeyDown(eKey in_key)
{
    return m_io->isKeyDown[static_cast<uint64>(in_key)] && m_io->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyUp(eKey in_key)
{
    return !m_io->isKeyDown[static_cast<uint64>(in_key)] && !m_io->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyPressed(eKey in_key)
{
    return m_io->isKeyDown[static_cast<uint64>(in_key)] && !m_io->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsKeyReleased(eKey in_key)
{
    return !m_io->isKeyDown[static_cast<uint64>(in_key)] && m_io->prevKeyDown[static_cast<uint64>(in_key)];
}

bool Input::IsMouseDown(eMouse in_mouse)
{
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(static_cast<int>(in_mouse));
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
}

}   // namespace crab