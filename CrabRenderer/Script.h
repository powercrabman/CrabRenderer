#pragma once
#include "Entity.h"

namespace crab
{

class Script
{
public:
    Script(Entity in_owner);
    virtual ~Script();

    virtual void OnUpdate(TimeStamp& in_ts) {}

    virtual Entity GetEntity() final;

private:
    Entity m_owner;
};

// todo - maybe add more functions like OnRender, OnEvent, etc.

}   // namespace crab
