#include "CrabPch.h"

#include "Script.h"

#include "Entity.h"

namespace crab
{

 Script::Script(Entity in_owner)
    : m_owner(in_owner)
{
}

 Script::~Script()
 {
 }

Entity Script::GetEntity()
{
    if (!m_owner.IsValid())
    {
        CRAB_DEBUG_BREAK(" m_owner is nullptr.");
        return Entity::s_null;
    }
    return m_owner;
}

}   // namespace crab