#include "EventDispatcher.h"
#include "CrabPch.h"

namespace crab
{

EventDispatcher::EventDispatcher(const CrabEvent& in_event)
    : m_event(in_event)
{
}

}   // namespace crab
