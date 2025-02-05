#pragma once

namespace crab
{

struct TimeStamp;

class ILayer
{
public:
    virtual void OnAttach()  = 0;
    virtual void OnDetach() = 0;

    virtual void OnUpdate(TimeStamp& in_ts)    = 0;
    virtual void OnRender(TimeStamp& in_ts)    = 0;
    virtual void OnRenderGUI(TimeStamp& in_ts) = 0;

    virtual void OnEvent(CrabEvent& in_event) = 0;

    virtual constexpr TypeInfo GetTypeInfo() const = 0;
};

}   // namespace crab
