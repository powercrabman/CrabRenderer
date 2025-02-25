#include "CrabPch.h"

#include "Script.h"

#include "Entity.h"

namespace crab
{

 Script::Script(Entity in_owner)
    : m_ownerEntity(in_owner)
{
}

 Script::~Script()
 {
 }

Entity Script::GetEntity()
{
    if (!m_ownerEntity.IsValid())
    {
        CRAB_DEBUG_BREAK(" m_ownerEntity is nullptr.");
        return Entity::s_null;
    }
    return m_ownerEntity;
}

}   // namespace crab