#pragma once

namespace crab
{

//===================================================
// Event Type
//===================================================

enum class eEventType
{
    // - Core Event
    AppClose_CoreEvent = 1,

    // - Window Event
    Resize_WindowEvent,
    Move_WindowEvent,

    // - IO Event
    KeyDown_IOEvent,
    KeyUp_IOEvent,
    MouseScroll_IOEvent,
    MouseUp_IOEvent,
    MouseDown_IOEvent
};

//===================================================
// Event Macro
//===================================================

#define HANDLE_EVENT(EventStruct, func)         \
    if (dispatcher.Dispatch<EventStruct>(func)) \
    return

#define EVENT_IMPL(EventStruct)                                            \
    inline static const eEventType s_staticType = eEventType::EventStruct; \
    inline static const char*      s_staticName = #EventStruct

//===================================================
// Event Struct
//===================================================

struct CrabEvent
{
    eEventType type;
    void*      eventData = nullptr;
    bool       isHandled = false;
};

//===================================================
// Concrete Event Data
//===================================================

struct AppClose_CoreEvent
{
    EVENT_IMPL(AppClose_CoreEvent);
};

struct Move_WindowEvent
{
    EVENT_IMPL(Move_WindowEvent);

    Int2 position;
};

struct Resize_WindowEvent
{
    EVENT_IMPL(Resize_WindowEvent);

    Int2 size;
};
struct KeyDown_IOEvent
{
    EVENT_IMPL(KeyDown_IOEvent);

    uint32 key;
};

struct KeyUp_IOEvent
{
    EVENT_IMPL(KeyUp_IOEvent);

    uint32 key;
};

struct MouseScroll_IOEvent
{
    EVENT_IMPL(MouseScroll_IOEvent);

    Vec2 deltaScroll;
};

struct MouseDown_IOEvent
{
    EVENT_IMPL(MouseDown_IOEvent);
    int32 button;
};

struct MouseUp_IOEvent
{
    EVENT_IMPL(MouseUp_IOEvent);
    int32 button;
};

}   // namespace crab
