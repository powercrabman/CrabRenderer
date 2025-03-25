#pragma once
#include "Entity.h"

namespace crab
{

struct TimeStamp;

class Script
{
    friend class SceneManager;

public:
    Script(Entity in_owner);
    virtual ~Script();

    virtual void Init()                     = 0;
    virtual void OnUpdate(TimeStamp& in_ts) = 0;

    virtual Entity GetEntity() final;

    bool IsInited() const { return m_isInited; }

protected:
    Entity m_ownerEntity;

private:
    bool m_isInited = false;
};

}   // namespace crab
