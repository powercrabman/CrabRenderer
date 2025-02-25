#pragma once
#include "Entity.h"

namespace crab
{

class Script
{
public:
    Script(Entity in_owner);
    virtual ~Script();

    virtual void OnUpdate(TimeStamp& in_ts) = 0;

    virtual Entity GetEntity() final;

protected:
    Entity m_ownerEntity;
};

// todo - maybe add more functions like OnRender, OnEvent, etc.

}   // namespace crab
