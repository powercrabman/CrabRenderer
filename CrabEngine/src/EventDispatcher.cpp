#include "InternalPCH.h"

#include "EventDispatcher.h"

namespace crab
{

EventDispatcher::EventDispatcher(EventBase* in_event)
    : m_event(in_event)
{
}

}   // namespace crab
