#include "CrabPch.h"

#include "Entity.h"
#include "Scene.h"

namespace crab
{

crab::Entity Entity::s_null = Entity(nullptr, entt::null);

Entity::Entity(Scene* in_scene, entt::entity in_entity)
    : m_registry(&in_scene->GetRegistry())
    , m_entity(in_entity)
{
}

Entity::Entity()
{
    *this = s_null;
}

}   // namespace crab