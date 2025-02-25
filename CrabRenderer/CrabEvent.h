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

#define IMPLEMENT_EVENT(EventStruct)                                       \
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
    IMPLEMENT_EVENT(AppClose_CoreEvent);
};

struct Move_WindowEvent
{
    IMPLEMENT_EVENT(Move_WindowEvent);

    uint32 x;
    uint32 y;
};

struct Resize_WindowEvent
{
    IMPLEMENT_EVENT(Resize_WindowEvent);

    uint32 width;
    uint32 height;
};
struct KeyDown_IOEvent
{
    IMPLEMENT_EVENT(KeyDown_IOEvent);

    uint32 key;
};

struct KeyUp_IOEvent
{
    IMPLEMENT_EVENT(KeyUp_IOEvent);

    uint32 key;
};

struct MouseScroll_IOEvent
{
    IMPLEMENT_EVENT(MouseScroll_IOEvent);

    int32 dx;
    int32 dy;
};

struct MouseDown_IOEvent
{
    IMPLEMENT_EVENT(MouseDown_IOEvent);
    int32 button;
};

struct MouseUp_IOEvent
{
    IMPLEMENT_EVENT(MouseUp_IOEvent);
    int32 button;
};

}   // namespace crab
