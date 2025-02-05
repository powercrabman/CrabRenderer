#pragma once
#include "CrabEvent.h"

namespace crab
{

class EventDispatcher
{
public:
    EventDispatcher(const CrabEvent& in_event);

    template<typename T>
    bool Dispatch(const std::function<void(const T&)>& in_func)
    {
        if (!m_event.isHandled && m_event.type == T::s_staticType)
        {
            T* data = static_cast<T*>(m_event.eventData);
            in_func(*data);
            return true;
        }
        return false;
    }

private:
    CrabEvent m_event;
};

}   // namespace crab
