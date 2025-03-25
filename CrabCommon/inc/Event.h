#pragma once
#include "CrabMacro.h"

namespace crab
{

//===================================================
// Event Base
// use this class to create event
//===================================================

class EventBase
{
public:
    EventBase()          = default;
    virtual ~EventBase() = default;

    void BlockEvent() { m_block = true; }
    bool IsBlocked() const { return m_block; }

    virtual std::string ToString() const     = 0;
    virtual const char* GetEventName() const = 0;
    virtual TypeInfo    GetTypeInfo() const  = 0;

private:
    bool m_block = false;
};

#define EVENT_IMPL(EventType, ToStringText)                                         \
    const char* GetEventName() const override { return #EventType; }                \
    TypeInfo    GetTypeInfo() const override { return TypeInfo::Get<EventType>(); } \
    std::string ToString() const override { return ToStringText; }

//===================================================
// Event Dispatcher
//===================================================

class EventDispatcher
{
public:
    EventDispatcher(EventBase* in_event);

    template<typename EventType>
    bool Dispatch(const std::function<void(EventType*)>& in_callback)
    {
        static_assert(IS_BASE_OF(EventBase, EventType), "EventType must be derived from EventBase");
        if (m_event->GetTypeInfo() == TypeInfo::Get<EventType>() && !m_event->IsBlocked())
        {
            in_callback(static_cast<EventType*>(m_event));
            return true;
        }
        return false;
    }

private:
    EventBase* m_event;
};

#define DISPATCH_EVENT(EventStruct, func)       \
    if (dispatcher.Dispatch<EventStruct>(func)) \
    return

}   // namespace crab